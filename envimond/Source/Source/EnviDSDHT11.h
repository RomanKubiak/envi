/*
  ==============================================================================

    EnviDSDHT11.h
    Created: 7 Dec 2013 11:40:03pm
    Author:  Administrator

  ==============================================================================
*/

#ifndef ENVIDSDHT11_H_INCLUDED
#define ENVIDSDHT11_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataSource.h"

#ifdef JUCE_LINUX
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#endif
#define DHT_MAXTIMINGS 85
class EnviApplication;

class EnviDSDHT11 : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSDHT11(EnviApplication &_owner, const ValueTree _instanceConfig);
		~EnviDSDHT11();
		const bool execute();
		const EnviData getResult();
		void run();
		void handleAsyncUpdate();
		bool readDHTValue();

		JUCE_LEAK_DETECTOR(EnviDSDHT11);

	private:
		int dht11_dat[5];
		EnviData result;
		int gpioPin;
		int timeout;
		int iterations;
		int iterationsDelay;
		float temperatureFahrenheit;
		float temperatureCelsius;
		float humidity;
};


#endif  // ENVIDSDHT11_H_INCLUDED
