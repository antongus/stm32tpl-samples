/**
*  @file usb-channel.h
*
*  USB communication channel class
*
*  Copyright © 2016 Anton B. Gusev aka AHTOXA
**/

#pragma once

#include "scmRTOS.h"
#include "cdcacm.h"
#include "textstream.h"

class UsbChannel: public TextStream
{
public:
	static void init();
	static bool isConfigured() { return CdcAcmIsConfigured(); }

	void PutChar(char ch) override;
	int GetChar(int timeout = 0) override;
	int Keypressed() override { return rxChannel.get_count(); }
	int CanSend() override { return txChannel.get_free_size(); }
	int TxEmpty() override { return txChannel.get_count() == 0; }
	void SendBuffer(const void* buf, size_t size) override;
	bool ReceiveBuffer(void* buf, size_t count, int timeout) override;

	void acceptData(uint8_t * buf, size_t size);
	unsigned freeSize() { return txChannel.get_free_size(); }
private:
	static constexpr unsigned txChannelSize = 256;
	static constexpr unsigned rxChannelSize = 256;

	// allow some functions access to private members
	friend void ::CdcAcmDataReceived(uint8_t * buf, size_t size);
	friend void ::CdcAcmSofReceived();

	OS::channel<char, txChannelSize, uint32_t> txChannel;
	OS::channel<char, rxChannelSize, uint32_t> rxChannel;

	static size_t const txSize = 48;
	char txBuf[txSize];

	void flushTx();
	void sendData(const void* buffer, size_t size);
};

extern UsbChannel usbChannel;
