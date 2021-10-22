/**
*  @file processes.h
*
*  Processes declaration.
*  All application processes declared in this file.
*  This allows easy maintain processes count and priorities.
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/

#pragma once

#include "scmRTOS.h"

/// LED matrix test process
using EthernetServerProcess      = OS::process<OS::pr0, 600>;

/// LED flasher process
using LedFlasherProcess      = OS::process<OS::pr1, 200>;

/// Debug terminal process
using DebugTerminalProcess  = OS::process<OS::pr2, 1200>;


/// declaration of processes objects
extern EthernetServerProcess ethernetServerProcess;
extern LedFlasherProcess ledFlasherProcess;
extern DebugTerminalProcess debugTerminalProcess;
