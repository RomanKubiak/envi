/*
  ==============================================================================

    EnviCLI.cpp
    Created: 2 Dec 2013 2:05:00pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviCLI.h"

EnviCLI::EnviCLI (const int argc, char *argv[])
{
	int c;

	while (1)
	{
		static struct option long_options[] =
		{
			{"verbose",			no_argument,		0, 'v'},
			{"background",		no_argument,		0, 'b'},
			{"log-file",		required_argument,	0, 'f'},
			{"log-level",		required_argument,	0, 'l'},
			{"store-file",		required_argument,	0, 's'},
			{"store-format",	required_argument,	0, 'S'},
			{"listen-port",		required_argument,	0, 'p'},
			{"disable-source",	required_argument,	0, 'd'},
			{"list-sources",	required_argument,	0, 'L'},
			{0, 0, 0, 0}
		};
	
		int option_index = 0; 
		int c = getopt_long (argc, argv, "vbf:l:s:S:p:d:L:", long_options, &option_index);

		if (c == -1)
			break;
     
		_DBG("option("+String(c)+"): option=["+String(long_options[option_index].name)+(optarg ? ("] value=["+String(optarg)+"]") : "]"));
		arguments.set (_STR(long_options[option_index].name), _STR(optarg));
	}
}

StringPairArray &EnviCLI::getAllArguments()
{
	return (arguments);
}

const bool EnviCLI::isSet(const String &argument)
{
	return (arguments.getAllKeys().indexOf(argument) >= 0);
}

const String EnviCLI::getParameter(const String &argument)
{
	return (arguments.getValue (argument, String::empty));
}