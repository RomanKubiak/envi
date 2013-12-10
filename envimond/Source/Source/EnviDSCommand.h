/*
  ==============================================================================

    EnviDSCommand.h
    Created: 3 Dec 2013 12:32:39pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDSCOMMAND_H_INCLUDED
#define ENVIDSCOMMAND_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataSource.h"
class EnviApplication;

class EnviDSCommand : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSCommand(EnviApplication &_owner, const ValueTree _instanceConfig);
		~EnviDSCommand();
		const String getName();
		const int getInterval();
		const int getTimeout();
		const bool execute();
		const EnviData getResult();
		void run();
		void handleAsyncUpdate();

		JUCE_LEAK_DETECTOR(EnviDSCommand);

	private:
		EnviData result;
		String commandOutput;
		int timeout;
		String cmd;
};

#endif  // ENVIDSCOMMAND_H_INCLUDED
