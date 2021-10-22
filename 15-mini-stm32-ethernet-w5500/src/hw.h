/**
*  @file hw.h
*
*  Project-wide hardware declarations.
*
*  Copyright © 2020 Anton B. Gusev
**/

#pragma once

#include "stm32.h"
#include "pin.h"
#include "scmRTOS.h"
#include "stm32_uart.h"
#include "stm32_spi.h"

struct Board
{
	/// LED indicator pin.
	using LedPin = Pin<'C', 13, 'L'>;

	/// debug UART properties
	static constexpr auto debugUartNum {STM32::UART::UART_1};
	static constexpr auto debugUartRemap {STM32::UART::REMAP_NONE};

	/// W5500 SPI properties
	static constexpr auto w5500SpiNum {STM32::SPI::SPI_1};
	static constexpr auto w5500SpiRemap {STM32::SPI::REMAP_NONE};

	/// W5500 pins
	struct EthPins
	{
		using Rst = Pin<'A', 2, 'L'>;
		using Int = Pin<'A', 3, 'L'>;
		using CS  = Pin<'A', 4, 'L'>;
		using PM0 = Pin<'C', 0>;
		using PM1 = Pin<'C', 1>;
		using PM2 = Pin<'C', 2>;
		static void init()
		{
			Rst::Off();
			Rst::Mode(OUTPUT);
			CS::Off();
			CS::Mode(OUTPUT);
		}
	};

};


/**
 * Start event. Signaled when initialization done
 * and all processes can run.
 */
extern OS::TEventFlag startEvent;

/**
 * UARTs
 */

/// Console (debug) UART properties
struct ConsoleUartProps
{
	// USART number
	static constexpr auto uartNum {Board::debugUartNum};
	// is USART remapped?
	static constexpr auto remap {Board::debugUartRemap};
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

/// SPI for Ethernet chip
struct SpiEthernetProps
{
	static constexpr auto NUMBER           {Board::w5500SpiNum};
	static constexpr auto REMAP            {Board::w5500SpiRemap};
	static constexpr auto InitialDivisor   {STM32::SPI::SPI_DIV_4};
	static constexpr auto InitialCPOL      {STM32::SPI::CPOL_L};
	static constexpr auto InitialCPHA      {STM32::SPI::CPHA_1};
	static constexpr auto UseInterrupt     {false};
	static constexpr uint32_t IrqPrioGroup {2};
	static constexpr uint32_t IrqSubPrio   {2};
};

using SpiEthernetType = STM32::SPI::Spi<SpiEthernetProps>;
extern SpiEthernetType spiEthernet;


