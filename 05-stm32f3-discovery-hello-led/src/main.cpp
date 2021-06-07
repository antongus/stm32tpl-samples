/**
*  @file main.cpp
*
*  Main project file.
*
*  Copyright © 2021 Anton B. Gusev
**/

#include "pin.h"
#include "scmRTOS.h"
#include "processes.h"

/// LED indicator.
using RedLed = Pin<'E', 9>;


StateIndicationProcess stateIndicationProcess("StateIndicationProcess");

int main()
{
	// initialize LED pin
	RedLed::Off();
	RedLed::Mode(OUTPUT);

	// run OS
	OS::run();
}

namespace OS
{
	template <>
	OS_PROCESS void StateIndicationProcess::exec()
	{
		for (;;)
		{
			RedLed::On();
			OS::sleep(100);
			RedLed::Off();
			OS::sleep(900);
		}
	}
}
