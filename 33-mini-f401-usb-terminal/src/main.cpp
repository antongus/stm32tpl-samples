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
#include "usb-channel.h"

HiPrioProcess hiPrioProcess("HiPrioProcess");
DebugTerminalProcess debugTerminalProcess("DebugTerminalProcess");
LedFlasherProcess ledFlasherProcess("LedFlasherProcess");


int main()
{
	UsbChannel::init();
	OS::run();
}

namespace OS
{
	template <>
	OS_PROCESS void HiPrioProcess::exec()
	{
		OS::sleep(1);
		startEvent.signal();
		// loop forever
		for (;;)
		{
			OS::sleep(1000);
		}
	}

	template <>
	OS_PROCESS void LedFlasherProcess::exec()
	{
		// initialize LED pin
		LedPin::Off();
		LedPin::Mode(OUTPUT);

		// loop forever
		for (;;)
		{
			LedPin::On();
			OS::sleep(100);
			LedPin::Off();
			OS::sleep(900);
		}
	}

} // namespace OS
