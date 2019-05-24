/**
*  @file startup.c
*
*  Startup file for STM32F1xx devices
*
*  Copyright © 2016 Anton B. Gusev aka AHTOXA
**/

#include <string.h>
#include "CMSIS/stm32f10x.h"
     

extern unsigned char __idata_start[];
extern unsigned char __data_start[];
extern unsigned char __data_end[];
extern unsigned char __bss_start[];
extern unsigned char __bss_end[];

__attribute__ ((weak))
int  __low_level_init();
void __libc_init_array();
int  main();          


static inline void InitClocks()
{
	// Enable HSE
	RCC->CR = (RCC->CR &~(RCC_CR_HSEON | RCC_CR_HSEBYP))| RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY)) ;

	// Enable Prefetch Buffer
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	// Flash 2 wait state for 72MHz (1WS for 24..48 MHz range - .)
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_2;

    // HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK/2
    RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1 | RCC_CFGR_ADCPRE))
    		| RCC_CFGR_HPRE_DIV1
			| RCC_CFGR_PPRE2_DIV1
			| RCC_CFGR_PPRE1_DIV2
			| RCC_CFGR_ADCPRE_DIV6
			;

	RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
	RCC->CFGR |= (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);                               // 8*9=72 MHz

	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;

	// Wait till PLL is ready
	while(!(RCC->CR & RCC_CR_PLLRDY)) ;

	// Select PLL as system clock source
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;

    // Wait till PLL is used as system clock source
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) ;
}

void InitHw()
{
	// Set HSION bit
	RCC->CR |= RCC_CR_HSION;
	// Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] and MCO[2:0] bits
	RCC->CFGR &= (uint32_t)0xF8FF0000;
	// Reset HSEON, CSSON and PLLON bits
	RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON);
	// Reset HSEBYP bit
	RCC->CR &= ~RCC_CR_HSEBYP;
	// Reset PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE bits
	RCC->CFGR &= (uint32_t)0xFF80FFFF;

	// Disable all interrupts and clear pending bits
	RCC->CIR = 0x009F0000;

	InitClocks();

	// enable IOPx periph
	RCC->APB2ENR |=
			RCC_APB2ENR_IOPAEN |
			RCC_APB2ENR_IOPBEN |
			RCC_APB2ENR_IOPCEN |
			RCC_APB2ENR_IOPDEN |
#ifdef RCC_APB2ENR_IOPEEN
			RCC_APB2ENR_IOPEEN |
#endif
			RCC_APB2ENR_AFIOEN;

//	NVIC_SetPriorityGrouping(7);	// no preemption,           4 bit of subprio (0..15)
//	NVIC_SetPriorityGrouping(6);	// 1 bit preemption (0..1), 3 bit of subprio (0..7)
	NVIC_SetPriorityGrouping(5);	// 2 bit preemption (0..3), 2 bit of subprio (0..3)
//	NVIC_SetPriorityGrouping(4);	// 3 bit preemption (0..3), 1 bit of subprio (0..1)
//	NVIC_SetPriorityGrouping(3);	// 4 bit preemption(0..15), no subprio
}

void Reset_Handler()
{
    if(__low_level_init())
    {
        memcpy(__data_start, __idata_start, __data_end - __data_start); // copy initialized variables
        memset(__bss_start, 0, __bss_end - __bss_start);                // zero-fill uninitialized variables
    	__asm__ __volatile__ ("cpsid i");
        __libc_init_array();                                            // low-level init & ctor loop
    	__asm__ __volatile__ ("cpsie i");
    }
    main();
}

__attribute__ ((weak))
void _init()
{
	InitHw();
}

int __low_level_init()
{
    return 1;
}

