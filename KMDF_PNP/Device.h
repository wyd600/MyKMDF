//#ifndef DEVICE_H
//#define DEVICE_H //��ֹͷ�ļ��Ѿ������� hdwwiz

#include <ntddk.h>
#include <wdf.h>

typedef struct _DEVICE_CONTEXT//�����豸����������
{
	WDFTIMER Timer;//��ʱ������
	WDFQUEUE Queue;//���ж���
}DEVICE_CONTEXT, * PDEVICE_CONTEXT;

//����һ���������Ƶ�����������
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext);

//
NTSTATUS EvtDriverDeviceAdd(_In_ WDFDRIVER Driver, _Inout_ PWDFDEVICE_INIT DeviceInit);

NTSTATUS MyTimerCreate(IN WDFTIMER* Timer, IN WDFDEVICE Device);

//������Ӧ��Driver.h�ж���
typedef struct _DRIVER_CONTEXT
{
	PULONG Number;
	WCHAR sz[1024];
	ULONG Boolean;
}DRIVER_CONTEXT, * PDRIVER_CONTEXT;
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DRIVER_CONTEXT, GetDriverContext);

NTSTATUS LoadParameter(IN WDFDRIVER Driver);
