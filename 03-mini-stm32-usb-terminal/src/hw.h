/**
*  @file hw.h
*
*  Project-wide hardware declarations.
*
*  Copyright © 2019 Anton B. Gusev aka AHTOXA
**/

#pragma once

#include "stm32.h"
#include "pin.h"
#include "scmRTOS.h"

/// LED indicator pin.
using LedPin = Pin<'C', 13, 'L'>;

/**
 * Start event. Signaled when initialization done
 * and all processes can run.
 */
extern OS::TEventFlag startEvent;
