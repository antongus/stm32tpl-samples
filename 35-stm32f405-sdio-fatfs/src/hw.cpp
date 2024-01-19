/**
*  @file hw.cpp
*
*  Project-level hardware configuration
*
*  Copyright © 2024 Anton B. Gusev
**/

#include "hw.h"

RtcModuleLSE rtc;

STM32::UART::Uart<Uart1Props> uart1;

TextStream& debug {uart1};


OS_INTERRUPT void USART1_IRQHandler()
{
	OS::TISRW ISR;
	uart1.UartIrqHandler();
}
