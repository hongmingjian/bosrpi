# Boot over Serial for Raspberry PI. 

This is a small program to boot RPI over serial.

After loaded by SoC to 0x8000, it relocates itself to 0x1000 and waits to receive uuencoded kernel.img over serial/UART/COM using Xmodem protocol. It will write received data into 0x8000 and decode uuencoded kernel.img in-place and finally transfer the control to 0x8000.
