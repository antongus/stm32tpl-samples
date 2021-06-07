/**
*  @file startup.c
*
*  Startup file for STM32F303xC devices
*
*  Copyright © 2021 Anton B. Gusev
**/

#include <string.h>
#include "CMSIS/stm32f3xx.h"
     

extern unsigned char __idata_start[];
extern unsigned char __data_start[];
extern unsigned char __data_end[];
extern unsigned char __bss_start[];
extern unsigned char __bss_end[];

__attribute__ ((weak))
int  __low_level_init();
void __libc_init_array();
int  main();          

#define USE_HSE

static void initClocks()
{
	// set flash latency to 2 cycles
	FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY)
			| 0 * FLASH_ACR_LATENCY_0
			| 1 * FLASH_ACR_LATENCY_1
			| 1 * FLASH_ACR_PRFTBE
			;

#ifdef USE_HSE
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY)){}

	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL))
			| RCC_CFGR_PLLSRC_HSE_PREDIV
			| RCC_CFGR_PLLMUL9
			| RCC_CFGR_USBPRE_DIV1_5
			;
#else
	RCC->CR |= RCC_CR_HSION;
	while(!(RCC->CR & RCC_CR_HSIRDY)){}

	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL))
			| RCC_CFGR_PLLSRC_HSI_DIV2
			| RCC_CFGR_PLLMUL12
			;
#endif
	RCC->CFGR2 = 0;

	// HCLK = SYSCLK, PCLK1 = HCLK/2, PCLK2 = HCLK
	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2))
			| RCC_CFGR_HPRE_DIV1
			| RCC_CFGR_PPRE1_DIV2
			| RCC_CFGR_PPRE2_DIV1
			;

	// turn PLL on
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY)) {}

	// select PLL as system clock source
	RCC->CFGR = ((RCC->CFGR & (~RCC_CFGR_SW)) | RCC_CFGR_SW_PLL);
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
}

void initHardware()
{
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  // set CP10 and CP11 Full Access

	RCC->CR |= RCC_CR_HSION;

	// Reset CFGR register
	RCC->CFGR &= 0xF87FC00CU;

	// Reset HSEON, CSSON and PLLON bits
	RCC->CR &= 0xFEF6FFFFU;

	// Reset HSEBYP bit
	RCC->CR &= 0xFFFBFFFFU;

	// Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE bits
	RCC->CFGR &= 0xFF80FFFFU;

	// Reset PREDIV1[3:0] bits
	RCC->CFGR2 &= 0xFFFFFFF0U;

	// Reset USARTSW[1:0], I2CSW and TIMs bits
	RCC->CFGR3 &= 0xFF00FCCCU;

	// Disable all interrupts
	RCC->CIR = 0x00000000U;

	initClocks();

	// enable GPIOx periph
	RCC->AHBENR |= 0
			| RCC_AHBENR_GPIOAEN
			| RCC_AHBENR_GPIOBEN
			| RCC_AHBENR_GPIOCEN
			| RCC_AHBENR_GPIODEN
			| RCC_AHBENR_GPIOEEN
			| RCC_AHBENR_GPIOFEN
			;

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
	initHardware();
}

int __low_level_init()
{
    return 1;
}

