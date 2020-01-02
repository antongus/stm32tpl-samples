/**
*  @file main.cpp
*
*  Main project file.
*
*  Copyright © 2020 Anton B. Gusev aka AHTOXA
*
*  This file is part of stm32tpl-samples project.
*  https://github.com/antongus/stm32tpl-samples
**/


#include "hw.h"

HighPriorityProcess highPriorityProcess;
LedFlasherProcess ledFlasherProcess;

int main()
{
    // run
    OS::run();
}

namespace OS
{

/// high priority process (does nothing)
template <>
OS_PROCESS void HighPriorityProcess::exec()
{
    // configure pins
	Board::Pins::Init();
	for(;;)
	{
		OS::sleep(1);
	}
}

/// LED flasher process
template <>
OS_PROCESS void LedFlasherProcess::exec()
{
	for(;;)
	{
		GreenLED::On();
		sleep(50);
		GreenLED::Off();
		sleep(950);
	}
}

} // namespace OS
