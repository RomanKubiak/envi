/*
  ==============================================================================

    EnviDSBMP085.h
    Created: 7 Dec 2013 11:40:35pm
    Author:  Administrator

  ==============================================================================
*/

#ifndef ENVIDSBMP085_H_INCLUDED
#define ENVIDSBMP085_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataSource.h"
class EnviApplication;

class EnviDSBMP085 : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSBMP085(EnviApplication &owner, const ValueTree instanceConfig);
		~EnviDSBMP085();
		const String getName();
		const int getInterval();
		const int getTimeout();
		const bool execute();
		const EnviData getResult();
		const var getProperty (const Identifier &identifier);
		void run();
		void handleAsyncUpdate();

		JUCE_LEAK_DETECTOR(EnviDSBMP085);

	private:
		EnviData result;
		ValueTree instanceState;
		int i2cAddr;
		int timeout;
};

#endif  // ENVIDSBMP085_H_INCLUDED
