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

LedMatrixProcess ledMatrixProcess("LedMatrixProcess");

namespace OS
{
	template <>
	OS_PROCESS void LedMatrixProcess::exec()
	{
		static constexpr unsigned delay = 100;
		ledMatrix.Init();
		for (;;)
		{
//			Point pt{0, 0};
//			ledMatrix.PutString(pt, "HELLO");
//			OS::sleep(1200);
			ledMatrix.Clear();
			for (unsigned row = 0; row < 8; row += 2)
			{
				for (unsigned col = 0; col < 32; ++col)
				{
					ledMatrix.Clear();
					ledMatrix.SetPixel(Point(col, row));
					ledMatrix.Update();
					OS::sleep(delay);
				}
				for (int col = 31; col >=0; --col)
				{
					ledMatrix.Clear();
					ledMatrix.SetPixel(Point(col, row+1));
					ledMatrix.Update();
					OS::sleep(delay);
				}
			}
			for (unsigned row = 6; row; --row)
			{
				ledMatrix.Clear();
				ledMatrix.SetPixel(Point(0, row));
				ledMatrix.Update();
				OS::sleep(delay);
			}
		}
	}
}
