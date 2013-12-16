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
	: owner(nullptr), logToConsole(true), logLevel(0)
{
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

void EnviLog::logMessage (const int level, const String &message)
{
	if (level > logLevel)
		return;

	const String msg = Time::getCurrentTime().formatted ("%Y-%m-%d %H:%M:%S") +" ENVI ["+levelToString(level)+"]: "+message;

	Logger::outputDebugString (msg);

	if (logToConsole)
	{
		std::cout << msg << std::endl;
	}

	if (fileLogger)
	{
		fileLogger->logMessage (msg);
	}
}

void EnviLog::logMessage(const String &message)
{
	logMessage(LOG_INFO, message);
}

const String EnviLog::levelToString(const int logLevel)
{
	switch (logLevel)
	{
		case LOG_INFO:
			return ("INF");
		case LOG_WARN:
			return ("WRN");
		case LOG_DEBUG:
			return ("DBG");
		case LOG_ERROR:
			return ("ERR");
		default:
			break;
	}
	return ("INF");
}

const Result EnviLog::setLogToFile(const File fileToLogTo)
{
	if (fileToLogTo.hasWriteAccess())
	{
		fileLogger = new FileLogger (fileToLogTo, "Envi log");
		return (Result::ok());
	}
	else
	{
		return (Result::fail("Can't write to log file: ["+fileToLogTo.getFullPathName()+"]"));
	}
}

void EnviLog::setLogToConsole(const bool _logToConsole)
{
	logToConsole = _logToConsole;
}

void EnviLog::setLogLevel(const int _logLevel)
{
	logLevel = _logLevel;
}
