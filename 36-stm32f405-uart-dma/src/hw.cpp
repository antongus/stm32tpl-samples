/**
*  @file hw.cpp
*
*  Project-level hardware configuration
*
*  Copyright © 2026 Anton B. Gusev
**/

#include "hw.h"

STM32::UART::UartDma<Uart2Props> uart2;

TextStream& uart {uart2};


OS_INTERRUPT void USART2_IRQHandler()
{
	OS::TISRW ISR;
	uart2.UartIrqHandler();
}

OS_INTERRUPT void DMA1_Stream5_IRQHandler()
{
	OS::TISRW ISR;
	uart2.RxDmaIrqHandler();
}
