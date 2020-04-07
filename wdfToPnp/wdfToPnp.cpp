// wdfToPnp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <initguid.h>
#include <SetupAPI.h>


#pragma comment(lib,"setupapi.lib")
#define IOCTL_TEST CTL_CODE(FILE_DEVICE_UNKNOWN,0X800,METHOD_BUFFERED,FILE_ANY_ACCESS)

DEFINE_GUID(DEVICEINTERFACE,
	0xd823ea9f, 0xd4d0, 0x4cf0, 0xbf, 0x83, 0xb0, 0x63, 0xee, 0x1a, 0x7f, 0x5f);

LPTSTR GetDevicePath()
{
	HDEVINFO hInfo = SetupDiGetClassDevs(&DEVICEINTERFACE, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (hInfo == NULL)
	{
		return NULL;
	}
	SP_DEVICE_INTERFACE_DATA ifdata = { 0 };
	ifdata.cbSize = sizeof(ifdata);
	if (!SetupDiEnumDeviceInterfaces(hInfo, NULL, &DEVICEINTERFACE, 0, &ifdata))
	{
		printf("枚举接口失败%d\n", GetLastError());
		return NULL;
	}
	PSP_DEVICE_INTERFACE_DETAIL_DATA pdetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(1024);
	DWORD dwSize = 0;
	RtlZeroMemory(pdetailData, 1024);//清空内存
	pdetailData->cbSize = sizeof(PSP_DEVICE_INTERFACE_DETAIL_DATA);
	if (!SetupDiGetDeviceInterfaceDetail(hInfo, &ifdata, pdetailData, 1024, &dwSize, NULL))//获取设备接口的详细数据，就可以拿到符号链接
	{
		return NULL;
	}
	else
	{
		return pdetailData->DevicePath;
	}
}

int main()
{
	LPTSTR devicePath = NULL;
	if ((devicePath = GetDevicePath()) != NULL)
	{
		printf("%ls\n", devicePath);
		HANDLE hDevice = CreateFile(devicePath, FILE_GENERIC_READ | GENERIC_READ | GENERIC_EXECUTE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);//拿到符号链接之后，打开符号链接
		if (hDevice == INVALID_HANDLE_VALUE)//判断获取是否成功
		{
			printf("打开失败%d\n", GetLastError());
			return -1;
		}
		//DWORD dwret;
		//DeviceIoControl(hDevice, IOCTL_TEST, NULL, 0, NULL, 0, &dwret, NULL);//传入队列里面的控制码
		//CloseHandle(hDevice);//关闭

		DWORD dwret;
		printf("请输入一个数字：\n");
		char n = getchar();
		char buffer[4] = { 0 };
		DeviceIoControl(hDevice, IOCTL_TEST, &n, sizeof(n), buffer, sizeof(buffer), &dwret, NULL);//传入队列里面的控制码
		printf("%s\n", buffer);
		CloseHandle(hDevice);//关闭
	}
	else
	{
		printf("设备接口获取失败%d\n", GetLastError());
	}

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
