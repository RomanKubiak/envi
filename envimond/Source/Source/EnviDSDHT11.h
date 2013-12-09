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
		EnviDSDHT11(EnviApplication &owner, const ValueTree instanceConfig);
		~EnviDSDHT11();
		const String getName();
		const int getInterval();
		const int getTimeout();
		const bool execute();
		const EnviData getResult();
		const var getProperty (const Identifier &identifier);
		void run();
		void handleAsyncUpdate();
		bool readDHTValue();
		JUCE_LEAK_DETECTOR(EnviDSDHT11);

	private:
		int dht11_dat[5];
		EnviData result;
		ValueTree instanceState;
		int gpioPin;
		int timeout;
		int iterations;
		int iterationsDelay;
		float temperatureFahrenheit;
		float temperatureCelsius;
		float humidity;
};


#endif  // ENVIDSDHT11_H_INCLUDED
