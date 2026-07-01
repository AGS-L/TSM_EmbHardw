# Setup - Windows (WSL)

Follow the same steps as for Linux.

Additionally, when loading the bitstream or binary, follow these steps:

### Passing USB to WSL

In normal circumstances, your GECKO5Education will not show up in the WSL environment. To fix that, follow these steps:

- Download and install *uspipd-win* https://learn.microsoft.com/en-us/windows/WSL/connect-usb.
- Reboot once finished.
- Plug the GECKO5Education into your computer.
- Open a WSL shell.
- Open a *PowerShell* console as administrator and execute:
  ```sh
  usbpid list
  ```
- In the output, look for the BUSID for the device:
  *0403:6010 USB Serial Converter A, USB Serial Converter B*
- Execute the following in the *PowerShell* console (where you replace `<BUSID>` by the info found in the previous step):
  ```sh
  usbipd bind --force --busid <BUSID>
  usbipd attach --wsl --busid <BUSID>
  ```
- In your WSL shell you should now be able to see the GECKO5Education by executing:
  ```
  lsusb
  ```

**Important**:

- If you close the *PowerShell* console, the connection to WSL will be interrupted.
- Each time you disconnect your GECKO5Education from your computer, you have to redo the steps above except for the first two (i.e. you don't have to reinstall *uspipd-win*, but you will have to bind with WSL).
