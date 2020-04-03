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

After loaded by SoC to 0x8000, it relocates itself to 0x1000 and waits to receive uuencoded files over serial/UART/COM using Xmodem protocol. It will write received data to 0x8000 and decode uuencoded data in-place.

If a received file has the name of (in the first line of uuencoded data)
- kernel.img: It will transfer the control to 0x8000 and boot uudecoded kernel.img.
-  otherwise: It will save uudecoded file to FAT file system on the SD card and wait to receive the next one.

## How to use?
- Download [kernel.img](https://github.com/hongmingjian/bosrpi/releases/download/v0.1/kernel.img) and copy it to the SD card. Or you can compile it from the source which is self-containing.
- Turnon your RPi.
- Uuencode your kernel.img by issuing `uuencode kernel.img kernel.img >kernel.uue`and send the file kernel.uue by Xmodem using minicom/screen/ExtraPutty/etc.

## Misc.
- The UART is initialized to 115200 8N1.
- Only tested on RPi 1B and 1B+.
- The file name in the first line of uuencoded data cannot have more than 15 characters.

