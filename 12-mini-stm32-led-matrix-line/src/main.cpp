/**
*  @file main.cpp
*
*  Main project file.
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/

#include "scmRTOS.h"
#include "hw.h"
#include "processes.h"
#include "usb-channel.h"
#include "matrix.h"
#include "led-matrix-test.h"

LedMatrixProcess ledMatrixProcess("LedMatrixProcess");
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
	OS_PROCESS void LedMatrixProcess::exec()
	{
		OS::sleep(1);
		ledMatrix.init();
		startEvent.signal();


		for (;;)
		{
			MatrixTests::test1();
			MatrixTests::test2();
			MatrixTests::test3();
			MatrixTests::walkingDotTest(40);
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
