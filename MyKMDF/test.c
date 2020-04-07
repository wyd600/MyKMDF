#include <ntddk.h>
#include <wdf.h>

#pragma region 01
//void EvtDriverUnload(IN WDFDRIVER Driver)
//{
//	KdPrint(("驱动卸载\n"));
//}
//
//extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING ResgistryPath)
//{
//	WDF_DRIVER_CONFIG config;
//	NTSTATUS status = STATUS_SUCCESS;
//
//	WDF_DRIVER_CONFIG_INIT(&config, NULL);
//	config.DriverInitFlags = WdfDriverInitNonPnpDriver;
//	config.EvtDriverUnload = EvtDriverUnload;
//
//	status = WdfDriverCreate(DriverObject, ResgistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);
//	if (!NT_SUCCESS(status))
//	{
//		KdPrint(("驱动框架创建失败\n"));
//	}
//	else
//	{
//		KdPrint(("驱动框架创建成功\n"));
//	}
//	return status;
//}
#pragma endregion

#pragma region 02
//void EvtDriverUnload(IN WDFDRIVER Driver)
//{
//	KdPrint(("驱动卸载\n"));
//}
//VOID EvtDeviceFileCreate(_In_ WDFDEVICE Device, _In_ WDFREQUEST Request, _In_ WDFFILEOBJECT FileObject)
//{
//	WdfRequestComplete(Request, STATUS_SUCCESS);
//	KdPrint(("create\n"));
//}
//
//VOID EvtFileClose(_In_ WDFFILEOBJECT FileObject)
//{
//	KdPrint(("EvtFileClose\n"));
//}
//VOID EvtFileClearup(_In_ WDFFILEOBJECT Fileobject)
//{
//	KdPrint(("EvtFileClearup\n"));
//}
//void EvtIoDeviceControl(_In_ WDFQUEUE Queue, _In_ WDFREQUEST Request, _In_ size_t OutputBufferLength, _In_ size_t InputBufferLength, _In_ ULONG IoControlCode)
//{
//	KdPrint(("驱动IO操作\n"));
//	WdfRequestComplete(Request, STATUS_SUCCESS);
//}
//
//NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING ResgistryPath)
//{
//	WDF_DRIVER_CONFIG config;
//	NTSTATUS status = STATUS_SUCCESS;
//	WDFDEVICE Device;
//	WDFDRIVER Driver;
//	WDF_FILEOBJECT_CONFIG FileConfig;
//	WDF_IO_QUEUE_CONFIG ioConfig;//io队列
//	WDFQUEUE Queue;//队列句柄
//	UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\HelloWDF");
//	UNICODE_STRING SymbolLinkName = RTL_CONSTANT_STRING(L"\\??\\HelloWDF");
//	PWDFDEVICE_INIT DeviceInit;
//
//	WDF_DRIVER_CONFIG_INIT(&config, NULL);
//	config.DriverInitFlags = WdfDriverInitNonPnpDriver;
//	config.EvtDriverUnload = EvtDriverUnload;
//
//	status = WdfDriverCreate(DriverObject, ResgistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &Driver);
//	if (!NT_SUCCESS(status))
//	{
//		KdPrint(("驱动框架创建失败\n"));
//	}
//	else
//	{
//		KdPrint(("驱动框架创建成功\n"));
//	}
//	DeviceInit = WdfControlDeviceInitAllocate(Driver, &SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RWX_RES_RWX);
//	if (DeviceInit == NULL)
//	{
//		status = STATUS_INSUFFICIENT_RESOURCES;//如果设备为空，则表示资源不足
//		goto End;
//	}
//	status = WdfDeviceInitAssignName(DeviceInit, &DeviceName);
//	if (!NT_SUCCESS(status))
//	{
//		KdPrint(("设备名称分配失败%x\n", status));
//		goto End;
//	}
//	//文件的创建，关闭，清理
//	WDF_FILEOBJECT_CONFIG_INIT(&FileConfig, EvtDeviceFileCreate, EvtFileClose, EvtFileClearup);
//	//初始化
//	WdfDeviceInitSetFileObjectConfig(DeviceInit, &FileConfig, WDF_NO_OBJECT_ATTRIBUTES);
//
//	//初始化io队列 --串行队列
//	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioConfig, WdfIoQueueDispatchSequential);
//	ioConfig.EvtIoDeviceControl = EvtIoDeviceControl;
//
//
//	//创建设备对象
//	status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &Device);
//	if (!NT_SUCCESS(status))
//	{
//		KdPrint(("设备创建失败%x\n", status));
//		goto End;
//	}
//
//	//给设备对象创建IO队列
//	status = WdfIoQueueCreate(Device, &ioConfig, WDF_NO_OBJECT_ATTRIBUTES, &Queue);
//	if (!NT_SUCCESS(status))
//	{
//		KdPrint(("队列创建失败%x\n", status));
//		goto End;
//	}
//
//	//创建符号链接
//	status = WdfDeviceCreateSymbolicLink(Device, &SymbolLinkName);
//	if (!NT_SUCCESS(status))
//	{
//		KdPrint(("创建符号链接失败%x\n", status));
//		goto End;
//	}
//	//对设备对象进行初始化
//	WdfControlFinishInitializing(Device);
//
//End:
//	return status;
//}




