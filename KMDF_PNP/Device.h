//#ifndef DEVICE_H
//#define DEVICE_H //防止头文件已经被定义 hdwwiz

#include <ntddk.h>
#include <wdf.h>

typedef struct _DEVICE_CONTEXT//定义设备对象上下文
{
	WDFTIMER Timer;//定时器对象
	WDFQUEUE Queue;//队列对象
}DEVICE_CONTEXT, * PDEVICE_CONTEXT;

//声明一个带有名称的上下文类型
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext);

//
NTSTATUS EvtDriverDeviceAdd(_In_ WDFDRIVER Driver, _Inout_ PWDFDEVICE_INIT DeviceInit);

NTSTATUS MyTimerCreate(IN WDFTIMER* Timer, IN WDFDEVICE Device);

//该类型应在Driver.h中定义
typedef struct _DRIVER_CONTEXT
{
	PULONG Number;
	WCHAR sz[1024];
	ULONG Boolean;
}DRIVER_CONTEXT, * PDRIVER_CONTEXT;
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DRIVER_CONTEXT, GetDriverContext);

NTSTATUS LoadParameter(IN WDFDRIVER Driver);
