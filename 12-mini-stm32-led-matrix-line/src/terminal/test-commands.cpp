/**
*  @file tests.cpp
*
*  various tests
*
*  Copyright © 2017 Anton B. Gusev aka AHTOXA
**/

#include "commands.h"
#include "vars.h"
#include "scmRTOS.h"

/**
 * Test command.
 * Executed if "TEST1" entered to terminal prompt.
 * There are two parameters passed to command handler:
 * @param stream - reference to TextStream object. Command handler can output any data to it.
 * @param args (char*) - command parameters. Command can parse them.
 */
INTERPRETER_COMMAND(TEST1)
{
	stream << "TEST1";
	return true;
}

INTERPRETER_COMMAND(SAY)
{
	stream << args;
	return true;
}

INTERPRETER_COMMAND(SHUTDOWN)
{
	stream << "OK";
	return true;
}

INTERPRETER_COMMAND(SET_I_LIM1)
{
	stream << "OK";
	return true;
}

INTERPRETER_COMMAND(CHECKTARGET)
{
	static int i = 0;
	stream << (++i & 1 ? "OK" : "FAULT");
	return true;
}

INTERPRETER_COMMAND(WAITTE)
{
	OS::sleep(6000);
	stream << "OK";
	return true;
}

