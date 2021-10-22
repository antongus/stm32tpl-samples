/**
*  @file TcpServer.h
*
*  TCP server.
*
*  Copyright © 2020 Anton B. Gusev
**/

#pragma once

#include "scmRTOS.h"
#include "textstream.h"

class TcpServer : public TextStream
{
public:
	void init();
	OS_PROCESS void loop();
	bool isConnected() const;

	static constexpr uint16_t portToListen { 49152 };
	static constexpr auto debugLog { true };

	/// TextStream overrides
	virtual void PutChar(char ch) override;
	virtual int GetChar(int timeout = 0) override;
	virtual int Keypressed() override { return rxChannel.get_count(); }
	virtual int CanSend() override { return txChannel.get_free_size(); }
	virtual int TxEmpty() override { return txChannel.get_count() == 0; }
	virtual void SendBuffer(void const* buf, size_t size) override;
	virtual bool ReceiveBuffer(void* buf, size_t count, int timeout) override;

private:
	enum class State
	{
		InitHw,
		CreateSocket,
		Socket,
		Listen,
		Close,
		Data,
		Error,
	};

	State state { State::InitHw };

	static constexpr size_t tempBufSize { 64 };
	uint8_t tempBuf[tempBufSize];

	static constexpr size_t rxChannelSize { 256 };
	static constexpr size_t txChannelSize { 256 };

	OS::channel<uint8_t, rxChannelSize, uint32_t> rxChannel;
	OS::channel<uint8_t, txChannelSize, uint32_t> txChannel;

	void generateMacAddress();
};

extern TcpServer tcpServer;


