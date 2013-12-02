/*
  ==============================================================================

    EnviCLI.cpp
    Created: 2 Dec 2013 2:05:00pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviCLI.h"

EnviCLI::EnviCLI (const int argc, char *argv[], const String &_optionsPattern)
	: optionsPattern(_optionsPattern)
{
	int i = 0;
	while (argv[++i] != NULL)
	{
		arguments.add (argv[i]);
	}
}

StringArray &EnviCLI::getAllArguments()
{
	return (arguments);
}

const bool EnviCLI::requiresParameter(const String &argument)
{
	if (optionsPattern [optionsPattern.indexOf(argument) + 1] == ':')
		return (true);

	return (false);
}

const bool EnviCLI::hasParameter(const String &argument)
{
    if (isSet(argument))
	{
		const String param = arguments[arguments.indexOf(argument)+1];
		if (!isSet (param))
		{
			/* is should be parameter not another argument [starts with -] */
			return (true);
		}
	}

	return (false);
}

const bool EnviCLI::isSet(const String &argument)
{
	if (!argument.startsWith("-"))
		return (false);

	return (arguments.indexOf(argument) ? true : false);
}

const int EnviCLI::getNumArguments()
{
	int count=0;
	for (int i=0; i<arguments.size(); i++)
		if (arguments[i].startsWith("-")) count++;
	return (count);
}

const String EnviCLI::getParameter(const String &argument)
{
	if (hasParameter (argument))
	{
		return (arguments[arguments.indexOf(argument)+1]);
	}

	return (String::empty);
}

const String EnviCLI::getArgument(const int argumentIndex)
{
	int count=0;
	for (int i=0; i<arguments.size(); i++)
	{
		if (count == argumentIndex)
			return (arguments[i]);

		if (arguments[i].startsWith("-")) count++;
	}

	return (String::empty);
}

const String EnviCLI::getParameter(const int argumentIndex)
{
	int count=0;
	for (int i=0; i<arguments.size(); i++)
	{
		if (count == argumentIndex)
		{
			return (getParameter (arguments[i]));
		}

		if (arguments[i].startsWith("-")) count++;
	}
	return (String::empty);
}
