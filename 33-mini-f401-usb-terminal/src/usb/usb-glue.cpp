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

void cdcAcmDataReceived(uint8_t * buf, size_t size)
{
	usbChannel.acceptData(buf, size);
}

void cdcAcmSofReceived()
{
	usbChannel.flushTx();
}

extern "C" void OTG_FS_IRQHandler()
{
	OS::TISRW isrw;
	cdcAcmPoll();
}

void cdcResetEvent()
{
}

void cdcSuspendEvent()
{
}

void cdcResumeEvent()
{
}

void fillSerialNumber(char* buf)
{
	TextBuffer<9> textBuf;
	textBuf.PutHex(0x12345678);
	strncpy(buf, textBuf, 8);
}
