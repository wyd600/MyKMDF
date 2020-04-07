#include <ntddk.h>
#include <wdf.h>
#include "Device.h"

#define IOCTL_TEST CTL_CODE(FILE_DEVICE_UNKNOWN,0X800,METHOD_BUFFERED,FILE_ANY_ACCESS)
void EvtIoDeviceControl(_In_ WDFQUEUE Queue, _In_ WDFREQUEST Request, _In_ size_t OutputBufferLength, _In_ size_t InputBufferLength, _In_ ULONG IoControlCode)
{
	KdPrint(("������IO����\n"));
	PVOID Buffer = NULL;
	size_t Length = 0;
	NTSTATUS status;
	CHAR n;//���洫����ַ�
	CHAR cc[] = "��һ�����������߰˾�";//�����Ӧ���ַ�
	PDEVICE_CONTEXT pDeviceContext = NULL;
	switch (IoControlCode)
	{
	case IOCTL_TEST:
		if (InputBufferLength == 0 || OutputBufferLength < 2)//�ж��������������С���ж���Ч��
		{
			KdPrint(("������Ч\n"));
			WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);//������Ч
			break;
		}
		pDeviceContext = GetDeviceContext(WdfIoQueueGetDevice(Queue));
		status = WdfRequestForwardToIoQueue(Request, pDeviceContext->Queue);//����Ϸ�����ѹ��IO����
		if (!NT_SUCCESS(status))
		{
			KdPrint(("ѹ�����ʧ��%d\n", status));
			WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);//���ʧ�ܣ�����ɵ�ǰ����
			return;
		}
		WdfTimerStart(pDeviceContext->Timer, WDF_REL_TIMEOUT_IN_SEC(3));//������ʱ�� 3s�ȴ�ʱ��
		////������Ч
		//status = WdfRequestRetrieveInputBuffer(Request, 1, &Buffer, &Length);//ȡ�����뻺������ַ
		//if (!NT_SUCCESS(status))
		//{
		//	KdPrint(("ȡ�����뻺������ַʧ��\n"));
		//	WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		//	break;
		//}
		//n = *(PCHAR)Buffer;
		//if (n >= '0' && n <= '9')
		//{
		//	n -= '0';
		//	status = WdfRequestRetrieveOutputBuffer(Request, 1, &Buffer, &Length);//�������з������������
		//	if (!NT_SUCCESS(status))
		//	{
		//		WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		//		KdPrint(("�������з������������ʧ��\n"));
		//		break;
		//	}
		//	strncpy(Buffer, &cc[n * 2], 2);
		//	WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 2);//�ɹ�
		//}
		//else
		//{
		//	KdPrint(("������Χ\n"));
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
	CHAR n;//���洫����ַ�
	CHAR cc[] = "��һ�����������߰˾�";//�����Ӧ���ַ�


	Device = WdfTimerGetParentObject(Timer);//��ȡ�豸������
	pDeviceContext = GetDeviceContext(Device);//��ȡ������
	//��ȡio��������
	status = WdfIoQueueRetrieveNextRequest(pDeviceContext->Queue, &Request);

	if (!NT_SUCCESS(status))
	{
		KdPrint(("��ȡ����ʧ��%d\n", status));
		return;
	}
	else
	{
		//������Ч
		status = WdfRequestRetrieveInputBuffer(Request, 1, &Buffer, &Length);//ȡ�����뻺������ַ
		if (!NT_SUCCESS(status))
		{
			KdPrint(("ȡ�����뻺������ַʧ��\n"));
			WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		}
		n = *(PCHAR)Buffer;
		if (n >= '0' && n <= '9')
		{
			n -= '0';
			status = WdfRequestRetrieveOutputBuffer(Request, 1, &Buffer, &Length);//�������з������������
			if (!NT_SUCCESS(status))
			{
				WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
				KdPrint(("�������з������������ʧ��\n"));
				return;
			}
			strncpy(Buffer, &cc[n * 2], 2);
			WdfRequestCompleteWithInformation(Request, STATUS_SUCCESS, 2);//�ɹ�
		}
		else
		{
			KdPrint(("������Χ\n"));
			WdfRequestComplete(Request, STATUS_INVALID_PARAMETER);
		}
	}
}

///ȡ����ʱ��
VOID EvtIoCanceledOnQueue(_In_ WDFQUEUE Queue, _In_ WDFREQUEST Request)
{
	WDFDEVICE Device = WdfIoQueueGetDevice(Queue);
	PDEVICE_CONTEXT pDeviceContext = GetDeviceContext(Device);
	//ȡ������
	WdfRequestCompleteWithInformation(Request, STATUS_CANCELLED, 0);
	//��ͣ��ʱ�� false���ȴ�
	WdfTimerStop(pDeviceContext->Timer, FALSE);
}