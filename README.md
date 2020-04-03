# Boot over Serial for Raspberry Pi. 

## Why?
If you are developing programs for bare metal RPi. You should be stuck in the following time-consuming loop.

1. Turnoff RPi.
2. Remove the SD card.
3. Plug the SD card to your PC.
4. Copy newly built kernel.img or other files to SD card.
5. Eject the SD card.
6. Insert the SD card back on the RPi.
7. Turnon the RPi.

## What's this?
This small program comes to save your time by booting RPi over serial.

After loaded by SoC to 0x8000, it relocates itself to 0x1000 and waits to receive uuencoded files over serial/UART/COM using Xmodem protocol. 

It will write received data to 0x8000 and decode uuencoded buffer in-place.

If a received file has the name (in the first line of uuencoded buffer)
- kernel.img: It will transfer the control to 0x8000 to boot the kernel.img.
-  otherwise: It will save the file to FAT file system on the SD card and wait to receive the next file.

## How to use?
- Precompiled: Download [kernel.img](https://github.com/hongmingjian/bosrpi/releases/download/v0.1/kernel.img) and copy it to the SD card.
- From source: If you have the toolchain, you can compile it from the source. This program is self-containing.
