/**
*  @file led-matrix-test.cpp
*
*  LED matrix test process.
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/

#include "scmRTOS.h"
#include "matrix.h"
#include "processes.h"

Max7219ChainType max;

LedMatrixProcess ledMatrixProcess;

namespace OS
{
	template <>
	OS_PROCESS void LedMatrixProcess::exec()
	{
		max.Init();

		for (;;)
		{
			max.Init();
			max.SetBrightness(2);
			max.Cls();
			max.Test(50);
			OS::sleep(1000);
		}
	}
}
