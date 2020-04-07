#include <ntddk.h>
#include <wdf.h>
#include "Device.h"

#define IOCTL_TEST CTL_CODE(FILE_DEVICE_UNKNOWN,0X800,METHOD_BUFFERED,FILE_ANY_ACCESS)
void EvtIoDeviceControl(_In_ WDFQUEUE Queue, _In_ WDFREQUEST Request, _In_ size_t OutputBufferLength, _In_ size_t InputBufferLength, _In_ ULONG IoControlCode)
{
	KdPrint(("有驱动IO操作\n"));
	PVOID Buffer = NULL;
	size_t Length = 0;
	NTSTATUS status;
	CHAR n;//保存传入的字符
	CHAR cc[] = "零一二三四五六七八九";//保存对应的字符
	PDEVICE_CONTEXT pDeviceContext = NULL;
	switch (IoControlCode)
	{
	case IOCTL_TEST:
		if (InputBufferLength == 0 || OutputBufferLength < 2)//判断输入输出参数大小来判断有效性
		{
			KdPrint(("参数无效\n"));
			WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);//参数无效
			break;
		}
		pDeviceContext = GetDeviceContext(WdfIoQueueGetDevice(Queue));
		status = WdfRequestForwardToIoQueue(Request, pDeviceContext->Queue);//请求合法，则压入IO队列
		if (!NT_SUCCESS(status))
		{
			KdPrint(("压入队列失败%d\n", status));
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);//标记失败，并完成当前请求
			return;
		}
		WdfTimerStart(pDeviceContext->Timer, WDF_REL_TIMEOUT_IN_SEC(3));//开启定时器 3s等待时间
		////参数有效
		//status = WdfRequestRetrieveInputBuffer(Request, 1, &Buffer, &Length);//取出输入缓冲区地址
		//if (!NT_SUCCESS(status))
		//{
		//	KdPrint(("取出输入缓冲区地址失败\n"));
		//	WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		//	break;
		//}
		//n = *(PCHAR)Buffer;
		//if (n >= '0' && n <= '9')
		//{
		//	n -= '0';
		//	status = WdfRequestRetrieveOutputBuffer(Request, 1, &Buffer, &Length);//从请求中返回输出缓冲区
		//	if (!NT_SUCCESS(status))
		//	{
		//		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		//		KdPrint(("从请求中返回输出缓冲区失败\n"));
		//		break;
		//	}
		//	strncpy(Buffer, &cc[n * 2], 2);
		//	WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 2);//成功
		//}
		//else
		//{
		//	KdPrint(("超出范围\n"));
		//	WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		//}
		break;
	default:
		WdfRequestCompleteWithInformation(Request, STATUS_INVALID_PARAMETER, 0);
		break;
	}
	//WdfRequestComplete(Request, STATUS_SUCCESS);
}

VOID EvtTimerFunc(_In_ WDFTIMER Timer)
{
	WDFDEVICE Device;
	PDEVICE_CONTEXT pDeviceContext = NULL;
	WDFREQUEST Request;
	NTSTATUS status;

	PVOID Buffer = NULL;
	size_t Length = 0;
	CHAR n;//保存传入的字符
	CHAR cc[] = "零一二三四五六七八九";//保存对应的字符


	Device = WdfTimerGetParentObject(Timer);//获取设备对象句柄
	pDeviceContext = GetDeviceContext(Device);//获取上下文
	//获取io队列请求
	status = WdfIoQueueRetrieveNextRequest(pDeviceContext->Queue, &Request);

	if (!NT_SUCCESS(status))
	{
		KdPrint(("获取队列失败%d\n", status));
		return;
	}
	else
	{
		//参数有效
		status = WdfRequestRetrieveInputBuffer(Request, 1, &Buffer, &Length);//取出输入缓冲区地址
		if (!NT_SUCCESS(status))
		{
			KdPrint(("取出输入缓冲区地址失败\n"));
			WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		}
		n = *(PCHAR)Buffer;
		if (n >= '0' && n <= '9')
		{
			n -= '0';
			status = WdfRequestRetrieveOutputBuffer(Request, 1, &Buffer, &Length);//从请求中返回输出缓冲区
			if (!NT_SUCCESS(status))
			{
				WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
				KdPrint(("从请求中返回输出缓冲区失败\n"));
				return;
			}
			strncpy(Buffer, &cc[n * 2], 2);
			WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 2);//成功
		}
		else
		{
			KdPrint(("超出范围\n"));
			WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		}
	}
}

///取消定时器
VOID EvtIoCanceledOnQueue(_In_ WDFQUEUE Queue, _In_ WDFREQUEST Request)
{
	WDFDEVICE Device = WdfIoQueueGetDevice(Queue);
	PDEVICE_CONTEXT pDeviceContext = GetDeviceContext(Device);
	//取消请求
	WdfRequestCompleteWithInformation(Request, STATUS_CANCELLED, 0);
	//关停计时器 false不等待
	WdfTimerStop(pDeviceContext->Timer, FALSE);
}