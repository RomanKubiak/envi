#ifdef WIRING_PI

#include "EnviDSDHT22.h"
#include "EnviApplication.h"

EnviDSDHT22::EnviDSDHT22(EnviApplication &owner)
	: Thread("EnviDSDHT22"), gpioPin(-1), EnviDataSource(owner, "dht22")
{
	addValue ("Temperature", Celsius);
	addValue ("Humidity", Percent);
}

EnviDSDHT22::~EnviDSDHT22()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
}

const Result EnviDSDHT22::initialize(const ValueTree _instanceConfig)
{
	EnviDataSource::initialize (_instanceConfig);

	if (instanceConfig.isValid())
	{
		gpioPin			= (int)instanceConfig.hasProperty(Ids::gpioPin)	? (int)getProperty(Ids::gpioPin)		: 5;
		iterationsDelay		= (int)instanceConfig.hasProperty(Ids::delay)		? (int)getProperty(Ids::delay)		: 2000;
		iterations		= (int)instanceConfig.hasProperty(Ids::iterations)	? (int)getProperty(Ids::iterations)	: 4;
	}

	return (Result::ok());
}

const Result EnviDSDHT22::execute()
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

void EnviDSDHT22::run()
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
				_DBG("EnviDSDHT22::run thread signalled to exit");
				return;
			}

		} while (wait (-1));
	}
}

void EnviDSDHT22::handleAsyncUpdate()
{
	collectFinished (Result::ok());
}

bool EnviDSDHT22::readDHTValue()
{
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;

	dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

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
			dht22_dat[j/8] <<= 1;
			if (counter > 16)
				dht22_dat[j/8] |= 1;
			j++;
		}
	}

	// check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
	// print it out if data is good
	if ((j >= 40) && (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) )
	{
		ScopedLock sl (dataSourceLock);
		float t, h;
		h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
		h /= 10;
		t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
		t /= 10.0;
		if ((dht22_dat[2] & 0x80) != 0)
			t *= -1;

		setValue (0, t);
		setValue (1, h);
		_DBG("EnviDSDHT22::readDHTValue got values: "+String::formatted("Humidity: %.2f Temperature: %.2f", h, t));

		return (true);

	}
	else
	{
		_INF("EnviDSDHT22 failed to read value, invalid checksum (retry if iterations set)");
		return (false);
	}

}
#endif
