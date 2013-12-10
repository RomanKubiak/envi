/*
  ==============================================================================

    EnviDSBMP085.cpp
    Created: 7 Dec 2013 11:40:35pm
    Author:  Administrator

  ==============================================================================
*/

#include "EnviDSBMP085.h"

EnviDSBMP085::EnviDSBMP085(EnviApplication &_owner, const ValueTree _instanceConfig)
	: EnviDataSource(_owner, _instanceConfig), Thread("EnviDSBMP085"), i2cAddr(-1), timeout(-1)
{
	if (instanceConfig.isValid())
	{
		timeout				= (bool)instanceConfig.hasProperty(Ids::timeout)	? (int)getProperty(Ids::timeout)	: 5000;
		i2cAddr				= (bool)instanceConfig.hasProperty(Ids::i2cAddr)	? (int)getProperty(Ids::i2cAddr)	: 0x77;
		index				= result.dataSourceId	= BMP086_DS + (int)getProperty(Ids::index);
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