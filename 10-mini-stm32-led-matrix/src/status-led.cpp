/**
*  @file status-led.cpp
*
*  Status led process.
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/

#include "pin.h"
#include "scmRTOS.h"
#include "processes.h"

/// LED indicator pin.
using LedPin = Pin<'C', 13, 'L'>;

LedFlasherProcess ledFlasherProcess;

namespace OS
{
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
}
