# Boot over Serial for Raspberry Pi. 

## What's this?
After loaded by SoC to 0x8000, this program relocates itself to 0x3000 and waits to receive **uuencoded** files over serial/UART/COM using **Xmodem** protocol. It will write received data to 0x8000 and decode uuencoded data **in-place**.

If a received file has the name of (in the first line of uuencoded data)
- kernel.img: It will transfer the control to 0x8000 to **boot** the uudecoded kernel.img.
-  otherwise: It will **save** uudecoded file to the FAT file system on the SD card and wait to receive the next one.

## How to use?
- Download [kernel.img](https://github.com/hongmingjian/bosrpi/releases/download/v1.1/kernel.img) and copy it to the SD card. Or you can compile it from the source which is self-contained.
- Turnon your RPi.
- Uuencode your kernel.img by issuing  
          `uuencode kernel.img kernel.img >kernel.img.uue`  
  and send the file kernel.img.uue by Xmodem using minicom/screen/ExtraPutty/etc to **boot** RPi from kernel.img.
- Uuencode other.bin by issuing  
           `uuencode other.bin other.bin >other.bin.uue`  
  and send the file other.bin.uue by Xmodem using minicom/screen/ExtraPutty/etc to **save** other.bin to the FAT file system on the SD card.

## Notes
- The UART is on GPIO pin 14(TxD) and 15(RxD) and is initialized to 115200 8N1.
- This bootloader preserves {r0-r3} from start.elf. And it uses a stack, which is in 0x8000 and grows downward.
- I have tested this program on RPi 1B, 1B+(V1.2), 2B(V1.1) and 3B+.
- The file name in the first line of uuencoded data cannot have more than 15 characters.
