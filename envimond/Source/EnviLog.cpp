/*
  ==============================================================================

    EnviLog.cpp
    Created: 3 Dec 2013 12:18:09pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviLog.h"
#include "EnviApplication.h"

juce_ImplementSingleton (EnviLog)

EnviLog::EnviLog()
	: owner(nullptr), logToFile(false), logToConsole(true)
{
	_DBG("EnviLog::ctor");
	Logger::setCurrentLogger (this);
}

EnviLog::~EnviLog()
{
	clearSingletonInstance();
}

void EnviLog::setOwner (EnviApplication *_owner)
{
	owner = _owner;
}

void EnviLog::handleAsyncUpdate()
{
}

void EnviLog::logMessage (const int level, const String &message)
{
	if (logToConsole)
	{
		std::cout << "ENVI [" << levelToString(level) << "]: " << message << std::endl;
	}
}

void EnviLog::logMessage(const String &message)
{
	logMessage(LOG_JUCE, message);
}

const String EnviLog::levelToString(const int logLevel)
{
	switch (logLevel)
	{
		case LOG_INFO:
			return ("INFO");
		case LOG_WARN:
			return ("WARN");
		case LOG_DEBUG:
			return ("DEBUG");
		case LOG_ERROR:
			return ("ERROR");
		case LOG_JUCE:
			return ("JUCE");
		default:
			break;
	}
	return ("INFO");
}

const Result EnviLog::setLogToFile(const bool _logToFile)
{
	logToFile	= logToFile;

	if (logToFile)
	{
		if (owner)
		{
			File f(owner->getOption("log-file").toString());
			if (f.hasWriteAccess())
			{
				fileLogger = new FileLogger (f, "Envi log");
			}
			else
			{
				return (Result::fail("Can't write to log file: ["+f.getFullPathName()+"]"));
			}
		}
	}
	return (Result::ok());
}
