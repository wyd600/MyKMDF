#include <ntddk.h>
#include <wdf.h>
#include "Device.h"

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING ResgistryPath)
{
	NTSTATUS status = STATUS_SUCCESS;
	//读取注册表数据
	WDF_OBJECT_ATTRIBUTES driverAttributes;



	WDF_DRIVER_CONFIG config;
	PDRIVER_CONTEXT pDriverContext = NULL;
	WDFDRIVER Driver;
	WDF_DRIVER_CONFIG_INIT(&config, EvtDriverDeviceAdd);

	//读取注册表数据
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&driverAttributes, DRIVER_CONTEXT);

	//创建驱动框架
	status = WdfDriverCreate(DriverObject, ResgistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &Driver);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("注册驱动的框架对象失败%d\n", status));
	}
	else
	{
		KdPrint(("注册驱动的框架对象成功%d\n", status));
		//拿到数据上下文
		pDriverContext = GetDriverContext(Driver);
		//加载参数
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
		KdPrint(("打开注册表参数失败%d\n", status));
		return status;
	}
	RtlInitUnicodeString(&ValueName, L"数字");
	status = WdfRegistryQueryULong(hkey, &ValueName, pDriverContext->Number);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("打开注册表数字参数失败%d\n", status));
		return status;
	}
	/*RtlInitUnicodeString(&ValueName, L"布尔");
	status = WdfRegistryQueryULong(hkey, &ValueName, pDriverContext->Boolean);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("打开注册表布尔参数失败%d\n", status));
		return status;
	}
	usValue.Buffer = pDriverContext->sz;
	usValue.MaximumLength = sizeof(pDriverContext->sz);
	usValue.Length = 0;
	RtlInitUnicodeString(&ValueName, L"字符");
	status = WdfRegistryQueryUnicodeString(hkey, &ValueName, &len, &usValue);
	if (NT_SUCCESS(status))
	{
		KdPrint(("字符：%wZ\n", &usValue));
	}
	else
	{
		KdPrint(("打开注册表数字参数失败%d\n", status));
		return status;
	}*/
	return status;

}