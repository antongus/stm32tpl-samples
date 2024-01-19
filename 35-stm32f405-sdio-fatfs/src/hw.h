/**
*  @file hw.h
*
*  Project-level hardware configuration
*
*  Copyright 2024 Anton B. Gusev
**/

#pragma once

#include "stm32.h"
#include "pin.h"
#include "stm32_uart.h"
#include "rtc.h"

extern RtcModuleLSE rtc;

using LedPin = Pin<'D', 9>;          // LED indicator
using SdCardPower = Pin<'C', 13>;    // Power for SD card

extern TextStream& debug;

/**
 * UART 1 (debug port)
 */
struct Uart1Props
{
	static constexpr auto uartNum {STM32::UART::UART_1};
	static constexpr auto remap   {STM32::UART::REMAP_NONE};
	static constexpr auto BAUDRATE    {921600ul};
	static constexpr auto RX_BUF_SIZE {128};
	static constexpr auto TX_BUF_SIZE {1024};
	static constexpr auto UART_INTERRUPT_PRIOGROUP {2};
	static constexpr auto UART_INTERRUPT_SUBPRIO {2};
	using PinDE = Pin<'D', 0>;  // DE pin for RS-485
};
