/*
  ==============================================================================

    EnviDSDHT11.h
    Created: 7 Dec 2013 11:40:03pm
    Author:  Administrator

  ==============================================================================
*/

#ifndef ENVIDSDHT11_H_INCLUDED
#define ENVIDSDHT11_H_INCLUDED

#ifdef WIRING_PI

#include "EnviIncludes.h"
#include "EnviDataSource.h"

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DHT_MAXTIMINGS 85

class EnviApplication;

class EnviDSDHT11 : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSDHT11(EnviApplication &_owner);
		~EnviDSDHT11();
		const Result initialize(const ValueTree _instanceConfig);
		const Result execute();
		void run();
		void handleAsyncUpdate();
		bool readDHTValue();

		JUCE_LEAK_DETECTOR(EnviDSDHT11);

	private:
		int dht11_dat[5];
		int gpioPin;
		int iterations;
		int iterationsDelay;
		float temperatureFahrenheit;
		float temperatureCelsius;
		float humidity;
};

#endif
#endif  // ENVIDSDHT11_H_INCLUDED
