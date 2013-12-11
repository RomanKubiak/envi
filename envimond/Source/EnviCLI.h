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
#include "getopt.h"

class EnviCLI
{
	public:
		EnviCLI (const int _argc, char *_argv[]);
		StringPairArray &getAllArguments();
		const String getParameter(const String &argument);
		const bool isSet(const String &argument);
		void printHelp();
		const bool isValid() { return (valid); }

		JUCE_LEAK_DETECTOR(EnviCLI);

	private:
		StringPairArray arguments;
		bool valid;
};

#endif  // ENVICLI_H_INCLUDED
