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
		EnviDSCommand(EnviApplication &_owner);
		~EnviDSCommand();
		const Result initialize(const ValueTree _instanceConfig);
		const Result execute();
		void run();
		void handleAsyncUpdate();
		void processCommandOutput(const String _commandOutput);
		JUCE_LEAK_DETECTOR(EnviDSCommand);

	private:
		StringArray command;
		String commandOutput;
		CriticalSection safeResultLock;
		HashMap<String,Expression> valueExpressions;
};

#endif  // ENVIDSCOMMAND_H_INCLUDED
