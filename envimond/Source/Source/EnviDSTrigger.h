/*
  ==============================================================================

    EnviDSTrigger.h
    Created: 23 Dec 2013 5:11:41pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDSTRIGGER_H_INCLUDED
#define ENVIDSTRIGGER_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataSource.h"
class EnviApplication;

class EnviDSTrigger : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSTrigger(EnviApplication &_owner);
		~EnviDSTrigger();
		const Result initialize(const ValueTree _instanceConfig);
		const Result execute();
		void run();
		void handleAsyncUpdate();

		JUCE_LEAK_DETECTOR(EnviDSTrigger);
};

#endif  // ENVIDSTRIGGER_H_INCLUDED
