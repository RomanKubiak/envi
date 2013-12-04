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

class EnviApplication : public MultiTimer
{
	public:
		EnviApplication(int argc, char* argv[]);
		const int runDispatchLoop();
		void cleanupAndExit();
		void addDataSource(EnviDataSource *sourceToAdd);
		void removeDataSource(EnviDataSource *sourceToRemove);
		const int getNumDataSources();
		EnviDataSource *getDataSource(const int index);
		const File getPropertiesFolder();
		PropertySet *getProperties();
		void timerCallback(int timerId);
		bool registerDataSources();
		EnviDataSource *createInstance(const File &sourceState);
		EnviDataSource *createInstance(const ValueTree dataSourceInstance);
		EnviDataSource *registerDataSource(EnviDataSource *dataSource);
		void sourceFailed(EnviDataSource *dataSource);
		void sourceWrite(EnviDataSource *dataSource);
		const var getOption(const Identifier &optionId);

	private:
		PropertySet defaultPropertyStorage;
		ApplicationProperties applicationProperties;
		EnviCLI enviCLI;
		ScopedPointer <EnviDB> enviDB;
		ScopedPointer <EnviHTTP> enviHTTP;
		OwnedArray <EnviDataSource,CriticalSection> dataSources;
};

#endif  // ENVIAPPLICATION_H_INCLUDED
