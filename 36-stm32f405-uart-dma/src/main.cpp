/**
*  @file main.cpp
*
*  Main project file.
*
*  Copyright © 2026 Anton B. Gusev
**/

#include "hw.h"
#include "scmRTOS.h"
#include "commands.h"
#include "vars.h"

using UartTerminalProcess   = OS::process<OS::pr0, 1200>;
using LedFlasherProcess     = OS::process<OS::pr1, 1200>;

// use new cool scmRTOS feature - process initialization with lambdas
UartTerminalProcess uartTerminalProcess("UartProcess", []{
	OS::sleep(1);

	char buf[256];
	for (;;)
	{
		uart << "=>";
		uart.Gets(buf, 255);
		if (*buf)
		{
			if (!Interpreter::parse(buf, uart))
				uart << "ERR";
			uart << "\r\n";
		}
	}
});

LedFlasherProcess ledFlasherProcess("LedFlasher", []{
	GreenLED::Off();
	GreenLED::Mode(OUTPUT);

	for (;;)
	{
		GreenLED::On();
		OS::sleep(50);
		GreenLED::Off();
		OS::sleep(950);
	}
});

int main()
{
	OS::run();
}
