#ifndef ENVIDSDHT22_H_INCLUDED
#define ENVIDSDHT22_H_INCLUDED

#ifdef WIRING_PI

#include "EnviIncludes.h"
#include "EnviDataSource.h"

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DHT_MAXTIMINGS 85

class EnviApplication;

class EnviDSDHT22 : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSDHT22(EnviApplication &_owner);
		~EnviDSDHT22();
		const Result initialize(const ValueTree _instanceConfig);
		const Result execute();
		void run();
		void handleAsyncUpdate();
		bool readDHTValue();

		JUCE_LEAK_DETECTOR(EnviDSDHT22);

	private:
		int dht22_dat[5];
		int gpioPin;
		int iterations;
		int iterationsDelay;
		float temperatureCelsius;
		float humidity;
};

#endif
#endif  // ENVIDSDHT22_H_INCLUDED
