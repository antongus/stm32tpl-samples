/**
*  @file led-matrix-test.cpp
*
*  LED matrix tests
*
*  Copyright (c) 2017 Anton B. Gusev aka AHTOXA
**/

#include "led-matrix-test.h"
#include "scmRTOS.h"
#include "matrix.h"

void MatrixTests::test1()
{
	Point pt;
	ledMatrix.Clear();
	ledMatrix.PutString(pt, "P: ");
	ledMatrix.PutString(pt, "12.3");
	ledMatrix.update();
	OS::sleep(2500);
}

void MatrixTests::test2()
{
	Point pt;
	ledMatrix.Clear();
	ledMatrix.PutString(pt, "Напр: 12.6В");
	ledMatrix.update();
	OS::sleep(2500);
}

void MatrixTests::test3()
{
	Point pt;
	ledMatrix.Clear();
	ledMatrix.PutString(pt, "Требуйте отстоя пива!");
	ledMatrix.update();
	OS::sleep(2500);
}

void MatrixTests::walkingDotTest(unsigned delay)
{
	Point pt;
	ledMatrix.Clear();
	for (unsigned row = 0; row < 8; row += 2)
	{
		for (unsigned col = 0; col < ledMatrix.Width(); ++col)
		{
			ledMatrix.Clear();
			ledMatrix.SetPixel(Point(col, row));
			ledMatrix.update();
			OS::sleep(delay);
		}
		for (int col = ledMatrix.Width()-1; col >=0; --col)
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
