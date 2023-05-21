/**
*  @file tests.cpp
*
*  various tests
*
*  Copyright © 2020 Anton B. Gusev aka AHTOXA
**/

#include "commands.h"
#include "vars.h"

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
