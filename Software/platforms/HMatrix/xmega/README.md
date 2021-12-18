# Hackyprog

xmega based esp32 programmer

## Setup

	sudo apt install gcc-avr
	sudo apt install avr-libc
	git clone https://github.com/abcminiuser/lufa
	git clone this

## Build

	make

## Flash

	dfu-programmer atxmega256a3u flash VirtualSerial.hex
	#or
	avrdude -c avrisp2 -p x256a3u -U flash:w:VirtualSerial.hex
avrdude -c avrisp2 -p x128a4u -U flash:w:VirtualSerial.hex
