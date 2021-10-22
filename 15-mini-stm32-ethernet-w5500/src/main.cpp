/**
*  @file main.cpp
*
*  Main project file.
*
*  Copyright © 2020 Anton B. Gusev
**/

#include "scmRTOS.h"
#include "hw.h"
#include "processes.h"
//#include "usb-channel.h"
#include "TcpServer.h"

EthernetServerProcess ethernetServerProcess("LedMatrixProcess");
DebugTerminalProcess debugTerminalProcess("DebugTerminalProcess");
LedFlasherProcess ledFlasherProcess("LedFlasherProcess");


int main()
{
	// initialize LED pin
	Board::LedPin::Off();
	Board::LedPin::Mode(OUTPUT);

	// initialize Ethernet pins
	Board::EthPins::init();

	// initialize USB stack
//	UsbChannel::init();

	// run OS
	OS::run();
}

namespace OS
{
	template <>
	OS_PROCESS void EthernetServerProcess::exec()
	{
		OS::sleep(1);
		startEvent.signal();

		tcpServer.init();
		tcpServer.loop();
	}

	template <>
	OS_PROCESS void LedFlasherProcess::exec()
	{
		// loop forever
		for (;;)
		{
			Board::LedPin::On();
			OS::sleep(100);
			Board::LedPin::Off();
			OS::sleep(900);
		}
	}

} // namespace OS
