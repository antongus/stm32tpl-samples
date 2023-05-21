/**
*  @file hw.h
*
*  Project-wide hardware declarations.
*
*  Copyright © 2020 Anton B. Gusev
**/

#pragma once

#include "scmRTOS.h"
#include "processes.h"
#include "pin.h"

/**
 * STM32746G-Discovery board hardware definitions
 */
struct Stm32F746DiscoveryHardware
{
	/// IO Pins
	struct Pins
	{
		using GreenLED = Pin<'I', 1>;

		static void Init()
		{
			GreenLED::Off();
			GreenLED::Mode(OUTPUT);
		}
	};
};

/**
 * UARTs
 */
#include "stm32_uart.h"

/// Console (debug) UART properties
struct ConsoleUartProps
{
	// USART number
	static const STM32::UART::UartNum uartNum = STM32::UART::UART_1;
	// is USART remapped?
	static const STM32::UART::Remap remap = STM32::UART::REMAP_NONE;
	// USART settings
	enum
	{
		BAUDRATE = 115200,               // baud rate
		RX_BUF_SIZE = 32,                // receive buffer size
		TX_BUF_SIZE = 64,                // transmit buffer size
		UART_INTERRUPT_PRIOGROUP = 2,    // priority group for USART interrupt
		UART_INTERRUPT_SUBPRIO = 2,      // sub-priority for USART interrupt
	};
	using PinDE = STM32::UART::DummyDE ; // DE pin for RS-485 or DummyDE otherwise
};

/// Console (debug) UART type
using ConsoleUart = STM32::UART::Uart<ConsoleUartProps>;

/// Console (debug) UART variable declaration
extern ConsoleUart uart;


using Board = Stm32F746DiscoveryHardware;

using GreenLED = Board::Pins::GreenLED;
