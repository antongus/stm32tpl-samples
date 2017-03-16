/**
*  @file main.cpp
*
*  Main project file.
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/

#include "pin.h"
#include "scmRTOS.h"

/// LED indicator.
using LedPin = Pin<'C', 13, 'L'>;


/// LED flasher process
using LedFlasherProcess      = OS::process<OS::pr0, 1200>;
LedFlasherProcess ledFlasherProcess;

int main()
{
	// initialize LED pin
	LedPin::Off();
	LedPin::Mode(OUTPUT);

	// run OS
	OS::run();
}

namespace OS
{
	template <>
	OS_PROCESS void LedFlasherProcess::exec()
	{
		for (;;)
		{
			LedPin::On();
			OS::sleep(100);
			LedPin::Off();
			OS::sleep(900);
		}
	}
}
