/*
  ==============================================================================

    EnviIPCClient.h
    Created: 14 Jan 2014 1:09:10pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIIPCCLIENT_H_INCLUDED
#define ENVIIPCCLIENT_H_INCLUDED

#include "EnviUIIncludes.h"
class EnviUIMain;
class EnviIPCClient;

class EnviIPCConnectionChecker : public Thread
{
	public:
		EnviIPCConnectionChecker(EnviIPCClient &_owner);
		~EnviIPCConnectionChecker();
		void run();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnviIPCConnectionChecker)

	private:
		EnviIPCClient &owner;
};

class EnviIPCClient : public InterprocessConnection, public AsyncUpdater
{
	public:
		EnviIPCClient(EnviUIMain &_owner);
		~EnviIPCClient();
		void connectionMade();
		void connectionLost();
		void messageReceived (const MemoryBlock& message);
		void handleAsyncUpdate();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnviIPCClient)

	private:
		ScopedPointer <EnviIPCConnectionChecker> enviIPCConnectionChecker;
		EnviUIMain &owner;
};

#endif  // ENVIIPCCLIENT_H_INCLUDED
