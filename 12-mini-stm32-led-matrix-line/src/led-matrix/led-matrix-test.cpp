/**
*  @file led-matrix-test.cpp
*
*  LED matrix test process.
*
*  Copyright В© 2017 Anton B. Gusev aka AHTOXA
**/

#include "scmRTOS.h"
#include "matrix.h"
#include "processes.h"

LedMatrixProcess ledMatrixProcess("LedMatrixProcess");

struct MatrixTests
{
	static void test1()
	{
		Point pt;
		ledMatrix.Clear();
		ledMatrix.PutString(pt, "P:");
		pt.col += 2;
		ledMatrix.PutString(pt, "123");
		ledMatrix.update();
		OS::sleep(2500);
	}

	static void test2()
	{
		Point pt;
		ledMatrix.Clear();
		ledMatrix.PutString(pt, "Вес:123т");
		ledMatrix.update();
		OS::sleep(2500);
	}

	static void test3()
	{
		Point pt;
		ledMatrix.Clear();
		ledMatrix.PutString(pt, "Нужен долив!");
		ledMatrix.update();
		OS::sleep(2500);
	}
};

namespace OS
{
	template <>
	OS_PROCESS void LedMatrixProcess::exec()
	{
		static constexpr unsigned delay = 100;

		ledMatrix.init();

		for (;;)
		{
			MatrixTests::test1();
			MatrixTests::test2();
			MatrixTests::test3();
		}

		for (;;)
		{
			Point pt{0, 0};
			ledMatrix.PutString(pt, "HELLO");
			ledMatrix.update();
			OS::sleep(5000);
			ledMatrix.Clear();
			for (unsigned row = 0; row < 8; row += 2)
			{
				for (unsigned col = 0; col < 64; ++col)
				{
					ledMatrix.Clear();
					ledMatrix.SetPixel(Point(col, row));
					ledMatrix.update();
					OS::sleep(delay);
				}
				for (int col = 63; col >=0; --col)
				{
					ledMatrix.Clear();
					ledMatrix.SetPixel(Point(col, row+1));
					ledMatrix.update();
					OS::sleep(delay);
				}
			}
			for (unsigned row = 6; row; --row)
			{
				ledMatrix.Clear();
				ledMatrix.SetPixel(Point(0, row));
				ledMatrix.update();
				OS::sleep(delay);
			}
		}
	}
}
