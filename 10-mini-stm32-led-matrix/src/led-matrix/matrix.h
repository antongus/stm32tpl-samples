/**
*  @file matrix.h
*
*  Led matrix
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "max7219.h"
#include "drawbuf.h"

class LedMatrix : public DrawBuffer<32, 8>
{
public:
	LedMatrix();
	void Init();
	void Update();
private:
	/**
	 * Define properties for MAX7219 chain
	 */
	struct Max7219ChainProps
	{
		enum
		{
			CHIP_COUNT = 4,
			DEFAULT_BRIGHTNESS = 2,
			LATCH_NOPS = 8,
		};
		static void Init()
		{
			CLK::Mode(OUTPUT); CLK::Off();
			LATCH::Mode(OUTPUT); LATCH::On();
			DATA::Mode(OUTPUT); DATA::Off();
		}

		static void Latch()
		{
			LATCH::Off();
			for (int i = 0; i < LATCH_NOPS; i++)
				__asm__ __volatile__ ("nop");
			LATCH::On();
		}

		/**
		 * Shift out one word to MAX7219 chain
		 * @param w - word to write
		 */
		static void ShiftWord(uint16_t w)
		{
			for (auto i = 0u; i < 16; ++i)
			{
				if (w & 0x8000)
					DATA::On();
				else
					DATA::Off();
				CLK::Off();
				w <<= 1;
				CLK::On();
			}
		}
	private:
		using CLK   = Pin<'A', 3, 'H'>;
		using LATCH = Pin<'A', 4, 'L'>;
		using DATA  = Pin<'A', 5, 'H'>;

	};

	using Max7219ChainType = TMax7219Chain<Max7219ChainProps>;
	Max7219ChainType max_;
};

extern LedMatrix ledMatrix;



#endif // MATRIX_H_INCLUDED
