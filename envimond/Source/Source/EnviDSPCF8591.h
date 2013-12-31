#ifndef ENVIDSPCF8591_H_INCLUDED
#define ENVIDSPCF8591_H_INCLUDED

#ifdef WIRING_PI

#include "EnviIncludes.h"
#include "EnviDataSource.h"

#include <wiringPi.h>
#include <pcf8591.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DHT_MAXTIMINGS 85

class EnviApplication;

class EnviDSPCF8591 : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSPCF8591(EnviApplication &_owner);
		~EnviDSPCF8591();
		const Result initialize(const ValueTree _instanceConfig);
		const Result execute();
		void run();
		void handleAsyncUpdate();
		bool readPCFvalues();
		JUCE_LEAK_DETECTOR(EnviDSPCF8591);

	private:
		int i2cAddr;
		int pinBase;
		int pcfFd;
		int pinMap[4];
};


#endif
#endif  // ENVIDSPCF8591_H_INCLUDED
