/*
  ==============================================================================

    EnviLog.cpp
    Created: 3 Dec 2013 12:18:09pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviLog.h"

juce_ImplementSingleton (EnviLog)

EnviLog::EnviLog() : owner(nullptr)
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
	std::cout << "Envi: " << message << std::endl;
}

void EnviLog::logMessage(const String &message)
{
	logMessage(LOG_JUCE, message);
}