#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>//Ϊ�����guid
#include "Queue.h"
#include "Device.h"

// {D823EA9F-D4D0-4CF0-BF83-B063EE1A7F5F}
DEFINE_GUID(DEVICEINTERFACE,
	0xd823ea9f, 0xd4d0, 0x4cf0, 0xbf, 0x83, 0xb0, 0x63, 0xee, 0x1a, 0x7f, 0x5f);


NTSTATUS EvtDriverDeviceAdd(_In_ WDFDRIVER Driver, _Inout_ PWDFDEVICE_INIT DeviceInit)
{
	//��仰��ʾ�ó���Ĵ���ռ�÷�ҳ�ڴ�
	//ֻ����PASSIVE_LEVEL�жϼ�����øó��򣬷��������
	//��ɲ�˵������ռ��ϵͳ�ķǷ�ҳ�ڴ�
	PAGED_CODE();

	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE Device;
	WDFQUEUE Queue;
	PDEVICE_CONTEXT pDeviceContext;
	WDF_IO_QUEUE_CONFIG IoConfig;

#pragma region ��ʱ��ʹ��
	WDF_OBJECT_ATTRIBUTES deviceAttributes;//�����豸���������
	//��ʼ��deviceAttributes������
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);
	deviceAttributes.SynchronizationScope = WdfSynchronizationScopeDevice;//ͬ��
#pragma endregion

	//�����豸�Ƿ��ռ������Ϊtrue��ʾֻ����һ��������豸
	//WdfDeviceInitSetExclusive(Device, TRUE); //��ʱע�ͣ�ʹ�÷�ʽ���󣬴����������


	//�����豸 --��Ӽ�ʱ������
	status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &Device);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("�豸����ʧ��\n"));
		return status;
	}

#pragma region ������ʱ��
	pDeviceContext = GetDeviceContext(Device);
	status = MyTimerCreate(&pDeviceContext->Timer, Device);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("�豸��ʱ������ʧ��%d\n", status));
		return status;
	}
#pragma endregion


	//��ʼ��io���ж���  --˳�����
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&IoConfig, WdfIoQueueDispatchSequential);
	//�ṩ���ж�����¼��ص�
	IoConfig.EvtIoDeviceControl = EvtIoDeviceControl;
	//�������� ֮ǰ���ȴ����豸����
	status = WdfIoQueueCreate(Device, &IoConfig, WDF_NO_OBJECT_ATTRIBUTES, &Queue);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("���д���ʧ��\n"));
	}

#pragma region �����ֶ�����
	WDF_IO_QUEUE_CONFIG_INIT(&IoConfig, WdfIoQueueDispatchManual);//��ʼ��һ���ֶ�����
	//�����ֶ�����ȡ������
	IoConfig.EvtIoCanceledOnQueue = EvtIoCanceledOnQueue;
	//�����ֶ�����
	status = WdfIoQueueCreate(Device, &IoConfig, WDF_NO_OBJECT_ATTRIBUTES, &pDeviceContext->Queue);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("�ֶ����д���ʧ��\n"));
	}
#pragma endregion


	//���豸�����豸�ӿ� �豸�Ϳ��Զ�β���
	WdfDeviceCreateDeviceInterface(Device, &DEVICEINTERFACE, NULL);
	KdPrint(("�����豸�ɹ�\n"));

	return status;
}

//����һ����ʱ��
NTSTATUS MyTimerCreate(IN WDFTIMER* Timer, IN WDFDEVICE Device)
{
	WDF_TIMER_CONFIG timerConfig;
	NTSTATUS status;
	WDF_OBJECT_ATTRIBUTES timerAttributes;
	//��ʼ����ʱ������
	WDF_TIMER_CONFIG_INIT(&timerConfig, EvtTimerFunc);
	//��ʼ����ʱ����������
	WDF_OBJECT_ATTRIBUTES_INIT(&timerAttributes);
	timerAttributes.ParentObject = Device;
	//������ʱ������
	status = WdfTimerCreate(&timerConfig, &timerAttributes, Timer);
	return status;
}