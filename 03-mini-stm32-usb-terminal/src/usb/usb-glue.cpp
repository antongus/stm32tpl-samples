/**
*  @file usb-glue.cpp
*
*  USB channel glue functions
*  This file contains functions to glue USB channel
*  and libopencm3 USB CDC code.
*
*  Copyright © 2016 Anton B. Gusev aka AHTOXA
**/

#include "usb-channel.h"
#include "stm32.h"
#include "textbuf.h"
#include <cstring>
#include "pin.h"

void CdcAcmDataReceived(uint8_t * buf, size_t size)
{
	usbChannel.acceptData(buf, size);
}

void CdcAcmSofReceived()
{
	usbChannel.flushTx();
}

extern "C" void USB_LP_CAN1_RX0_IRQHandler()
{
	OS::TISRW isrw;
	CdcAcmPoll();
}

void CdcResetEvent()
{
}

void CdcSuspendEvent()
{
}

void CdcResumeEvent()
{
}

void FillSerialNumber(char* buf)
{
	TextBuffer<9> textBuf;
	textBuf.PutHex(0x12345678);
	strncpy(buf, textBuf, 8);
}
