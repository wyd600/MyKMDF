#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>//为了添加guid
#include "Queue.h"
#include "Device.h"

// {D823EA9F-D4D0-4CF0-BF83-B063EE1A7F5F}
DEFINE_GUID(DEVICEINTERFACE,
	0xd823ea9f, 0xd4d0, 0x4cf0, 0xbf, 0x83, 0xb0, 0x63, 0xee, 0x1a, 0x7f, 0x5f);


NTSTATUS EvtDriverDeviceAdd(_In_ WDFDRIVER Driver, _Inout_ PWDFDEVICE_INIT DeviceInit)
{
	//这句话表示该程序的代码占用分页内存
	//只能在PASSIVE_LEVEL中断级别调用该程序，否则会蓝屏
	//入股不说明，则占用系统的非分页内存
	PAGED_CODE();

	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE Device;
	WDFQUEUE Queue;
	PDEVICE_CONTEXT pDeviceContext;
	WDF_IO_QUEUE_CONFIG IoConfig;

#pragma region 计时器使用
	WDF_OBJECT_ATTRIBUTES deviceAttributes;//定义设备对象的属性
	//初始化deviceAttributes机构提
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);
	deviceAttributes.SynchronizationScope = WdfSynchronizationScopeDevice;//同步
#pragma endregion

	//设置设备是否独占，设置为true表示只能有一个程序打开设备
	//WdfDeviceInitSetExclusive(Device, TRUE); //暂时注释，使用方式有误，传入参数有误


	//创建设备 --添加计时器属性
	status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &Device);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("设备创建失败\n"));
		return status;
	}

#pragma region 创建计时器
	pDeviceContext = GetDeviceContext(Device);
	status = MyTimerCreate(&pDeviceContext->Timer, Device);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("设备计时器创建失败%d\n", status));
		return status;
	}
#pragma endregion


	//初始化io队列对象  --顺序队列
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&IoConfig, WdfIoQueueDispatchSequential);
	//提供队列对象的事件回调
	IoConfig.EvtIoDeviceControl = EvtIoDeviceControl;
	//创建队列 之前首先创建设备对象
	status = WdfIoQueueCreate(Device, &IoConfig, WDF_NO_OBJECT_ATTRIBUTES, &Queue);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("队列创建失败\n"));
	}

#pragma region 创建手动队列
	WDF_IO_QUEUE_CONFIG_INIT(&IoConfig, WdfIoQueueDispatchManual);//初始化一个手动队列
	//创建手动队列取消函数
	IoConfig.EvtIoCanceledOnQueue = EvtIoCanceledOnQueue;
	//创建手动队列
	status = WdfIoQueueCreate(Device, &IoConfig, WDF_NO_OBJECT_ATTRIBUTES, &pDeviceContext->Queue);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("手动队列创建失败\n"));
	}
#pragma endregion


	//给设备创建设备接口 设备就可以多次插入
	WdfDeviceCreateDeviceInterface(Device, &DEVICEINTERFACE, NULL);
	KdPrint(("创建设备成功\n"));

	return status;
}

//创建一个计时器
NTSTATUS MyTimerCreate(IN WDFTIMER* Timer, IN WDFDEVICE Device)
{
	WDF_TIMER_CONFIG timerConfig;
	NTSTATUS status;
	WDF_OBJECT_ATTRIBUTES timerAttributes;
	//初始化计时器配置
	WDF_TIMER_CONFIG_INIT(&timerConfig, EvtTimerFunc);
	//初始化计时器对象特性
	WDF_OBJECT_ATTRIBUTES_INIT(&timerAttributes);
	timerAttributes.ParentObject = Device;
	//创建计时器对象
	status = WdfTimerCreate(&timerConfig, &timerAttributes, Timer);
	return status;
}