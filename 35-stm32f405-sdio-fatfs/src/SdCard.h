/**
*  @file SdCard.h
*
*  SD-карта памяти, подключённая к SDIO
*
*  Copyright (c) 2024 by Anton Gusev
**/

#pragma once

#include "pin.h"
#include "hw.h"
#include "ff.h"
#include "diskio.h"
#include "Trace.h"

static constexpr auto CARD1 {0};

/**
 * SDIO stuff
 */
#include "stm32_sdio_sd.h"

// Props for SdioCore class template.
struct SdioCoreProps
{
	static constexpr auto sdioInterruptPrioGroup {1};
	static constexpr auto sdioInterruptSubprio   {1};
	static constexpr auto dmaInterruptPrioGroup  {2};
	static constexpr auto dmaInterruptSubprio    {2};

	static constexpr auto hiSpeedDivisor {4};
	static constexpr auto commandTimeout {2'000'000u};

	static constexpr auto debugTrace {(Trace::flags & Trace::Sdio) != 0};

	static void powerOn()
	{
		if (!isPowerOn())
		{
			SdCardPower::On();
			OS::sleep(2500);  // некоторые карточки очень долго "просыпаются"
		}
	}
	static void powerOff()
	{
		if (!isPowerOn())
		{
			SdCardPower::Off();
		}
	}
	static bool isPowerOn() { return SdCardPower::Signalled(); }


	template <typename ... Args>
	static void trace(const char* format, const Args& ... args)
	{
		if constexpr (Trace::flags & Trace::Sdio)
			tracer::trace(format, args...);
	}
};

using SdCard = STM32::SD::SdioSdCard<SdioCoreProps>;
