/*
  ==============================================================================

    EnviCLI.cpp
    Created: 2 Dec 2013 2:05:00pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviCLI.h"

EnviCLI::EnviCLI (const int argc, char *argv[]) : valid(false)
{
	int c = -1;
	int option_index = 0;

	while (1)
	{
		static struct option long_options[] =
		{
			{"verbose",			no_argument,		0, 'v'},
			{"background",		no_argument,		0, 'b'},
			{"list-sources",	no_argument,		0, 'L'},
			{"help",			no_argument,		0, 'h'},
			{"log-console",		no_argument,		0, 'c'},
			{"log-file",		required_argument,	0, 'f'},
			{"log-level",		required_argument,	0, 'l'},
			{"store-file",		required_argument,	0, 's'},
			{"store-format",	required_argument,	0, 'S'},
			{"listen-port",		required_argument,	0, 'p'},
			{"enable-source",	required_argument,	0, 'e'},
			{"sources-dir",		required_argument,  0, 'd'},
			{0, 0, 0, 0}
		};

		c = getopt_long (argc, argv, "vbLhf:l:s:S:p:e:d:", long_options, &option_index);

		if (c == -1)
			break;

		switch (c)
		{
			case 'v':
				arguments.set ("verbose", String::empty);
				break;
			case 'b':
				arguments.set ("background", String::empty);
				break;
			case 'h':
				arguments.set ("help", String::empty);
				break;
			case 'L':
				arguments.set ("list-sources", String::empty);
				break;
			case 'c':
				arguments.set ("log-console", String::empty);
				break;

			case 'f':
				arguments.set ("log-file", _STR(optarg));
				break;
			case 'l':
				arguments.set ("log-level", _STR(optarg));
				break;
			case 's':
				arguments.set ("store-file", _STR(optarg));
				break;
			case 'S':
				arguments.set ("store-format", _STR(optarg));
				break;
			case 'p':
				arguments.set ("listen-port", _STR(optarg));
				break;
			case 'e':
				arguments.set ("enable-source", _STR(optarg));
				break;
			case 'd':
				arguments.set ("sources-dir", _STR(optarg));
				break;

			default:
				break;
		}

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

void EnviCLI::printHelp()
{
    String help;
    help 	<< "Usage: " << File::getSpecialLocation(File::currentApplicationFile).getFileName() << " [OPTION]...\n"
			<< "Start collecting data from defined data sources (GPIO/i2c/SPI based sensors).\n"
			<< "Write that data to a file or a SQL database\n\n"
			<< "Mandatory arguments to long options are mandatory for short options too.\n"
			<< "\t-h, --help             \t\tPrint this help\n"
			<< "\t-v, --versbose         \t\tBe vrbose about what we are doing\n"
			<< "\t-f, --log-file=FILE	 \tLog to a file specified by FILE.\n"
			<< "\t-c, --log-console      \tPrint logs to console.\n"
			<< "\t-l, --log-level=LEVEL	 \tWhen logging information, what level to use\n"
			<< "\t                       \t\t(DEBUG, INFO, WARN, ERROR)\n"
			<< "\t-s, --store-file=FILE	 \tWrite sensor data to a file specified by FILE.\n"
			<< "\t                       \t\tThe format of the file will be different\n"
			<< "\t                       \t\tdepending on the store engine chosen\n"
			<< "\t-S, --store-format=FORMAT \tChoose the storage format for the data\n"
			<< "\t                       \t\tthere ae two formats available SQLITE3 and CSV\n"
			<< "\t-p, --listen-port=PORT \t\tWhat TCP port to listen on for JSON-RPC requests\n"
			<< "\t-e, --enable-source=LIST \tA comma seperated list of sources to start\n"
			<< "\t-L, --list-sources     \t\tPrint a list of available data sources and exit\n"
			<< "\t-d, --sources-dir=FILE \t\tWhere to look for data sources configs\n"
			<< "\n"
			<< "Don't report bugs, i don't care :)\n"
			<< "For updates (i doubt there will be any) go to: https://github.com/RomanKubiak/envi \n\n";

	std::cout << help;
}
