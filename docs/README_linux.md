# Setup - Linux

## CAD Tools

**Note**: This installation guide was written using Ubuntu 24.04 LTS, but should also work on other distributions.

- Download the *OSS-CAD-suite* (https://github.com/YosysHQ/oss-cad-suite-build/releases)
- Extract the contents of the *OSS-CAD-suite* with:
  ```sh
  tar -xf oss-cad-suite-linux-x64-<date>.tgz
  ```
- Move the tools to your `/opt/` directory by using:
  ```sh
  sudo mv oss-cad-suite /opt/
  ```
- Add the *OSS-CAD-suite* to the PATH variable by opening your `.bashrc` file with your favorite editor and add the following at the end:
  ```sh
  export PATH=/opt/oss-cad-suite/bin:$PATH
  ```

## Additional Configurations

- To be able to use the UART(RS232), you need to be in the dialout-group. You can add yourself to this group by:
  ```sh
  sudo usermod -a -G dialout <your user name>
  ```
  **Note**: For this command to take effect, you have to log out and log in again.
- Normally you do not have access to the USB-devices of the GECKO5Education as normal user. To be able to use these devices, you can use following command:
  ```sh
  sudo cp /opt/oss-cad-suite/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d
  ```
  **Note**: For this rule to take effect, you have to unplug & reinsert your GECKO5Education from your computer.

## Compiler

**Warning**: Compiling the cross-compiler takes several hours.

- Open a terminal.
- Download the required packages with the following command (this is for Ubuntu 26.04 LTS):
  ```sh
  sudo apt install gcc build-essential git rsync wget texinfo guile-3.0
  ```
  For other Linux distributions, please look up the equivalent packages.
- Clone the toolchain from GitHub by executing the following:
  ```sh
  git clone https://github.com/EPFL-LAP/or1kToolchain.git
  ```
- Download more by executing this list of commands:
  ```sh
  cd or1kToolchain/gnu_src
  wget ftp.gnu.org/gnu/binutils/binutils-2.45.tar.bz2
  wget mirror.koddos.net/gcc/releases/gcc-15.2.0/gcc-15.2.0.tar.gz
  git clone git://sourceware.org/git/cgen.git
  ```
- Build the toolchain by:
  ```sh
  cd ../patch
  ./compile_linux.sh
  ```
- Add the compiler to the PATH variable by opening your `.bashrc` file with your favorite editor and add the following at the end:
  ```sh
  export PATH=/opt/or1k_toolchain/bin:$PATH
  ```
- Check if the compiler has been installed correctly by executing `or1k-elf-gcc -version`. The output should be:
  ```
  or1k-elf-gcc (GCC) 15.2.0
  Copyright (C) 2025 Free Software Foundation, Inc.
  This is free software; see the source for copying conditions.  There is NO
  warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  ```
  If that isn't the output you see, restart the terminal and check again.
- Compile and install the *convert_or32* utility:
  ```sh
  cd ../convert_or32
  gcc -O2 -o convert_or32 read_elf.c convert_or32.c
  sudo mv convert_or32 /opt/or1k_toolchain/bin/
  ```

## Camera Viewer

To view the output, you will require camera viewing software, unless you have a screen to hook up the HDMI output to. A free solution is *OBS* (https://obsproject.com/):

- Download & open *OBS*
- In the *Sources* tab, press the + button and select *Video Capture Device*
- In the pop-up window (*Properties for 'Video Capture Device'*), select *SandbergCapture*
    - The *HDMI Capture Link to USB* must be connected to your computer.
    - If Video doesn't update, you can deactive & reactive the capture device in the *Properties* window.

Alternatively, you can use *Webcamoid* (https://github.com/webcamoid/webcamoid/releases), though if you want to download the executables you need to pay or compile it yourself.

## Serial Terminal

In order to load the program (binary) on the board, you will need to send it over UART to the softcore. You require a program capable of sending files over a serial port.
One such program is *cutecom* (https://gitlab.com/cutecom/cutecom/). On the official website you can download the sources; however, most Linux distributions already have *cutecom* packaged. Under Ubuntu 24.04 LTS it can be installed with:

```sh
sudo apt install cutecom
```

An alternative is *CoolTerm* (https://freeware.the-meiers.org/).

These are the required UART settings:

- Baud rate: 115200
- Data Bits: 8
- Flow Control: None
- Parity: None
- Stop Bits: 1

## Hardware

- Connect the OV7670 camera to the board:

  ![](images/camera.png)
<!--  -->
- Connect the HDMI cable to the board.
- Connect the other end of the HDMI cable to either the *HDMI Capture Link to USB* or a screen.
- Connect the USB cable to the board, then your computer.

# Usage Instructions - Linux

## Bitstream

### Loading

If you wish to load the provided bitstream [or1300SingleCore.bit](../bit/or1300SingleCore.bit), run the following:
```sh
openFPGALoader or1300SingleCore.bit
```

### Generating

If you wish to modify the softcore and generate a new bitstream, run the [`synthesizeOr1300.sh`](../soc/or1300/synthesizeOr1300.sh) script.

**Note**: If you decide to read through the Verilog on your own, keep in mind that the Verilog macro `GECKO5Education` is defined - it's passed in `synthesizeOr1300.sh`.

## Binary (Executable)

### Loading

**Warning**: when transfering a binary to the softcore, the softcore must be in its reset state (press the *Reset* button on the board). The board will print the following if it has been reset:

```
CS-473 System programming for systems on chip
Openrisc based virtual Prototype.
Build version: Thu Aug 28 06:56:14 AM CEST 2025

I am CPU 1 of 1 running at 59.40 MHz.

Known RS232 commands:
** Start the program loaded in target
*p Set programming mode (default)
*v Set verification mode
*i Show info on program in target
*t Toggle target between SDRam (default), soft-Bios and Flash
*m Perform simple SDRam memcheck
*s Check SPI-flash chip
*e Erase SPI-Flash chip
*f Store program loaded in SDRAM to SPI-Flash
*c Compare program loaded in SDRAM with SPI-Flash
*r Run program stored in SPI-Flash
*q Toggle stack pointer from SDRAM (default) with SPM
*h This helpscreen
```

(*Virtual Prototype* is the term EPFL uses for their softcore.)

To send the binary, follow these steps:

- Connect the board.
- Open *cutecom* (or whatever serial port software you use).
- Send [camera.cmem](../bin/camera.cmem).
- The terminal will not send anything back during transfer, but you will see the transfer progress on the screen output of the board.
- Once the transfer has completed, it will print `Download: done`.
- To start your program, send `**`.
- If you want to load a new program, remember that you need to reset the softcore (not the whole board, just the softcore).

### Generating

A base binary is provided, so (re)compile the binary only if you want to change the binary. If that is the case, run the [`Makefile`](../soc/camera-program/Makefile) by executing `make`. Rerun this command every time you want to recompile the binary.
