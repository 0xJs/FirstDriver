# FirstDriver
Simple driver projects, posted to GitHub to share pieces of code within my CheatSheet.

#### Examples
- SendDriver - Receives data from sendclient
  - [IoCreateDevice](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iocreatedevice) - Create device object
  - [IoCreateSymbolicLink](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iocreatesymboliclink) - Create symbolic link for the client to communicate with
  - [IoGetCurrentIrpStackLocation](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iogetcurrentirpstacklocation) - Get a pointer to I/O stack location
  - Read the message from `Irp->AssociatedIrp.SystemBuffer`
  - [IofCompleteRequest](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iocompleterequest) - Complete I/O operations
- SendClient - Sends data to driver
  - [CreateFileW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew) - Open a handle to the driver using symbolic link
  - [DeviceIoControl](https://learn.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-deviceiocontrol) - Send control code to the driver
- GetDriver - Send data to getclient
  - [IoCreateDevice](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iocreatedevice) - Create device object
  - [IoCreateSymbolicLink](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iocreatesymboliclink) - Create symbolic link for the client to communicate with
  - [IoGetCurrentIrpStackLocation](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iogetcurrentirpstacklocation) - Get a pointer to I/O stack location
  - [RtlCopyMemory](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-rtlcopymemory) - Copy the message into Systembuffer to be send to user-mode
  - [IofCompleteRequest](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdm/nf-wdm-iocompleterequest) - Complete I/O operations
- GetClient - Receives data from driver
  - [CreateFileW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew) - Open a handle to the driver using symbolic link
  - [ReadFile](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile) - Read data from the driver
