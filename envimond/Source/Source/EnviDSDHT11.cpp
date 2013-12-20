/*
  ==============================================================================

    EnviDSDHT.cpp
    Created: 7 Dec 2013 11:40:03pm
    Author:  Administrator

  ==============================================================================
*/

#include "EnviDSDHT11.h"
#include "EnviApplication.h"

EnviDSDHT11::EnviDSDHT11(EnviApplication &owner)
	: Thread("EnviDSDHT11"), gpioPin(-1), EnviDataSource(owner, "dht11")
{
	addValue ("Temperature", EnviData::Celsius);
	addValue ("Humidity", EnviData::Percent);
}

EnviDSDHT11::~EnviDSDHT11()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
}

const Result EnviDSDHT11::initialize(const ValueTree _instanceConfig)
{
	EnviDataSource::initialize (_instanceConfig);

	if (instanceConfig.isValid())
	{
		gpioPin			= (int)instanceConfig.hasProperty(Ids::gpioPin)	? (int)getProperty(Ids::gpioPin)	: 5;
		iterationsDelay		= (int)instanceConfig.hasProperty(Ids::delay)		? (int)getProperty(Ids::delay)		: 2000;
		iterations		= (int)instanceConfig.hasProperty(Ids::iterations)	? (int)getProperty(Ids::iterations)	: 2000;
	}

	return (Result::ok());
}

const Result EnviDSDHT11::execute()
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
	collectFinished (Result::ok());
}

#ifdef JUCE_LINUX
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
		setValue (1, String (_STR(dht11_dat[0])+"."+_STR(dht11_dat[1])).getFloatValue());
		setValue (0, String (_STR(dht11_dat[2])+"."+_STR(dht11_dat[3])).getFloatValue());

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
#else
bool EnviDSDHT11::readDHTValue()
{
	return (true);
}
#endif
