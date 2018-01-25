/**
*  @file processes.h
*
*  Processes declaration.
*  All application processes declared in this file.
*  This allows easy maintain processes count and priorities.
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/

#ifndef PROCESSES_H_INCLUDED
#define PROCESSES_H_INCLUDED

#include "scmRTOS.h"

/// LED matrix test process
using LedMatrixProcess      = OS::process<OS::pr0, 600>;

/// LED flasher process
using LedFlasherProcess      = OS::process<OS::pr1, 200>;

/// declaration of processes objects
extern LedMatrixProcess ledMatrixProcess;
extern LedFlasherProcess ledFlasherProcess;

#endif // PROCESSES_H_INCLUDED
