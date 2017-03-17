/**
*  @file terminal.cpp
*
*  Serial port debug terminal.
*
*  Copyright © 2016 Anton B. Gusev aka AHTOXA
**/

#include "hw.h"
#include "commands.h"
#include "vars.h"
#include "util.h"

using DebugTerminalProcess  = OS::process<OS::pr1, 1200>;

namespace OS
{
	template <>
	OS_PROCESS void DebugTerminalProcess::exec()
	{
		char buf[81];
		//startEvent.wait();
		for (;;)
		{
			uart << "=>";
			uart.Gets(buf, 60);
			if (*buf)
			{
				if (!Interpreter::parse(buf, uart))
					uart << "ERR";
				uart << "\r\n";
			}
		}
	}
}

DebugTerminalProcess debugTerminalProcess;
