/*
  ==============================================================================

    EnviFlatFileStore.h
    Created: 5 Dec 2013 1:29:55pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIFLATFILESTORE_H_INCLUDED
#define ENVIFLATFILESTORE_H_INCLUDED

#include "EnviDataStore.h"
class EnviApplication;

class EnviFlatFileStore : public EnviDataStore
{
	public:
		EnviFlatFileStore(EnviApplication &_owner);
		~EnviFlatFileStore();
		const Result openStore();
		const Result closeStore();
        const Result storeData(const EnviData &dataToStore);
        const Result rotate();
		int	getNumEntries() const;
		EnviData getEntry(const int entryIndex);
        Array<EnviData> getEntries(const int startIndex, const int numEntries);
        Array<EnviData> getEntries(const Time entryStartTime, const Time entryEndTime);
		const bool isValid();

		JUCE_LEAK_DETECTOR(EnviFlatFileStore);

	private:
		File storeFile;
};

#endif  // ENVIFLATFILESTORE_H_INCLUDED
