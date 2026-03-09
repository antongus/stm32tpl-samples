/**
*  @file terminal.cpp
*
*  test commands and config variables
*
*  Copyright © 2026 Anton B. Gusev
**/

#include "commands.h"
#include "vars.h"
#include "util.h"
#include <cstdlib>

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

/**
 * Echo command
 */
INTERPRETER_COMMAND(SAY)
{
	stream << args;
	return true;
}


/// config variables

/**
 * "GET" command handler.
 * calls get function for a given variable
 */
INTERPRETER_COMMAND(GET)
{
	char *lt = args;
	char *token = GetToken(0, &lt);

	if (token)
	{
		Variable * v = VarList::Find(token);
		if (v)
			return v->Get(lt, stream);
	}
	return false;
}

/**
 * "SET" command handler.
 * calls set function for a given variable
 */
INTERPRETER_COMMAND(SET)
{
	char *lt = args;
	char *token = GetToken(0, &lt);

	if (token)
	{
		Variable * v = VarList::Find(token);
		if (v)
			return v->Set(lt, stream);
	}
	return false;
}

/// define test variable
int var1 = 0;

/// add variable VAR1 to list of variables
DEFINE_VARIABLE(VAR1);

/// define "get" handler for variable VAR1
VARIABLE_GET_HANDLER(VAR1)
{
	stream << var1;
	return true;
}

/// define "set" handler for variable VAR1
VARIABLE_SET_HANDLER(VAR1)
{
	var1 = std::atoi(args);
	return true;
}
