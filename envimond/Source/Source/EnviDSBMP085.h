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
		EnviDSBMP085(EnviApplication &_owner, const ValueTree _instanceConfig);
		~EnviDSBMP085();
		const bool execute();
		const EnviData getResult();
		void run();
		void handleAsyncUpdate();

		JUCE_LEAK_DETECTOR(EnviDSBMP085);

	private:
		EnviData result;
		int i2cAddr;
		int timeout;
};

#endif  // ENVIDSBMP085_H_INCLUDED
