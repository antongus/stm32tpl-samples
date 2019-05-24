/**
*  @file usb-channel.cpp
*
*  USB communication channel class implementation
*
*  Copyright © 2016 Anton B. Gusev aka AHTOXA
**/

#include "usb-channel.h"
#include "stm32.h"
#include "pin.h"
#include <algorithm>

UsbChannel usbChannel;

/**
 * Initialize USB channel
 */
void UsbChannel::init()
{
	using UsbDPlus = Pin<'A', 12>;
	UsbDPlus::Mode(OUTPUT);
	UsbDPlus::Off();
	for (volatile int i = 0; i < 500; ++i)
		asm volatile("nop;");
	UsbDPlus::Mode(ANALOGINPUT);

	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0);

	NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	NVIC_EnableIRQ(USBWakeUp_IRQn);
	CdcAcmInit();
}

void UsbChannel::PutChar(char ch)
{
	txChannel.push(ch);
}

int UsbChannel::GetChar(int timeout)
{
	char ch = 0;
	if (rxChannel.pop(ch, timeout))
		return ch;
	return -1;
}

void UsbChannel::SendBuffer(const void* buf, size_t size)
{
	const char* ptr = reinterpret_cast<const char*>(buf);
	txChannel.write(ptr, size);
}

bool UsbChannel::ReceiveBuffer(void* buf, size_t count, int timeout)
{
	char* ptr = reinterpret_cast<char*>(buf);
	while (count)
	{
		uint32_t slice = count > rxChannelSize ? rxChannelSize : count;
		if (!rxChannel.read(ptr, slice, timeout))
			return false;
		count -= slice;
		ptr += slice;
	}
	return true;
}

/**
 * Write data to TX buffer
 * @param buffer - buffer with data
 * @param size - data size
 */
void UsbChannel::sendData(const void* buffer, size_t size)
{
	txChannel.write(static_cast<char const*>(buffer), size);
}

/**
 * Accept data received from USB
 * @param buffer - buffer with data
 * @param size - data size
 */
void UsbChannel::acceptData(uint8_t * buf, size_t size)
{
	auto const freeSize = rxChannel.get_free_size();
	if (size > freeSize)
		size = freeSize;
	rxChannel.write(reinterpret_cast<char *>(buf), size);
}

/**
 * Send data from TX buffer to USB.
 * This function must be called periodically, from USB SOF interrupt.
 */
void UsbChannel::flushTx()
{
	size_t count = txChannel.read_isr(txBuf, txSize);
	if (count)
		CdcAcmSendData(txBuf, count);
}

