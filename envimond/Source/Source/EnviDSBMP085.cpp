/*
  ==============================================================================

    EnviDSBMP085.cpp
    Created: 7 Dec 2013 11:40:35pm
    Author:  Administrator

  ==============================================================================
*/

#include "EnviDSBMP085.h"
#include "EnviApplication.h"

EnviDSBMP085::EnviDSBMP085(EnviApplication &owner)
	: Thread("EnviDSBMP085"), i2cAddr(-1), EnviDataSource(owner, "bmp085")
{
}

EnviDSBMP085::~EnviDSBMP085()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
}

const Result EnviDSBMP085::initialize(const ValueTree _instanceConfig)
{
	instanceConfig = _instanceConfig.createCopy();

	if (instanceConfig.isValid())
	{
		i2cAddr				= (bool)instanceConfig.hasProperty(Ids::i2cAddr)	? (int)getProperty(Ids::i2cAddr)	: 0x77;
	}
}

const Result EnviDSBMP085::execute()
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
		return (Result::ok());
	}

	return (Result::ok());
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