#pragma endregion

#pragma region 06 --创建PNP的wdf驱动程序
void EvtDriverUnload(IN WDFDRIVER Driver)
{
	KdPrint(("驱动卸载\n"));
}
VOID EvtDeviceFileCreate(_In_ WDFDEVICE Device, _In_ WDFREQUEST Request, _In_ WDFFILEOBJECT FileObject)
{
	WdfRequestComplete(Request, STATUS_SUCCESS);
	KdPrint(("create\n"));
}

VOID EvtFileClose(_In_ WDFFILEOBJECT FileObject)
{
	KdPrint(("EvtFileClose\n"));
}
VOID EvtFileClearup(_In_ WDFFILEOBJECT Fileobject)
{
	KdPrint(("EvtFileClearup\n"));
}
void EvtIoDeviceControl(_In_ WDFQUEUE Queue, _In_ WDFREQUEST Request, _In_ size_t OutputBufferLength, _In_ size_t InputBufferLength, _In_ ULONG IoControlCode)
{
	KdPrint(("驱动IO操作\n"));
	WdfRequestComplete(Request, STATUS_SUCCESS);
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING ResgistryPath)
{
	WDF_DRIVER_CONFIG config;
	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE Device;
	WDFDRIVER Driver;
	WDF_FILEOBJECT_CONFIG FileConfig;
	WDF_IO_QUEUE_CONFIG ioConfig;//io队列
	WDFQUEUE Queue;//队列句柄
	UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\HelloWDF");
	UNICODE_STRING SymbolLinkName = RTL_CONSTANT_STRING(L"\\??\\HelloWDF");
	PWDFDEVICE_INIT DeviceInit;

	WDF_DRIVER_CONFIG_INIT(&config, NULL);
	config.DriverInitFlags = WdfDriverInitNonPnpDriver;
	config.EvtDriverUnload = EvtDriverUnload;

	status = WdfDriverCreate(DriverObject, ResgistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &Driver);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("驱动框架创建失败\n"));
	}
	else
	{
		KdPrint(("驱动框架创建成功\n"));
	}
	DeviceInit = WdfControlDeviceInitAllocate(Driver, &SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RWX_RES_RWX);
	if (DeviceInit == NULL)
	{
		status = STATUS_INSUFFICIENT_RESOURCES;//如果设备为空，则表示资源不足
		goto End;
	}
	status = WdfDeviceInitAssignName(DeviceInit, &DeviceName);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("设备名称分配失败%x\n", status));
		goto End;
	}
	//文件的创建，关闭，清理
	WDF_FILEOBJECT_CONFIG_INIT(&FileConfig, EvtDeviceFileCreate, EvtFileClose, EvtFileClearup);
	//初始化
	WdfDeviceInitSetFileObjectConfig(DeviceInit, &FileConfig, WDF_NO_OBJECT_ATTRIBUTES);

	//初始化io队列 --串行队列
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioConfig, WdfIoQueueDispatchSequential);
	ioConfig.EvtIoDeviceControl = EvtIoDeviceControl;


	//创建设备对象
	status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &Device);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("设备创建失败%x\n", status));
		goto End;
	}

	//给设备对象创建IO队列
	status = WdfIoQueueCreate(Device, &ioConfig, WDF_NO_OBJECT_ATTRIBUTES, &Queue);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("队列创建失败%x\n", status));
		goto End;
	}

	//创建符号链接
	status = WdfDeviceCreateSymbolicLink(Device, &SymbolLinkName);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("创建符号链接失败%x\n", status));
		goto End;
	}
	//对设备对象进行初始化
	WdfControlFinishInitializing(Device);

End:
	return status;
}
#pragma endregion


