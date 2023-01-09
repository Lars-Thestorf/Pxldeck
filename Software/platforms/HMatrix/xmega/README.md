## Setup

	sudo apt install gcc-avr
	sudo apt install avr-libc
	sudo apt install dfu-programmer

## Build

	make

## Bootloader
Required: set fuses: (Enable Watchdog and Brownout Detection)

	avrdude -c avrisp2 -p x128a4u -U fuse1:w:0x00:m
	avrdude -c avrisp2 -p x128a4u -U fuse2:w:0xbd:m
	avrdude -c avrisp2 -p x128a4u -U fuse4:w:0xfa:m
	avrdude -c avrisp2 -p x128a4u -U fuse5:w:0xe8:m
Optional: install bootloader which will make it USB flashable

	avrdude -c avrisp2 -p x128a4u -U flash:w:bootloader.hex:i

## Flash
If bootloader is installed:

	dfu-programmer atxmega128a4u flash LaLeMa.hex
Otherwise:

	avrdude -c avrisp2 -p x128a4u -U flash:w:LaLeMa.hex
