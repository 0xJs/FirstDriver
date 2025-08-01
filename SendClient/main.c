#include "helpers.h"

// Define IOCTL code that matches the drivers definition
#define IOCTL_SEND_DATA CTL_CODE(0x800, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)

int main() {

	HANDLE 	hDevice				= NULL; // Handle to device
	DWORD 	dwBytesReturned		= NULL;	// Number of bytes read
	BOOL	bSuccess			= NULL; // Bool to store FALSE/TRUE

	// Open a file handle to the driver using its symbolic link
	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
	hDevice = CreateFileW(
		L"\\\\.\\SendDriver",		// Symbolic link to communicate with driver
		GENERIC_WRITE,				// Write access
		FILE_SHARE_WRITE,			// Allow write sharing
		NULL,						// Default security attributes
		OPEN_EXISTING,				// Open the existing device
		0,							// No special attributes
		NULL						// No template file
	);
	if (hDevice == INVALID_HANDLE_VALUE) {
		errorWin32("CreateFile - Failed to open the device");
		return EXIT_FAILURE;
	}
	okay("CreateFileW - Opened file handle 0x%p", hDevice);

	// Data to be send to the driver
	char cData[] = "Hello World!";

	// Sends control code directly to specified device driver
	// https://learn.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-deviceiocontrol
	bSuccess = DeviceIoControl(
		hDevice,			// Handle to the device
		IOCTL_SEND_DATA,	// IOCTL Code that specifies the operation
		cData,				// Pointer to input data
		sizeof(cData),		// Size of data
		NULL,				// Output buffer not required
		0,					// Size of the output buffer
		&dwBytesReturned,	// Output size
		NULL				// Overlapped structure
	);
	if (!bSuccess) {
		errorWin32("DeviceIoControl - Failed to open the device");
		if (hDevice) {
			CloseHandle(hDevice);
			info("CloseHandle - Closed handle to device");
		}
		return EXIT_FAILURE;
	}
	okay("DeviceIoControl - Data send successfully: %s", cData);

	// Close file handle
	if (hDevice) {
		CloseHandle(hDevice);
		info("CloseHandle - Closed handle to device");
	}

	return EXIT_SUCCESS;
}