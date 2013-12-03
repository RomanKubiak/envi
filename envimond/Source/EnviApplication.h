/*
  ==============================================================================

    EnviApplication.h
    Created: 2 Dec 2013 1:54:29pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIAPPLICATION_H_INCLUDED
#define ENVIAPPLICATION_H_INCLUDED

#include "EnviCLI.h"
#include "EnviDataSource.h"
#include "EnviScheduler.h"
#include "EnviLog.h"

class SharedMessageThread : public Thread
{
public:
    SharedMessageThread()
      : Thread ("VstMessageThread"),
        initialised (false)
    {
        startThread (7);

        while (! initialised)
            sleep (1);
    }

    ~SharedMessageThread()
    {
        signalThreadShouldExit();
        JUCEApplicationBase::quit();
        waitForThreadToExit (5000);
        clearSingletonInstance();
    }

    void run() override
    {
        initialiseJuce_GUI();
        initialised = true;

        MessageManager::getInstance()->setCurrentThreadAsMessageThread();

        while ((! threadShouldExit()) && MessageManager::getInstance()->runDispatchLoopUntil (250))
        {}
    }

    juce_DeclareSingleton (SharedMessageThread, false);

private:
    bool initialised;
};

class EnviApplication : public MultiTimer
{
	public:
		EnviApplication(int argc, char* argv[]);
		bool messageLoop();
		void run();
		void cleanupAndExit();
		void addDataSource(EnviDataSource *sourceToAdd);
		void removeDataSource(EnviDataSource *sourceToRemove);
		const int getNumDataSources();
		EnviDataSource *getDataSource(const int index);
		const File getPropertiesFolder();
		PropertySet *getProperties();
		void timerCallback(const int timerId);
		bool registerDataSources();
		EnviDataSource *createInstance(const File &sourceState);
		EnviDataSource *createInstance(const ValueTree dataSourceInstance);
		EnviDataSource *registerDataSource(EnviDataSource *dataSource);
	private:
		PropertySet defaultPropertyStorage;
		ApplicationProperties applicationProperties;
		EnviCLI enviCLI;
		ScopedPointer <EnviScheduler> enviScheduler;
		ScopedPointer <EnviLog> enviLog;
		OwnedArray <EnviDataSource,CriticalSection> dataSources;
};

#endif  // ENVIAPPLICATION_H_INCLUDED
