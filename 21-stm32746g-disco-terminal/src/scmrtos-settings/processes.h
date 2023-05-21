/**
*  @file processes.h
*
*  Processes declaration.
*  All application processes declared in this file.
*  This allows easy maintain processes count and priorities.
*
*  Copyright © 2020 Anton B. Gusev
**/

#pragma once

#include "scmRTOS.h"

/// High priority process
using HighPriorityProcess      = OS::process<OS::pr0, 600>;

/// LED flasher process
using LedFlasherProcess      = OS::process<OS::pr1, 200>;

/// declaration of processes
extern HighPriorityProcess highPriorityProcess;
extern LedFlasherProcess ledFlasherProcess;
