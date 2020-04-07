#include <ntddk.h>
#include <wdf.h>
#include "Device.h"

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING ResgistryPath)
{
	NTSTATUS status = STATUS_SUCCESS;
	//��ȡע�������
	WDF_OBJECT_ATTRIBUTES driverAttributes;



	WDF_DRIVER_CONFIG config;
	PDRIVER_CONTEXT pDriverContext = NULL;
	WDFDRIVER Driver;
	WDF_DRIVER_CONFIG_INIT(&config, EvtDriverDeviceAdd);

	//��ȡע�������
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&driverAttributes, DRIVER_CONTEXT);

	//�����������
	status = WdfDriverCreate(DriverObject, ResgistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &Driver);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("ע�������Ŀ�ܶ���ʧ��%d\n", status));
	}
	else
	{
		KdPrint(("ע�������Ŀ�ܶ���ɹ�%d\n", status));
		//�õ�����������
		pDriverContext = GetDriverContext(Driver);
		//���ز���
		LoadParameter(Driver);
	}

	return status;
}

NTSTATUS LoadParameter(IN WDFDRIVER Driver)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDRIVER_CONTEXT pDriverContext = NULL;
	WDFKEY hkey;
	UNICODE_STRING ValueName;
	ULONG len;
	UNICODE_STRING usValue;
	pDriverContext = GetDriverContext(Driver);


	WdfDriverOpenParametersRegistryKey(Driver, KEY_READ | KEY_WRITE, WDF_NO_OBJECT_ATTRIBUTES, &hkey);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("��ע������ʧ��%d\n", status));
		return status;
	}
	RtlInitUnicodeString(&ValueName, L"����");
	status = WdfRegistryQueryULong(hkey, &ValueName, pDriverContext->Number);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("��ע������ֲ���ʧ��%d\n", status));
		return status;
	}
	/*RtlInitUnicodeString(&ValueName, L"����");
	status = WdfRegistryQueryULong(hkey, &ValueName, pDriverContext->Boolean);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("��ע���������ʧ��%d\n", status));
		return status;
	}
	usValue.Buffer = pDriverContext->sz;
	usValue.MaximumLength = sizeof(pDriverContext->sz);
	usValue.Length = 0;
	RtlInitUnicodeString(&ValueName, L"�ַ�");
	status = WdfRegistryQueryUnicodeString(hkey, &ValueName, &len, &usValue);
	if (NT_SUCCESS(status))
	{
		KdPrint(("�ַ���%wZ\n", &usValue));
	}
	else
	{
		KdPrint(("��ע������ֲ���ʧ��%d\n", status));
		return status;
	}*/
	return status;

}