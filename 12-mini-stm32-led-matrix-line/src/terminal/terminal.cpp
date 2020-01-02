/**
*  @file terminal.cpp
*
*  Serial port debug terminal.
*
*  Copyright © 2016 Anton B. Gusev aka AHTOXA
**/

#include "hw.h"
#include "scmRTOS.h"
#include "commands.h"
#include "processes.h"
#include "vars.h"
#include "util.h"
#include "usb-channel.h"


namespace OS
{
	template <>
	OS_PROCESS void DebugTerminalProcess::exec()
	{
		char buf[81];

		startEvent.wait();
		for (;;)
		{
			if (!usbChannel.isConfigured())
			{
				OS::sleep(10);
				continue;
			}
			usbChannel << "=>";
			usbChannel.Gets(buf, 60);
			if (*buf)
			{
				if (!Interpreter::parse(buf, usbChannel))
					usbChannel << "ERR";
				usbChannel << "\r\n";
			}
		}
	}
}
