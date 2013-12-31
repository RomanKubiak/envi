#ifdef WIRING_PI

#include "EnviDSPCF8591.h"
#include "EnviApplication.h"

EnviDSPCF8591::EnviDSPCF8591(EnviApplication &owner)
	: Thread("EnviDSPCF8591"), i2cAddr(-1), EnviDataSource(owner, "pcf8591"), pcfFd(-1)
{
	pinMap[0] = pinMap[1] = pinMap[2] = pinMap[3] = -1;
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
	EnviDataSource::initialize (_instanceConfig);

	if (instanceConfig.isValid())
	{
		i2cAddr		= (bool)instanceConfig.hasProperty(Ids::i2cAddr) ? (int)getProperty(Ids::i2cAddr).toString().getHexValue32() : 0x48;

#ifdef JUCE_LINUX
		pcfFd		= pcf8591Setup (pinBase = (getInstanceNumber() * 4) + 100, i2cAddr);

		if (pcfFd < 0)
		{
			return (Result::fail ("pcf8591Setup failed for i2cAddr: "+_STR(i2cAddr)+" with pinBase: "+_STR(pinBase)));
		}
		else
		{
			_DBG("EnviDSPCF8591::initialize pcf8591Setup seuccees i2cAddr: "+_STR(i2cAddr)+" pinBase: "+_STR(pinBase));
		}
#endif
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
						pinMap[pin] 		= addValue (instanceConfig.getChild(i).getProperty(Ids::name).toString(), EnviData::stringToUnit (instanceConfig.getChild(i).getProperty(Ids::unit).toString()));
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
	collectFinished (Result::ok());
}

bool EnviDSPCF8591::readPCFvalues()
{
	ScopedLock sl (dataSourceLock);
	for (int i=0; i<4; i++)
	{
		if (pinMap[i] >= 0)
		{
			_DBG("EnviDSPCF8591::readPCFvalues reading pin: "+_STR(i));
			const int value = analogRead(pinBase+i);
			if (value < 0)
			{
				_DBG("EnviDSPCF8591::readPCFvalues negative value on pin read: "+_STR(value));
			}
			else
			{
				_DBG("EnviDSPCF8591::readPCFvalues got value: "+_STR(value));
				setValue (pinMap[i], value);
			}
		}
	}
	return (true);
}

#endif
