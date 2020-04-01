# Boot over serial for Raspberry PI. 

This is a small program to boot RPI over serial.

After loaded by SoC to 0x8000, it relocates itself to 0x1000 and waits to receive uuencoded kernel.img to 0x8000 over serial. Then it will decode uuencoded kernel.img in-place and finally transfer the control to 0x8000.
