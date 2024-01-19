/**
*  @file scmRTOS_TARGET_CFG.h
*
*  scmRTOS project-level target (port) configuration.
*
*  Copyright 2023 Anton B. Gusev
**/

#pragma once

#define SCMRTOS_USE_CUSTOM_TIMER 0

// SysTick clock frequency and its interrupt rate in Hz.
#define SYSTICKFREQ     168000000UL
#define SYSTICKINTRATE  1000

// Number of priority bits implemented in hardware.
#define CORE_PRIORITY_BITS  4

