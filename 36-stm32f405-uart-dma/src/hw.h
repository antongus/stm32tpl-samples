/**
*  @file hw.h
*
*  Project-level hardware configuration
*
*  Copyright 2026 Anton B. Gusev
**/

#pragma once

#include "stm32.h"
#include "pin.h"
#include "stm32_uart_dma.h"

// stm32f4discovery LEDs
using BlueLED   = Pin<'D', 15>;
using RedLED    = Pin<'D', 14>;
using GreenLED  = Pin<'D', 12>;
using OrangeLED = Pin<'D', 13>;

extern TextStream& uart;

/**
 * UART 2 on PA2/PA3
 */
struct Uart2Props
{
	static constexpr auto uartNum     {STM32::UART::UART_2};
	static constexpr auto remap       {STM32::UART::REMAP_NONE};
	static constexpr auto BAUDRATE    {1000000ul};
	static constexpr auto RX_BUF_SIZE {256};
	static constexpr auto TX_BUF_SIZE {256};
	static constexpr auto UART_INTERRUPT_PRIOGROUP {2};
	static constexpr auto UART_INTERRUPT_SUBPRIO {2};
	static constexpr auto RXDMA_INTERRUPT_PRIOGROUP {2};
	static constexpr auto RXDMA_INTERRUPT_SUBPRIO {2};
	using PinDE = STM32::UART::DummyDE;
};
