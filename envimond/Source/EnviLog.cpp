/*
  ==============================================================================

    EnviLog.cpp
    Created: 3 Dec 2013 12:18:09pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviLog.h"

EnviLog::EnviLog(EnviApplication &_owner) : owner(_owner)
{
	_DBG("EnviLog::ctor");
	Logger::setCurrentLogger (this);
}

EnviLog::~EnviLog()
{
}

void EnviLog::handleAsyncUpdate()
{
}

void EnviLog::logMessage (const String &message)
{
	std::cout << "Envi: " << message << std::endl;
}
