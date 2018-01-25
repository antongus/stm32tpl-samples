/**
*  @file hw.cpp
*
*  Project-wide hardware definitions.
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/

#include "hw.h"

/// Start event
OS::TEventFlag startEvent;

/// UART object
ConsoleUart uart;

/// UART interrupt handler.
OS_INTERRUPT void USART1_IRQHandler()
{
	OS::TISRW ISR;
	uart.UartIrqHandler();
}
