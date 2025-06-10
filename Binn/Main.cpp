#include <ntddk.h>
#include "Binn/binn.h"

void DriverUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
}

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgBreakPoint();

	// 初始化
	DriverObject->DriverUnload = DriverUnload;
	NTSTATUS status = STATUS_SUCCESS;

	// 写
	binn* obj = binn_object();
	binn_object_set_int32(obj, "id", 123);
	binn_object_set_str(obj, "name", "John");
	binn_object_set_double(obj, "total", 2.55);
	PVOID bufferPtr = binn_ptr(obj);
	int bufferSize = binn_size(obj);
	UNREFERENCED_PARAMETER(bufferPtr);
	UNREFERENCED_PARAMETER(bufferSize);

	// 读
	UCHAR buffer[] = { 0xE2,0x23,0x03,0x02,0x69,0x64,0x20,0x7B,0x04,0x6E,0x61,0x6D,0x65,0xA0,0x04,0x4A,0x6F,0x68,0x6E,0x00,0x05,0x74
		,0x6F,0x74,0x61,0x6C,0x82,0x40,0x04,0x66,0x66,0x66,0x66,0x66,0x66 };
	obj = binn_open(buffer);
	int id;
	char* name;
	double total;
	id = binn_object_int32(obj, "id");
	name = binn_object_str(obj, "name");
	total = binn_object_double(obj, "total");


	// 用完释放
	binn_free(obj);

	return status;
}