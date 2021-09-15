/*
             LUFA Library
     Copyright (C) Dean Camera, 2020.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2020  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the VirtualSerial demo. This file contains the main tasks of the demo and
 *  is responsible for the initial application hardware configuration.
 */

#include "VirtualSerial.h"
#include "setup.h"
#include <stdbool.h>

#include "swRingbuffer.h"

/** Contains the current baud rate and other settings of the virtual serial port. While this demo does not use
 *  the physical USART and thus does not use these settings, they must still be retained and returned to the host
 *  upon request or the host will assume the device is non-functional.
 *
 *  These values are set by the host via a class-specific request, however they are not required to be used accurately.
 *  It is possible to completely ignore these value or use other settings as the host is completely unaware of the physical
 *  serial link characteristics and instead sends and receives data in endpoint streams.
 */
static CDC_LineEncoding_t LineEncoding = { .BaudRateBPS = 0,
                                           .CharFormat  = CDC_LINEENCODING_OneStopBit,
                                           .ParityType  = CDC_PARITY_None,
                                           .DataBits    = 8                            };

#define ENDPOINT_RX_UART_TX_BYTES 128

uint8_t uart_rx_ring_buffer[128];
uint8_t uart_rx_ring_read_pos = 0;

uint8_t uart_tx_buffer[4][ENDPOINT_RX_UART_TX_BYTES];
uint8_t uart_tx_buffer_trfcnt[4];
volatile uint8_t uart_tx_buffer_dma_pos = 0;
volatile uint8_t uart_tx_buffer_usb_pos = 0;
volatile bool dma_ready = true;

#define INJECT_MESSAGE_BUFFER_SIZE 1024
uint8_t inject_message_usb_buffer[INJECT_MESSAGE_BUFFER_SIZE];
uint8_t inject_message_uart_buffer[INJECT_MESSAGE_BUFFER_SIZE];
bool injecting_allowed = true; //while flashing esp its not allowed to inject other data

sw_ringbuffer_t inject_message_usb_sw_ringbuffer;
sw_ringbuffer_t inject_message_uart_sw_ringbuffer;

void inject_message_usb(char* text)
{
	size_t textlen = strlen(text);
	sw_ringbuffer_write(&inject_message_usb_sw_ringbuffer, (uint8_t*)text, textlen);
}
void inject_data_usb(uint8_t* data, uint16_t len)
{
	sw_ringbuffer_write(&inject_message_usb_sw_ringbuffer, data, len);
}

void inject_message_uart(char* text)
{
	size_t textlen = strlen(text);
	sw_ringbuffer_write(&inject_message_uart_sw_ringbuffer, (uint8_t*)text, textlen);
}
void inject_data_uart(uint8_t* data, uint16_t len)
{
	sw_ringbuffer_write(&inject_message_uart_sw_ringbuffer, data, len);
}

void SetupVirtualSerial(void) {
	/* USB Hardware Initialization */
	USB_Init();
	
	SetupUart();
	SetupAdditionalPins();

	GlobalInterruptEnable();
	
	uart_rx_ring_buffer_init();
	uart_tx_buffer_init();
	
	sw_ringbuffer_init(&inject_message_usb_sw_ringbuffer, inject_message_usb_buffer, INJECT_MESSAGE_BUFFER_SIZE);
	sw_ringbuffer_init(&inject_message_uart_sw_ringbuffer, inject_message_uart_buffer, INJECT_MESSAGE_BUFFER_SIZE);
}

void VirtualSerialWorker(void) {
	CDC_Task();
	USB_USBTask();
}

