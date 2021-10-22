/**
*  @file TcpServer.cpp
*
*  TCP server.
*
*  Copyright © 2020 Anton B. Gusev
**/

#include "socket.h"
#include "wizchip_conf.h"
#include "TcpServer.h"
#include "hw.h"
#include "stm32_crc.h"
#include <cstring>


TcpServer tcpServer;

static wiz_NetInfo network_info  =
{
	{0x00, 0x08, 0xdc, 0xFF, 0xFF, 0xFF},   // MAC address
	{192, 168, 1, 201},                     // IP address
	{255, 255, 255, 0},                     // Subnet mask
	{192, 168, 0, 1},                       // Gateway address
};

extern "C" void eth_cs_sel()
{
	for (auto i = 0U; i < 2; ++i)
		__asm__ __volatile__ ("nop");
	Board::EthPins::CS::On();
}

extern "C" void eth_cs_desel()
{
	for (auto i = 0U; i < 10; ++i)
		__asm__ __volatile__ ("nop");
	Board::EthPins::CS::Off();
}

extern "C" uint8_t eth_spi_rb()
{
	return spiEthernet.Rw(0);
}

extern "C" void eth_spi_wb(uint8_t b)
{
	spiEthernet.Rw(b);
}

void TcpServer::init()
{
	state = State::InitHw;
	OS::sleep(10);
	Board::EthPins::Rst::On();
	OS::sleep(10);
	Board::EthPins::Rst::Off();
	OS::sleep(100);

	if (debugLog)
		uart << "Initializing server...\r\n";

	generateMacAddress();

	reg_wizchip_cs_cbfunc(eth_cs_sel, eth_cs_desel);
	reg_wizchip_spi_cbfunc(eth_spi_rb, eth_spi_wb);
	wizphy_reset();

	uint8_t  bufSizes[] = {8, 8};
	wizchip_init(bufSizes, bufSizes);

	if (debugLog)
	{
		uart << "MAC addr: ";
		for (auto i = 0; i < 6; ++i)
		{
			uart.PutHex(network_info.mac[i]);
			if (i < 5)
				uart << ':';
			else
				uart << "\r\n";
		}
		uart << "IP addr: "
				<< network_info.ip[0] << '.' << network_info.ip[1] << '.'
				<< network_info.ip[2] << '.' << network_info.ip[3] << "\r\n";
		uart << "Net mask: "
				<< network_info.sn[0] << '.' << network_info.sn[1] << '.'
				<< network_info.sn[2] << '.' << network_info.sn[3] << "\r\n";
		uart << "Gateway: "
				<< network_info.gw[0] << '.' << network_info.gw[1] << '.'
				<< network_info.gw[2] << '.' << network_info.gw[3] << "\r\n";
	}

	wizchip_setnetinfo(&network_info);
	state = State::CreateSocket;
}

void TcpServer::loop()
{
	for (;;)
	{
		OS::sleep(1);

		switch (state)
		{
		case State::InitHw: // do nothing (hardware initialization in progress)
			break;

		case State::CreateSocket:
			if (debugLog)
				uart << "Creating socket... ";
			if(auto ret = socket(0, Sn_MR_TCP, portToListen, 0); ret == 0)
			{
				state = State::Socket;
				if (debugLog)
					uart << "OK!\r\n";
			}
			else
			{
				state = State::Error;
				if (debugLog)
					uart << "ERROR!\r\n";
			}
			break;

		case State::Socket:
			if (debugLog)
				uart << "Starting listen... ";
			if (auto ret = listen(0); ret == SOCK_OK)
			{
				state = State::Listen;
				if (debugLog)
					uart << "OK!\r\n";
			}
			else if (ret == SOCKERR_SOCKCLOSED)
			{
				state = State::Close;
				if (debugLog)
					uart << "ERROR!\r\n";
			}
			break;

		case State::Listen:
			if (auto ret = getSn_SR(0); ret == SOCK_LISTEN) // read status for socket
			{
				OS::sleep(100);
			}
			else if (ret == SOCK_ESTABLISHED)
			{
				uint8_t addr[7];
				getsockopt(0, SO_DESTIP, addr);
				state = State::Data;

				if (debugLog)
					uart << "client connected, IP: "
						<< addr[0] << "."
						<< addr[1] << "."
						<< addr[2] << "."
						<< addr[3] << "\r\n";
				break;
			}
			else
			{
				state = State::Close;
			}
			break;

		case State::Close:
			if (debugLog)
				uart << "Closing socket\r\n";
			disconnect(0);
			close(0);
			rxChannel.flush();
			txChannel.flush();
			state = State::CreateSocket;
			break;

		case State::Data:
			if (getSn_SR(0) == SOCK_ESTABLISHED)
			{
				auto size = getSn_RX_RSR(0);
				if (size != 0) // data received
				{
					if (size > tempBufSize)
						size = tempBufSize;

					auto rxCount = recv(0, tempBuf, size);
					if (rxCount <= 0)
					{
						if (debugLog)
							uart << "Read error\r\n";
						state = State::Close;
						break;
					}

					auto freeRxSize = static_cast<int>(rxChannel.get_free_size());
					if (rxCount > freeRxSize)
					{
						if (debugLog)
							uart << "RX buffer overflow!\r\n";
						rxCount = freeRxSize;
					}
					rxChannel.write(tempBuf, rxCount);
				}

				auto txCount = txChannel.get_count();
				if (txCount)
				{
					if (txCount > tempBufSize)
						txCount = tempBufSize;
					txChannel.read(tempBuf, txCount, 1);
					auto bytesSent = send(0, tempBuf, txCount);
					if (static_cast<int>(txCount) != bytesSent)
					{
						if (debugLog)
							uart << "Write error\r\n";
						state = State::Close;
						break;
					}
				}
			}
			else
			{
				if (debugLog)
					uart << "Client disconnected\r\n";
				state = State::Close;
			}
			break;

		case State::Error: // something goes wrong
			break;

		}
	}
}

bool TcpServer::isConnected() const
{
	return state == State::Data;
}

void TcpServer::PutChar(char ch)
{
	txChannel.push(ch);
}

int TcpServer::GetChar(int timeout)
{
	uint8_t ch { 0 };
	if (rxChannel.pop(ch, timeout))
		return static_cast<int>(ch);
	return -1;
}

void TcpServer::SendBuffer(void const* buf, size_t size)
{
	auto ptr = reinterpret_cast<uint8_t const*>(buf);
	txChannel.write(ptr, size);
}

bool TcpServer::ReceiveBuffer(void* buf, size_t count, int timeout)
{
	auto ptr = reinterpret_cast<uint8_t*>(buf);
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
 * Generate MAC-address using DeviceID
 * First two bytes are fixed, next 4 bytes == crc32(DeviceId).
 */
void TcpServer::generateMacAddress()
{
	network_info.mac[0] = 0x02;  // locally administered unicast address
	network_info.mac[1] = 0x1A;  // just a "random" value :-)

	auto crc = STM32::CrcCalculator::CalcEthernet(reinterpret_cast<void const*>(chip::DEVICE_ID_ADDR), 12);
	memcpy(&network_info.mac[2], &crc, 4);
}
