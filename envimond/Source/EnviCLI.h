/*
  ==============================================================================

    EnviCLI.h
    Created: 2 Dec 2013 2:05:00pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVICLI_H_INCLUDED
#define ENVICLI_H_INCLUDED

#include "EnviIncludes.h"

class EnviCLI
{
	public:
		EnviCLI (const int argc, char *argv[], const String &_options);
		StringArray &getAllArguments();
		const bool requiresParameter(const juce_wchar optionToCheck);
		const String getParameter(const String &argument);
		const String getParameter(const int argumentIndex);
		const bool isSet(const String &argument);
		const bool requiresParameter(const String &argument);
		const bool hasParameter(const String &argument);
		const int getNumArguments();
		const String getArgument(const int argumentIndex);
	private:
		String optionsPattern;
		StringArray arguments;
};

#endif  // ENVICLI_H_INCLUDED
