/**
*  @file terminal-command.h
*
*  Created on: 25 мая 2019 г.
*  Copyright © 2019 Anton B. Gusev aka AHTOXA
**/


#pragma once

#include <memory>

#include "util.h"
#include "textstream.h"

class TerminalCommand
{
public:
	TerminalCommand()
		: command(CommandName)
	{}
	char const* getName() const { return command; }
	TerminalCommand* getNext() const { return next; }
private:
	char const* command;
	TerminalCommand* next { nullptr };
};


template <char const* CommandName>
class TerminalHandler
	: public TerminalCommand
{
public:
	TerminalHandler()
		: command(CommandName)
	{}
	void say(TextStream& stream)
	{
		stream << command;
	}
private:
	char const* command;
};

class TerminalCommands
{
public:
private:
	TerminalCommand* head { nullptr };
};
