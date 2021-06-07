/**
*  @file processes.h
*
*  OS processes
*
*  Copyright © 2021 Anton B. Gusev
**/

#pragma once

#include "scmRTOS.h"

/// state indication process (flashes LED)
using StateIndicationProcess  = OS::process<OS::pr0, 1200>;

extern StateIndicationProcess stateIndicationProcess;