void uart_rx_ring_buffer_init(void)
{
	DMA.CH2.CTRLA                         = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm | DMA_CH_REPEAT_bm;
	DMA.CH2.CTRLB                         = DMA_CH_TRNINTLVL_OFF_gc | DMA_CH_TRNIF_bm;

	__int24 dst = (__int24)(int16_t)uart_rx_ring_buffer;
	__int24 src = (__int24)(int16_t)&USARTC1.DATA;

	DMA.CH2.REPCNT                        = 0;
	DMA.CH2.TRFCNT                        = (uint16_t) 128;
	DMA.CH2.ADDRCTRL                      = DMA_CH_SRCRELOAD_TRANSACTION_gc | DMA_CH_SRCDIR_FIXED_gc | DMA_CH_DESTDIR_INC_gc | DMA_CH_DESTRELOAD_TRANSACTION_gc;
	DMA.CH2.TRIGSRC                       = DMA_CH_TRIGSRC_USARTC1_RXC_gc;

	DMA.CH2.DESTADDR0                     = (uint8_t) (uint16_t) dst;
	DMA.CH2.DESTADDR1                     = (uint8_t)  *(((uint8_t *)&(dst)) + 1);
	DMA.CH2.DESTADDR2                     = (uint8_t)  *(((uint8_t *)&(dst)) + 2);

	DMA.CH2.SRCADDR0                      = (uint8_t) (uint16_t) src;
	DMA.CH2.SRCADDR1                      = (uint8_t)  *(((uint8_t *)&(src)) + 1);
	DMA.CH2.SRCADDR2                      = (uint8_t)  *(((uint8_t *)&(src)) + 2); 

	DMA.CH2.CTRLA                        |= DMA_CH_ENABLE_bm;
}

void uart_tx_buffer_init(void)
{
	DMA.CTRL = DMA_PRIMODE_CH0123_gc | DMA_ENABLE_bm;

	DMA.CH1.CTRLA                         = DMA_CH_BURSTLEN_1BYTE_gc | DMA_CH_SINGLE_bm;
	DMA.CH1.CTRLB                         = DMA_CH_TRNINTLVL_MED_gc | DMA_CH_TRNIF_bm;

	__int24 dst = (__int24)(int16_t)&USARTC1.DATA;

	DMA.CH1.REPCNT                        = 1;

	DMA.CH1.ADDRCTRL                      = DMA_CH_SRCRELOAD_NONE_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTDIR_FIXED_gc | DMA_CH_DESTRELOAD_NONE_gc;
	DMA.CH1.TRIGSRC                       = DMA_CH_TRIGSRC_USARTC1_DRE_gc;

	DMA.CH1.DESTADDR0                     = (uint8_t) (uint16_t) dst;
	DMA.CH1.DESTADDR1                     = (uint8_t)  *(((uint8_t *)&(dst)) + 1);
	DMA.CH1.DESTADDR2                     = (uint8_t)  *(((uint8_t *)&(dst)) + 2);
	
	dma_ready = true;
}

void uart_tx_dma_transfer_init(void)
{
	__int24 src = (__int24)(uint16_t)&uart_tx_buffer[uart_tx_buffer_dma_pos][0];
	dma_ready = false;
	DMA.CH1.TRFCNT                        = uart_tx_buffer_trfcnt[uart_tx_buffer_dma_pos];

	DMA.CH1.SRCADDR0                      = (uint8_t) (uint16_t) src;
	DMA.CH1.SRCADDR1                      = (uint8_t)  *(((uint8_t *)&(src)) + 1);
	DMA.CH1.SRCADDR2                      = (uint8_t)  *(((uint8_t *)&(src)) + 2); 
	DMA.CH1.CTRLA                        |= DMA_CH_ENABLE_bm;
}
void UartSetBaud(uint32_t baud) {
	uint16_t val = (((uint32_t)F_CPU * 2) + baud*8)/(baud*16) - 1;
	USARTC1.BAUDCTRLA = val & 0xFF;
	USARTC1.BAUDCTRLB = val >> 8;
}
void SetupUart(){
	PORTC.DIRCLR = PIN6_bm;    // pin PC2 (RXC1) as input
	PORTC.DIRSET = PIN7_bm;    // pin PC3 (TXC1) as output
	USARTC1.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc;   // 8N1

	uint32_t baud = 115200;
	UartSetBaud(baud);

	USARTC1.CTRLA = USART_RXCINTLVL_OFF_gc;

	USARTC1.CTRLB |= USART_TXEN_bm | USART_RXEN_bm | USART_CLK2X_bm;   // enable TX+RX and Clock
}
void SetupAdditionalPins(){
	PIN_BOOT_reg.PASTE3(PIN, PIN_BOOT_bit, CTRL) = PORT_OPC_PULLUP_gc;
	PIN_EN_reg.PASTE3(PIN, PIN_EN_bit, CTRL) = PORT_OPC_PULLUP_gc;
	PIN_BOOT_reg.OUTCLR = 1 << PIN_BOOT_bit;
	PIN_EN_reg.OUTCLR = 1 << PIN_EN_bit;
}

/** Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Device_Connect(void)
{
	/* Indicate USB enumerating */
}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
 *  the status LEDs and stops the USB management and CDC management tasks.
 */
void EVENT_USB_Device_Disconnect(void)
{
	/* Indicate USB not ready */
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host set the current configuration
 *  of the USB device after enumeration - the device endpoints are configured and the CDC management task started.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup CDC Data Endpoints */
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_TX_EPADDR, EP_TYPE_BULK, CDC_TXRX_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_RX_EPADDR, EP_TYPE_BULK,  CDC_TXRX_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPADDR, EP_TYPE_INTERRUPT, CDC_NOTIFICATION_EPSIZE, 1);

	/* Reset line encoding baud rate so that the host knows to send new values */
	LineEncoding.BaudRateBPS = 0;

	/* Indicate endpoint configuration success or failure */
}

/** Event handler for the USB_ControlRequest event. This is used to catch and process control requests sent to
 *  the device from the USB host before passing along unhandled control requests to the library for processing
 *  internally.
 */
void EVENT_USB_Device_ControlRequest(void)
{
	/* Process CDC specific control requests */
	switch (USB_ControlRequest.bRequest)
	{
		case CDC_REQ_GetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Write the line coding data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
				Endpoint_ClearOUT();
			}

			break;
		case CDC_REQ_SetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Read the line coding data in from the host into the global struct */
				Endpoint_Read_Control_Stream_LE(&LineEncoding, sizeof(CDC_LineEncoding_t));
				Endpoint_ClearIN();

				uint32_t baud = LineEncoding.BaudRateBPS;
				UartSetBaud(baud);
			}

			break;
		case CDC_REQ_SetControlLineState:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				Endpoint_ClearStatusStage();

				/* NOTE: Here you can read in the line state mask from the host, to get the current state of the output handshake
						lines. The mask is read in from the wValue parameter in USB_ControlRequest, and can be masked against the
						CONTROL_LINE_OUT_* masks to determine the RTS and DTR line states using the following code:
				*/
				bool DTR = (USB_ControlRequest.wValue & CDC_CONTROL_LINE_OUT_DTR) != 0;
				bool RTS = (USB_ControlRequest.wValue & CDC_CONTROL_LINE_OUT_RTS) != 0;

#ifdef DO_DEBUG
				//open serial monitor: 320
				//RTS: ----------_____
				//DTR: -----__________
				//flashing procedure: 32310
				//RTS: ---------------__________
				//DTR: -----_____----------_____
				if (DTR==0 && RTS==0)
					DebugSendChar('0');
				if (DTR==1 && RTS==0)
					DebugSendChar('1');
				if (DTR==0 && RTS==1)
					DebugSendChar('2');
				if (DTR==1 && RTS==1)
					DebugSendChar('3');
#endif
				if (RTS){
					PIN_EN_reg.DIRSET = 1 << PIN_EN_bit; //Reset LOW
				} else {
					PIN_EN_reg.DIRCLR = 1 << PIN_EN_bit; //Reset floating
				}
				if (DTR){
					PIN_BOOT_reg.DIRSET = 1 << PIN_BOOT_bit; //Boot LOW
					injecting_allowed = RTS;
					if (!injecting_allowed){
						sw_ringbuffer_flush(&inject_message_uart_sw_ringbuffer);
						sw_ringbuffer_flush(&inject_message_usb_sw_ringbuffer);
					}
				} else {
					PIN_BOOT_reg.DIRCLR = 1 << PIN_BOOT_bit; //Boot floating
				}
			}
			break;
	}
}

