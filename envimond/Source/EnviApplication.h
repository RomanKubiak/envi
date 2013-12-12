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
#include "EnviDB.h"
#include "EnviHTTP.h"
#include "EnviWiringPi.h"

class EnviApplication : public MultiTimer
{
	public:
		EnviApplication(int argc, char* argv[]);
		~EnviApplication();

		const Result runDispatchLoop();
		void cleanupAndExit();
		void addDataSource(EnviDataSource *sourceToAdd);
		void removeDataSource(EnviDataSource *sourceToRemove);
		const int getNumDataSources();
		EnviDataSource *getDataSource(const int index);
		const File getPropertiesFolder();
		PropertySet *getProperties();
		void timerCallback(int timerId);
		const Result findDataSourcesOnDisk();
		EnviCLI &getCLI();
		ApplicationProperties &getApplicationProperties();
		EnviDataSource *createInstance(const File &sourceState);
		EnviDataSource *checkForValidInstance(const ValueTree dataSourceInstance);
		EnviDataSource *registerDataSource(EnviDataSource *dataSource);
		void sourceWrite(EnviDataSource *dataSource, const Result &failureReason);
		const var getOption(const Identifier &optionId);
		const bool isValid() { return (valid); }

		JUCE_LEAK_DETECTOR(EnviApplication);

	private:
		PropertySet defaultPropertyStorage;
		ApplicationProperties applicationProperties;
		EnviCLI enviCLI;
		ScopedPointer <EnviDB> enviDB;
		ScopedPointer <EnviHTTP> enviHTTP;
		StringArray allowedSources;
		File dataSourcesDir;
		bool valid;
#ifdef JUCE_LINUX
		ScopedPointer <EnviWiringPi> enviWiringPi;
#endif
		OwnedArray <EnviDataSource,CriticalSection> dataSources;
};

#endif  // ENVIAPPLICATION_H_INCLUDED
