/*
  ==============================================================================

    EnviDSDHT.cpp
    Created: 7 Dec 2013 11:40:03pm
    Author:  Administrator

  ==============================================================================
*/

#include "EnviDSDHT11.h"
#include "EnviApplication.h"

EnviDSDHT11::EnviDSDHT11(EnviApplication &owner, const ValueTree instanceConfig)
	: EnviDataSource(owner), Thread("EnviDSDHT11")
{
	instanceState = instanceConfig.createCopy();

	if (instanceState.isValid())
	{
		timeout			= (bool)instanceState.hasProperty (Ids::timeout)	? (int)getProperty(Ids::timeout)	: 5000;
		gpioPin			= (int)instanceState.hasProperty(Ids::gpioPin)		? (int)getProperty(Ids::gpioPin)	: 5;
		iterationsDelay		= (int)instanceState.hasProperty(Ids::delay)		? (int)getProperty(Ids::delay)		: 2000;
		iterations		= (int)instanceState.hasProperty(Ids::iterations)	? (int)getProperty(Ids::iterations)	: 2000;
	}

	result.addValue (EnviData::Value("temperature", EnviData::Celsius));
	result.addValue (EnviData::Value("temperature", EnviData::Fahrenheit));
	result.addValue (EnviData::Value("humidity", EnviData::Percent));
}

EnviDSDHT11::~EnviDSDHT11()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
}

const String EnviDSDHT11::getName()
{
	return (getProperty(Ids::name));
}

const int EnviDSDHT11::getInterval()
{
	return (getProperty(Ids::interval));
}

const int EnviDSDHT11::getTimeout()
{
	return (getProperty(Ids::timeout));
}

const bool EnviDSDHT11::execute()
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

const EnviData EnviDSDHT11::getResult()
{
	ScopedLock sl (dataSourceLock);
	return (result);
}

const var EnviDSDHT11::getProperty (const Identifier &identifier)
{
	ScopedLock sl (dataSourceLock);
	return (instanceState.getProperty (identifier));
}

void EnviDSDHT11::run()
{
	while (1)
	{
		do
		{
			for (int i=0; i<(int)iterations; i++)
			{
				if (readDHTValue())
				{
					triggerAsyncUpdate();
					break;
				}
				else
				{
					wait(iterationsDelay);
				}
			}

			if (threadShouldExit())
			{
				_DBG("EnviDSDHT11::run thread signalled to exit");
				return;
			}

		} while (wait (-1));
	}
}

void EnviDSDHT11::handleAsyncUpdate()
{
	_DBG("EnviDSDHT11::handleAsyncUpdate");
	ScopedLock sl (dataSourceLock);
	owner.sourceWrite (this);
}

#ifndef JUCE_LINUX
bool EnviDSDHT11::readDHTValue()
{
	ScopedLock sl (dataSourceLock);
	Random r(Time::getHighResolutionTicks());
	result[0].value 		= getRandomFloat(100.0f);
	result[0].sampleTime	= Time::getCurrentTime();
	result[1].value 		= getRandomFloat(200.0f);
	result[1].sampleTime	= Time::getCurrentTime();
	result[2].value			= getRandomFloat(100.0f);
	result[2].sampleTime	= Time::getCurrentTime();
	return (true);
}
#else
bool EnviDSDHT11::readDHTValue()
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;
	float f; // fahrenheit

	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

	// pull pin down for 18 milliseconds
	pinMode(gpioPin, OUTPUT);
	digitalWrite(gpioPin, LOW);
	delay(18);

	// then pull it up for 40 microseconds
	digitalWrite(gpioPin, HIGH);
	delayMicroseconds(40);

	// prepare to read the pin
	pinMode(gpioPin, INPUT);

	// detect change and read data
	for ( i=0; i< DHT_MAXTIMINGS; i++)
	{
		counter = 0;
		while (digitalRead(gpioPin) == laststate)
		{
			counter++;
			delayMicroseconds(1);
			if (counter == 255)
			{
				break;
			}
		}

		laststate = digitalRead(gpioPin);

		if (counter == 255) break;

		// ignore first 3 transitions
		if ((i >= 4) && (i%2 == 0))
		{
			// shove each bit into the storage bytes
			dht11_dat[j/8] <<= 1;
			if (counter > 16)
				dht11_dat[j/8] |= 1;
			j++;
		}
	}

	// check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	// print it out if data is good
	if ((j >= 40) && (dht11_dat[4] == ((dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF)) )
	{
		ScopedLock sl (dataSourceLock);
		result[0].value 		= String (_STR(dht11_dat[2])+"."+_STR(dht11_dat[3])).getFloatValue();
		result[0].sampleTime	= Time::getCurrentTime();
		result[1].value 		= dht11_dat[2] * 9. / 5. + 32;
		result[1].sampleTime	= Time::getCurrentTime();
		result[2].value			= String (_STR(dht11_dat[0])+"."+_STR(dht11_dat[1])).getFloatValue();
		result[2].sampleTime	= Time::getCurrentTime();

		_DBG("EnviDSDHT11::readDHTValue got values");
		_DBG(_STR(temperatureFahrenheit)+" "+_STR(temperatureCelsius)+" "+_STR(humidity));

		return (true);
	}
	else
	{
		_DBG("EnviDSDHT11::readDHTValue data invalid gpioPin:"+_STR(gpioPin));
		return (false);
	}
}
#endif