/** Function to manage CDC data transmission and reception to and from the host. */
void CDC_Task(void)
{
	uint8_t next_tx_buffer_usb_pos = ((uart_tx_buffer_usb_pos + 1) & 0b11);
	if(next_tx_buffer_usb_pos != uart_tx_buffer_dma_pos) {
		if (injecting_allowed && sw_ringbuffer_available(&inject_message_uart_sw_ringbuffer)) {
			uint16_t len = sw_ringbuffer_read(&inject_message_uart_sw_ringbuffer, &uart_tx_buffer[uart_tx_buffer_usb_pos][0], ENDPOINT_RX_UART_TX_BYTES);
			uart_tx_buffer_trfcnt[uart_tx_buffer_usb_pos] = len;
			uart_tx_buffer_usb_pos = next_tx_buffer_usb_pos;
		}
		if(dma_ready && uart_tx_buffer_dma_pos != uart_tx_buffer_usb_pos) //Ready will be false after the call...
		{
			uart_tx_dma_transfer_init();
		}
	}
	
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;

	/* If we have a free buffer receive data from endpoint */
	next_tx_buffer_usb_pos = ((uart_tx_buffer_usb_pos + 1) & 0b11);
	if(next_tx_buffer_usb_pos != uart_tx_buffer_dma_pos) //If the next buffer still in work we should not use it for new data.
	{
		Endpoint_SelectEndpoint(CDC_RX_EPADDR);

		/* Check to see if any data has been received */
		if (Endpoint_IsOUTReceived())
		{
			//while(!(USARTC0.STATUS & USART_DREIF_bm));
			//USARTC0.DATA = 'd';
			/* Create a temp buffer big enough to hold the incoming endpoint packet */

			/* Remember how large the incoming packet is */
			uint8_t datalen = Endpoint_BytesInEndpoint();

            //Avoid to get more bytes then buffer size.
			if(datalen > ENDPOINT_RX_UART_TX_BYTES)
				datalen = ENDPOINT_RX_UART_TX_BYTES;

            uart_tx_buffer_trfcnt[uart_tx_buffer_usb_pos] = datalen;

			/* Read in the incoming packet into the buffer */
			Endpoint_Read_Stream_LE(&uart_tx_buffer[uart_tx_buffer_usb_pos][0], datalen, NULL);

			uart_tx_buffer_usb_pos = next_tx_buffer_usb_pos;
			/* Finalize the stream transfer to send the last packet */
			Endpoint_ClearOUT();
		}
	}
	if(dma_ready && uart_tx_buffer_dma_pos != uart_tx_buffer_usb_pos) //Ready will be false after the call...
	{
		uart_tx_dma_transfer_init();
	}

	uint8_t dma_pos = 128 - DMA.CH2.TRFCNTL;
	if(dma_pos != uart_rx_ring_read_pos)
	{
		/* Select the Serial Tx Endpoint */
		Endpoint_SelectEndpoint(CDC_TX_EPADDR);

		uint8_t bytes_to_transfer;
		if(uart_rx_ring_read_pos < dma_pos)
		{
			bytes_to_transfer = dma_pos - uart_rx_ring_read_pos;
		}
		else
		{
			bytes_to_transfer = 128 - uart_rx_ring_read_pos;
		}
		/* Write the received data to the endpoint */
		Endpoint_Write_Stream_LE(uart_rx_ring_buffer + uart_rx_ring_read_pos, bytes_to_transfer, NULL);
		uart_rx_ring_read_pos = (uart_rx_ring_read_pos + bytes_to_transfer) & 0b1111111;

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();

		/* Wait until the endpoint is ready for the next packet */
		Endpoint_WaitUntilReady();

		/* Send an empty packet to prevent host buffering */
		Endpoint_ClearIN();
	}
	else {
		if (injecting_allowed && sw_ringbuffer_available(&inject_message_usb_sw_ringbuffer)) {
			Endpoint_SelectEndpoint(CDC_TX_EPADDR);
			
			sw_ringbuffer_external_job_t read = sw_ringbuffer_read_external(&inject_message_usb_sw_ringbuffer, INJECT_MESSAGE_BUFFER_SIZE);
			Endpoint_Write_Stream_LE(read.start, read.len, NULL);
			if (read.more_data_available){
				read = sw_ringbuffer_read_external(&inject_message_usb_sw_ringbuffer, INJECT_MESSAGE_BUFFER_SIZE);
				Endpoint_Write_Stream_LE(read.start, read.len, NULL);
			}
			
			Endpoint_ClearIN();
			Endpoint_WaitUntilReady();
			Endpoint_ClearIN();
		}
	}
}

//DMA Interrupt
ISR(DMA_CH1_vect)
{
	DMA.CH1.CTRLB |= DMA_CH_TRNIF_bm;

	uart_tx_buffer_dma_pos = (uart_tx_buffer_dma_pos + 1) &0b11;
	if(uart_tx_buffer_dma_pos != uart_tx_buffer_usb_pos)
	{
		uart_tx_dma_transfer_init();
	}
	else
	{
		dma_ready = true;
	}
}
