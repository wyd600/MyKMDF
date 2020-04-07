#include <ntddk.h>
#include <wdf.h>

#pragma region 01
//void EvtDriverUnload(IN WDFDRIVER Driver)
//{
//	KdPrint(("����ж��\n"));
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
//		KdPrint(("������ܴ���ʧ��\n"));
//	}
//	else
//	{
//		KdPrint(("������ܴ����ɹ�\n"));
//	}
//	return status;
//}
#pragma endregion

#pragma region 02
//void EvtDriverUnload(IN WDFDRIVER Driver)
//{
//	KdPrint(("����ж��\n"));
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
//	KdPrint(("����IO����\n"));
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
//	WDF_IO_QUEUE_CONFIG ioConfig;//io����
//	WDFQUEUE Queue;//���о��
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
//		KdPrint(("������ܴ���ʧ��\n"));
//	}
//	else
//	{
//		KdPrint(("������ܴ����ɹ�\n"));
//	}
//	DeviceInit = WdfControlDeviceInitAllocate(Driver, &SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RWX_RES_RWX);
//	if (DeviceInit == NULL)
//	{
//		status = STATUS_INSUFFICIENT_RESOURCES;//����豸Ϊ�գ����ʾ��Դ����
//		goto End;
//	}
//	status = WdfDeviceInitAssignName(DeviceInit, &DeviceName);
//	if (!NT_SUCCESS(status))
//	{
//		KdPrint(("�豸���Ʒ���ʧ��%x\n", status));
//		goto End;
//	}
//	//�ļ��Ĵ������رգ�����
//	WDF_FILEOBJECT_CONFIG_INIT(&FileConfig, EvtDeviceFileCreate, EvtFileClose, EvtFileClearup);
//	//��ʼ��
//	WdfDeviceInitSetFileObjectConfig(DeviceInit, &FileConfig, WDF_NO_OBJECT_ATTRIBUTES);
//
//	//��ʼ��io���� --���ж���
//	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioConfig, WdfIoQueueDispatchSequential);
//	ioConfig.EvtIoDeviceControl = EvtIoDeviceControl;
//
//
//	//�����豸����
//	status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &Device);
//	if (!NT_SUCCESS(status))
//	{
//		KdPrint(("�豸����ʧ��%x\n", status));
//		goto End;
//	}
//
//	//���豸���󴴽�IO����
//	status = WdfIoQueueCreate(Device, &ioConfig, WDF_NO_OBJECT_ATTRIBUTES, &Queue);
//	if (!NT_SUCCESS(status))
//	{
//		KdPrint(("���д���ʧ��%x\n", status));
//		goto End;
//	}
//
//	//������������
//	status = WdfDeviceCreateSymbolicLink(Device, &SymbolLinkName);
//	if (!NT_SUCCESS(status))
//	{
//		KdPrint(("������������ʧ��%x\n", status));
//		goto End;
//	}
//	//���豸������г�ʼ��
//	WdfControlFinishInitializing(Device);
//
//End:
//	return status;
//}




#pragma endregion

#pragma region 06 --����PNP��wdf��������
void EvtDriverUnload(IN WDFDRIVER Driver)
{
	KdPrint(("����ж��\n"));
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
	KdPrint(("����IO����\n"));
	WdfRequestComplete(Request, STATUS_SUCCESS);
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING ResgistryPath)
{
	WDF_DRIVER_CONFIG config;
	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE Device;
	WDFDRIVER Driver;
	WDF_FILEOBJECT_CONFIG FileConfig;
	WDF_IO_QUEUE_CONFIG ioConfig;//io����
	WDFQUEUE Queue;//���о��
	UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\HelloWDF");
	UNICODE_STRING SymbolLinkName = RTL_CONSTANT_STRING(L"\\??\\HelloWDF");
	PWDFDEVICE_INIT DeviceInit;

	WDF_DRIVER_CONFIG_INIT(&config, NULL);
	config.DriverInitFlags = WdfDriverInitNonPnpDriver;
	config.EvtDriverUnload = EvtDriverUnload;

	status = WdfDriverCreate(DriverObject, ResgistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &Driver);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("������ܴ���ʧ��\n"));
	}
	else
	{
		KdPrint(("������ܴ����ɹ�\n"));
	}
	DeviceInit = WdfControlDeviceInitAllocate(Driver, &SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RWX_RES_RWX);
	if (DeviceInit == NULL)
	{
		status = STATUS_INSUFFICIENT_RESOURCES;//����豸Ϊ�գ����ʾ��Դ����
		goto End;
	}
	status = WdfDeviceInitAssignName(DeviceInit, &DeviceName);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("�豸���Ʒ���ʧ��%x\n", status));
		goto End;
	}
	//�ļ��Ĵ������رգ�����
	WDF_FILEOBJECT_CONFIG_INIT(&FileConfig, EvtDeviceFileCreate, EvtFileClose, EvtFileClearup);
	//��ʼ��
	WdfDeviceInitSetFileObjectConfig(DeviceInit, &FileConfig, WDF_NO_OBJECT_ATTRIBUTES);

	//��ʼ��io���� --���ж���
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioConfig, WdfIoQueueDispatchSequential);
	ioConfig.EvtIoDeviceControl = EvtIoDeviceControl;


	//�����豸����
	status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &Device);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("�豸����ʧ��%x\n", status));
		goto End;
	}

	//���豸���󴴽�IO����
	status = WdfIoQueueCreate(Device, &ioConfig, WDF_NO_OBJECT_ATTRIBUTES, &Queue);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("���д���ʧ��%x\n", status));
		goto End;
	}

	//������������
	status = WdfDeviceCreateSymbolicLink(Device, &SymbolLinkName);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("������������ʧ��%x\n", status));
		goto End;
	}
	//���豸������г�ʼ��
	WdfControlFinishInitializing(Device);

End:
	return status;
}
#pragma endregion


