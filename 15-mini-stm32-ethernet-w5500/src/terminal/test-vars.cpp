/**
*  @file test-vars.cpp
*
*  Some test variables and utility functions.
*
*  Copyright © 2016 Anton B. Gusev aka AHTOXA
**/

#include "commands.h"
#include "vars.h"
#include "util.h"
#include <cstdlib>

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
