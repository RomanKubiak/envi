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

#ifdef JUCE_LINUX
#include "EnviWiringPi.h"
#endif

class EnviApplication : public MultiTimer
{
	public:
		EnviApplication(int argc, char* argv[]);
		~EnviApplication();

		const Result runDispatchLoop();
		void cleanupAndExit();
		void removeDataSource(EnviDataSource *sourceToRemove);
		const int getNumDataSources();
		EnviDataSource *getDataSource(const int index);
		const File getPropertiesFolder();
		PropertySet *getProperties();
		void timerCallback(int timerId);
		const Result findDataSourcesOnDisk();
		EnviCLI &getCLI();
		EnviDataSource *createInstance(const File &sourceState);
		EnviDataSource *createInstance(const ValueTree dataSourceInstance);
		void sourceWrite(EnviDataSource *dataSource, const Result &failureReason);
		const int getNumInstances(const Identifier dsType);
		EnviDataSource *getInstanceFromType(const Identifier dsType);
		const bool isValid();
		const File getEnviSourcesDir();
		const File getEnviScriptsDir();
		const File getEnviStoreFile();
		const File getEnviLogFile();
		JUCE_LEAK_DETECTOR(EnviApplication);

	private:
		EnviCLI enviCLI;
		ScopedPointer <EnviDB> enviDB;
		ScopedPointer <EnviHTTP> enviHTTP;
		StringArray disabledSources;
		File dataSourcesDir;
		bool valid;
#ifdef JUCE_LINUX
		ScopedPointer <EnviWiringPi> enviWiringPi;
#endif
		OwnedArray <EnviDataSource,CriticalSection> dataSources;
};

#endif  // ENVIAPPLICATION_H_INCLUDED
