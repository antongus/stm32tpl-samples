/**
*  @file matrix.cpp
*
*  Led matrix
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/

#include "matrix.h"

LedMatrix ledMatrix;

LedMatrix::LedMatrix()
{
}

void LedMatrix::init()
{
	max_.Init();
	max_.SetBrightness(2);
	max_.Cls();
}

void LedMatrix::update()
{
	for (auto digit = 0u; digit < max_.DIGIT_COUNT; ++digit)
	{
		max_.SetDigit(digit, GetHByte({(digit / 8) * 8, digit % 8}));
		continue;

		Point pt { (digit / 8) * 8, digit % 8 };
		unsigned byte {0};
		for (auto bit = 0u; bit < 8; ++bit)
		{
			if (GetPixel(pt))
				byte |= 1u << bit;
			++pt.col;
		}
		max_.SetDigit(digit, byte);
	}
}
