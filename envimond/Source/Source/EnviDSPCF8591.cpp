#include "EnviDSPCF8591.h"
#include "EnviApplication.h"

EnviDSPCF8591::EnviDSPCF8591(EnviApplication &owner)
	: Thread("EnviDSPCF8591"), i2cAddr(-1), EnviDataSource(owner, "pcf8591")
{
	pinMap[0] = pinMap[1] = pinMap[2] = pinMap[3] = false;
	result.addValue (EnviData::Value("AIN0", EnviData::Integer));
	result.addValue (EnviData::Value("AIN1", EnviData::Integer));
	result.addValue (EnviData::Value("AIN2", EnviData::Integer));
	result.addValue (EnviData::Value("AIN3", EnviData::Integer));
}

EnviDSPCF8591::~EnviDSPCF8591()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
}

const Result EnviDSPCF8591::initialize(const ValueTree _instanceConfig)
{
	instanceConfig = _instanceConfig.createCopy();

	if (instanceConfig.isValid())
	{
		i2cAddr		= (bool)instanceConfig.hasProperty(Ids::i2cAddr) ? (int)getProperty(Ids::i2cAddr).toString().getHexValue32() : 0x48;
		if (instanceConfig.getNumChildren() > 0)
		{
			for (int i=0; i<instanceConfig.getNumChildren(); i++)
			{
				if (instanceConfig.getChild(i).hasProperty(Ids::pin))
				{
					const int pin = instanceConfig.getChild(i).getProperty(Ids::pin);
					
					if (pin >= 0 && pin <= 3)
					{
						_DBG("EnviDSPCF8591::initialize using pin: "+_STR(pin));
						pinMap[pin] 		= true;
						if (instanceConfig.getChild(i).hasProperty(Ids::name))
							result[pin].name 	= instanceConfig.getChild(i).getProperty(Ids::name).toString();
						if (instanceConfig.getChild(i).hasProperty(Ids::unit))
							result[pin].unit 	= EnviData::stringToUnit (instanceConfig.getChild(i).getProperty(Ids::unit).toString());
					}
					else
					{
						_INF("EnviDSPCF8591::initialize invalid pin number for value: "+instanceConfig.getChild(i).getProperty(Ids::name).toString());
					}
				}
			}
			return (setAllExpressions());
		}
	}

	return (Result::ok());
}

const Result EnviDSPCF8591::execute()
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

void EnviDSPCF8591::run()
{
	while (1)
	{
		do
		{
			if (readPCFvalues())
			{
				triggerAsyncUpdate();
				continue;
			}

			if (threadShouldExit())
			{
				_DBG("EnviDSPCF8591::run thread signalled to exit");
				return;
			}

		} while (wait (-1));
	}
}

void EnviDSPCF8591::handleAsyncUpdate()
{
}

#ifndef JUCE_LINUX
bool EnviDSPCF8591::readPCFvalues()
{
	ScopedLock sl (dataSourceLock);
	return (true);
}
#else
bool EnviDSPCF8591::readPCFvalues()
{
	ScopedLock sl (dataSourceLock);
	for (int i=0; i<4; i++)
	{
		if (pinMap[i])
		{
			_DBG("EnviDSPCF8591::readPCFvalues reading pin: "+_STR(i));
		}
	}
	return (true);
}
#endif
