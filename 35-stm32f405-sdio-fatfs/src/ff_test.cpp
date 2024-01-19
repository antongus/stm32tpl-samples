/**
*  @file ff_test.cpp
*
*  fatfs test.
*
*  Copyright © 2024 Anton B. Gusev
**/

#include "hw.h"
#include "scmRTOS.h"
#include "SdCard.h"
#include "Trace.h"

FATFS fatFs;

/**
 * append text to test1.txt file
 * @return
 */
static bool appendTextToFile()
{
	FRESULT res;
	FIL f;

	// open file for writing
	if ((res = f_open(&f, "test1.txt", FA_WRITE | FA_OPEN_ALWAYS)))
	{
		f_close(&f);
		tracer::trace("\r\n error in f_open() :{}", static_cast<uint32_t>(res));
		return false;
	}

	// Move to end of the file to append data
	if ((res = f_lseek(&f, f_size(&f))))
	{
		f_close(&f);
		tracer::trace("\r\n error in f_seek() :{}", static_cast<uint32_t>(res));
		return false;
	}

	UINT written;
	res = f_write(&f, "HELLO, SDIO!\r\n", 14, &written);
	f_close(&f);
	if (res)
	{
		tracer::trace("\r\n error in f_write() :{}", static_cast<uint32_t>(res));
		return false;
	}

	if (written < 14)
	{
		tracer::trace("\r\n write size mismatch() :{}", written);
		return false;
	}

	tracer::trace("\r\n write success");
	return true;
}

static void halt()
{
	SdCardPower::Off();

	OS::sleep(20); // wait for uart transmission ends
	TCritSect cs;  // disable all interrupts and flash led
	for (;;)
	{
		LedPin::On();
		for (auto i = 0; i < 10000000; ++i)
			__asm__ __volatile__ ("nop");
		LedPin::Off();
		for (auto i = 0; i < 10000000; ++i)
			__asm__ __volatile__ ("nop");
	}
}

void ffTest()
{
	SdCardPower::Off();
	SdCardPower::Mode(OUTPUT);

	tracer::trace("\r\n Power on...");
	SdCardPower::On();
	OS::sleep(2000); // some SD cards performs looong initialization when powered on

	for (auto i = 0; i < 10; ++i)
	{
		tracer::trace("\r\n Initializing SD card (attempt {})...",i);
		if (SdCard::init())
			break;
		SdCardPower::Off();
		OS::sleep(50);
		SdCardPower::On();
		OS::sleep(100);
	}
	if (SdCard::getState() != STM32::SD::CardState::Ready)
	{
		tracer::trace("\r\nCould not initialize SD card.");
		halt();
	}

	tracer::trace("\r\n Mounting card...");
	if (auto ret = f_mount(&fatFs, "0:", 1); ret != FR_OK)
	{
		tracer::trace("\r\n mount error {}", static_cast<uint32_t>(ret));
		halt();
	}

	if (auto ret = f_chdrive("0:"); ret != FR_OK)
	{
		tracer::trace("\r\n chdrive error {}", static_cast<uint32_t>(ret));
		halt();
	}

	tracer::trace("\r\n Going to root dir...");
	if (auto res = f_chdir("/"); res != FR_OK)       // ensure we are at top level
	{
		tracer::trace("\r\nDQ: error changing dir to root: {}", static_cast<uint32_t>(res));
		halt();
	}

	tracer::trace("\r\n Appending text to file in root dir...");
	if (!appendTextToFile())
	{
		tracer::trace("\r\n append error");
		halt();
	}

	tracer::trace("\r\n Creating directory DIR1...");
	if (auto res = f_mkdir("DIR1"); res == FR_OK)
	{
		tracer::trace("\r\n created directory \"DIR1\"");
	}
	else if (res == FR_EXIST)
	{
		tracer::trace("\r\n directory \"DIR1\" already exists");
	}
	else
	{
		tracer::trace("\r\nDQ: error creating dir \"DIR1\" ({})", static_cast<uint32_t>(res));
		halt();
	}

	tracer::trace("\r\n Entering directory DIR1...");
	if (auto res = f_chdir("DIR1"); res != FR_OK)
	{
		tracer::trace("\r\nDQ: error entering dir \"DIR1\" ({})", static_cast<uint32_t>(res));
		halt();
	}

	for (auto i = 0; i < 100; ++i)
	{
		OS::sleep(10'000);
		tracer::trace("\r\n Appending text to file in DIR1...");
		if (!appendTextToFile())
		{
			tracer::trace("\r\n append error");
			halt();
		}
	}


}
