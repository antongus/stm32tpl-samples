/**
*  @file main.cpp
*
*  Main project file.
*
*  Copyright © 2020 Anton B. Gusev
**/

#include "hw.h"
#include "scmRTOS.h"

/// LED flasher process
using FatfsTestProcess      = OS::process<OS::pr0, 1200>;
using LedFlasherProcess     = OS::process<OS::pr1, 1200>;

FatfsTestProcess fatfsTestProcess;
LedFlasherProcess ledFlasherProcess;

int main()
{
	OS::run();
}

extern void ffTest();

namespace OS
{
template <>
OS_PROCESS void FatfsTestProcess::exec()
{
	OS::sleep(1);

	ffTest();

	for (;;)
	{
		OS::sleep(1000);
	}
}

template <>
OS_PROCESS void LedFlasherProcess::exec()
{
	LedPin::Off();
	LedPin::Mode(OUTPUT);

	for (;;)
	{
		LedPin::On();
		OS::sleep(100);
		LedPin::Off();
		OS::sleep(900);
	}
}
}
