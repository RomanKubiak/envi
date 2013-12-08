/*
  ==============================================================================

    EnviDSBMP085.cpp
    Created: 7 Dec 2013 11:40:35pm
    Author:  Administrator

  ==============================================================================
*/

#include "EnviDSBMP085.h"

EnviDSBMP085::EnviDSBMP085(EnviApplication &owner, const ValueTree instanceConfig)
	: EnviDataSource(owner), Thread("EnviDSBMP085"), i2cAddr(-1), timeout(-1)
{
	instanceState = instanceConfig.createCopy();

	if (instanceState.isValid())
	{
		timeout = (bool)instanceState.hasProperty (Ids::timeout) ? (int)getProperty(Ids::timeout) : 5000;

		if (!instanceState.hasProperty(Ids::i2cAddr))
		{
			_WRN("EnviDSBMP085::ctor no i2cAddr set, disabling source");
			setDisabled(true);
		}
		else
		{
			i2cAddr	= getProperty(Ids::i2cAddr);
		}
	}
}

EnviDSBMP085::~EnviDSBMP085()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
}

const String EnviDSBMP085::getName()
{
	return (getProperty(Ids::name));
}

const int EnviDSBMP085::getInterval()
{
	return (getProperty(Ids::interval));
}

const int EnviDSBMP085::getTimeout()
{
	return (getProperty(Ids::timeout));
}

const bool EnviDSBMP085::execute()
{
	if (!isDisabled())
	{
		if (isThreadRunning())
		{
			notify();
		}
		else
		{
			startThread();
		}
		return (true);
	}

	return (false);
}

const EnviData EnviDSBMP085::getResult()
{
	ScopedLock sl (dataSourceLock);
	return (result);
}

const var EnviDSBMP085::getProperty (const Identifier &identifier)
{
	ScopedLock sl (dataSourceLock);
	return (instanceState.getProperty (identifier));
}

void EnviDSBMP085::run()
{
	while (1)
	{
		do
		{
			if (threadShouldExit())
			{
				_DBG("EnviDSBMP085::run thread signalled to exit");
				return;
			}

			triggerAsyncUpdate();
		} while (wait (-1));
	}
}

void EnviDSBMP085::handleAsyncUpdate()
{
}

#ifdef JUCE_LINUX
#endif