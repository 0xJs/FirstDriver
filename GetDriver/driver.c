#include <ntddk.h>

#define DRIVER_NAME "GetDriver"
#define DRIVER_DEVICENAME L"\\Device\\GetDriver"
#define DRIVER_SYMLINK L"\\??\\GetDriver"

// Define IOCTL code that matches the drivers definition
#define IOCTL_SEND_DATA CTL_CODE(0x800, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)

VOID UnloadDriver(IN PDRIVER_OBJECT DriverObject);
NTSTATUS DriverCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DriverRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath) {

	NTSTATUS NTstatus = STATUS_SUCCESS;
	PDEVICE_OBJECT pDeviceObject;

	// Unreference unused parameter
	UNREFERENCED_PARAMETER(RegistryPath);

	// Execute UnloadDriver on driver unload
	DriverObject->DriverUnload = UnloadDriver;

	// Assign major functions for handling device interactions
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverCreateClose;
	DriverObject->MajorFunction[IRP_MJ_READ] = DriverRead;

	// Define the device name
	UNICODE_STRING  usDevName = RTL_CONSTANT_STRING(DRIVER_DEVICENAME);

	// Create device object
	// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iocreatedevice
	NTstatus = IoCreateDevice(
		DriverObject,					// Pointer to the driver object for the caller.
		0,								// Can be null
		&usDevName,						// Name of device object
		FILE_DEVICE_UNKNOWN,			// Specify UNKNOWN device type
		FILE_DEVICE_SECURE_OPEN,		// Most drivers specify FILE_DEVICE_SECURE_OPEN for this parameter.
		FALSE,							// Most drivers set this value to FALSE.
		&pDeviceObject					// Output pointer to device object
	);
	if (!NT_SUCCESS(NTstatus)) {
		DbgPrintEx(0, 0, "[%s] Driver failed to create device (0x%08X)", DRIVER_NAME, NTstatus);
		return NTstatus;
	}

	// Set the device flags to use buffered I/O
	pDeviceObject->Flags |= DO_BUFFERED_IO;

	// Create a symbolic link for user-mode applications to access the device
	// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iocreatesymboliclink
	UNICODE_STRING usSymLink = RTL_CONSTANT_STRING(DRIVER_SYMLINK);
	NTstatus = IoCreateSymbolicLink(
		&usSymLink, // Pointer to a buffered Unicode string that is the user-visible name.
		&usDevName	// Pointer to a buffered Unicode string that is the name of the driver-created device object.
	);
	if (!NT_SUCCESS(NTstatus)) {
		DbgPrintEx(0, 0, "[%s] Driver failed to create symbolic link (0x%08X)", DRIVER_NAME, NTstatus);

		// Cleanup device link
		// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iodeletedevice
		IoDeleteDevice(pDeviceObject);

		return NTstatus;
	}

	// Print debug message that the driver is loaded
	DbgPrintEx(0, 0, "[%s] Driver loaded!", DRIVER_NAME);

	return STATUS_SUCCESS;

}

// Driver unload function
VOID UnloadDriver(IN PDRIVER_OBJECT DriverObject) {

	// Define the symbolic link name
	UNICODE_STRING usSymLink = RTL_CONSTANT_STRING(DRIVER_SYMLINK);

	// Delete the symbolic link
	// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iodeletesymboliclink
	IoDeleteSymbolicLink(&usSymLink);

	// Delete the device object
	// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iodeletedevice
	IoDeleteDevice(DriverObject->DeviceObject);

	// Print debug message
	DbgPrintEx(0, 0, "[%s] Driver unloaded!", DRIVER_NAME);
}

// Handles IRP_MJ_CREATE and IRP_MJ_CLOSE requests from user-mode applications
NTSTATUS DriverCreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp) {

	// Unreference unused parameter
	UNREFERENCED_PARAMETER(DeviceObject);

	// Set IRP status to success
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	// Complete the request and return success
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

// Handles IRP_MJ_READ requests to provide data to user-mode applications
NTSTATUS DriverRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp) {

	// Unreference unused parameter
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS			NTstatus		= STATUS_SUCCESS;
	PIO_STACK_LOCATION	pStackLocation	= NULL;

	// Get pointer to the caller's I/O stack location in the specified IRP.
	// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iogetcurrentirpstacklocation
	pStackLocation = IoGetCurrentIrpStackLocation(Irp);
	ULONG ulBufferSize = pStackLocation->Parameters.Read.Length;

	// Prepare data to send to the client
	LPCSTR pMessage = "Hello message from Driver";
	SIZE_T stMessageLength = strlen(pMessage) + 1; // include the null terminator

	DbgPrintEx(0, 0, "[%s] Sending message to client console", DRIVER_NAME);

	// Check if buffer is large enough to hold the message
	if (ulBufferSize < stMessageLength) {
		NTstatus = STATUS_BUFFER_TOO_SMALL;
		DbgPrintEx(0, 0, "[%s] Buffer to small", DRIVER_NAME);
		DbgPrintEx(0, 0, "[%s] ulBufferSize = %d", DRIVER_NAME, ulBufferSize);
		DbgPrintEx(0, 0, "[%s] stMessageLength = %zu", DRIVER_NAME, stMessageLength);
	}
	else {
		// Copy the message into Systembuffer to be send to user-mode
		RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, pMessage, stMessageLength);
		Irp->IoStatus.Information = stMessageLength;
		DbgPrintEx(0, 0, "[%s] Send message to client console", DRIVER_NAME);
	}

	// Set IRP status and complete the request
	Irp->IoStatus.Status = NTstatus;

	// https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iocompleterequest
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return NTstatus;
}