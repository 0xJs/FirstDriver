#include "helpers.h"

// Define IOCTL code that matches the drivers definition
#define IOCTL_SEND_DATA CTL_CODE(0x800, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define BUFFER_SIZE 256

int main() {

	HANDLE 	hFile			= NULL;	// Handle to file
	LPSTR	pBuffer			= NULL;	// Pointer to buffer which holds the string
	DWORD 	dwBytesRead		= 0;	// Number of bytes read

	// Open a file handle to the driver using its symbolic link
	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
	hFile = CreateFileW(
		L"\\\\.\\GetDriver",		// Symbolic link to communicate with driver
		GENERIC_READ,				// Read access
		0,							// No sharing
		NULL,						// Default security attributes
		OPEN_EXISTING,				// Open the existing device
		0,							// No special attributes
		NULL						// No template file
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		errorWin32("CreateFile - Failed to open the device");
		return EXIT_FAILURE;
	}
	okay("CreateFileW - Opened handle to file 0x%p", hFile);

	// Allocate memory for the buffer
	pBuffer = malloc(BUFFER_SIZE);
	if (!pBuffer) {
		error("malloc - Failed to allocate memory");
	}
	okay("malloc - Allocated %d bytes of memory at 0x%p", BUFFER_SIZE, pBuffer);

	// Read data from the driver
	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
	if (!ReadFile(
		hFile,				// Handle to file
		pBuffer,			// Pointer to buffer
		BUFFER_SIZE,		// Bytes to read
		&dwBytesRead,		// Number of bytes read
		NULL				// Can be NULL
	)){
		errorWin32("ReadFile - Failed to read from driver");
		return EXIT_FAILURE;
	}
	okay("ReadFile - Read %d bytes from driver", dwBytesRead);

	// Print the received data
	// Confirm successful data transmission
	info("String: %s", pBuffer);

	// Close handle to file
	if (hFile) {
		CloseHandle(hFile);
	}

	return EXIT_SUCCESS;

}