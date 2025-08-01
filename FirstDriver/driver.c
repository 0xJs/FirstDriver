#include <ntddk.h>

#define DRIVER_NAME "FirstDriver"

VOID UnloadDriver(IN PDRIVER_OBJECT DriverObject);

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath) {

	// Unreference unused parameter
	UNREFERENCED_PARAMETER(RegistryPath);

	// Execute UnloadDriver on driver unload
	DriverObject->DriverUnload = UnloadDriver;

	// Print debug message
	DbgPrintEx(0, 0, "[%s] Driver loaded!", DRIVER_NAME);

	return STATUS_SUCCESS;

}

// Driver unload function
VOID UnloadDriver(IN PDRIVER_OBJECT DriverObject) {

	// Unreference unused parameter
	UNREFERENCED_PARAMETER(DriverObject);

	// Print debug message
	DbgPrintEx(0, 0, "[%s] Driver unloaded!", DRIVER_NAME);

}