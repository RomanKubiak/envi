/*
  ==============================================================================

    EnviDB.h
    Created: 3 Dec 2013 8:44:34pm
    Author:  Administrator

  ==============================================================================
*/

#ifndef ENVIDB_H_INCLUDED
#define ENVIDB_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataSource.h"
#include "EnviFlatFileStore.h"
#include "EnviSqlite3Store.h"
class EnviApplication;

class EnviDB : public Thread
{
	public:
		EnviDB(EnviApplication &_owner);
		const bool init();
		~EnviDB();
		void run();
		void writeResult(EnviDataSource *dataSource);

		JUCE_LEAK_DETECTOR(EnviDB)

	private:
		// ScopedPointer <EnviFlatFileStore> enviStore;
		ScopedPointer <EnviSqlite3Store> enviStore;
		EnviApplication &owner;
		OwnedArray <EnviData, CriticalSection> dataQueue;
};


#endif  // ENVIDB_H_INCLUDED
