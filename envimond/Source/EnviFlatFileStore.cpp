/*
  ==============================================================================

    EnviFlatFileStore.cpp
    Created: 5 Dec 2013 1:29:55pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviFlatFileStore.h"
#include "EnviApplication.h"

EnviFlatFileStore::EnviFlatFileStore(EnviApplication &_owner)
	: EnviDataStore(_owner)
{
}

EnviFlatFileStore::~EnviFlatFileStore()
{
	closeStore();
}

const bool EnviFlatFileStore::isValid()
{
	return (storeFile.existsAsFile() && storeFile.hasWriteAccess());
}

const Result EnviFlatFileStore::openStore()
{
	if (owner.getOption(Ids::storeFile).toString().isEmpty())
	{
		storeFile = owner.getPropertiesFolder().getChildFile("storage.dat");
	}
	else
	{
		storeFile = File (owner.getOption(Ids::storeFile));
	}

	if (!storeFile.existsAsFile())
	{
		if (storeFile.create().failed())
		{
			return (Result::fail ("EnviFlatFileStore::ctor can't create new store file at: ["+storeFile.getFullPathName()+"]"));
		}
		else
		{
			_DBG("EnviFlatFileStore::ctor store file created: ["+storeFile.getFullPathName()+"]");
			return (Result::ok());
		}
	}
	else
	{
		if (!storeFile.hasWriteAccess())
		{
			return (Result::fail("EnviFlatFileStore::ctor store file exists but is not writable"));
		}
		else
		{
			_DBG("EnviFlatFileStore::ctor store file already exists: ["+storeFile.getFullPathName()+"]");
			return (Result::ok());
		}
	}
}

const Result EnviFlatFileStore::closeStore()
{
	return (Result::ok());
}

const Result EnviFlatFileStore::storeData(const EnviData &dataToStore)
{
	_DBG("EnviFlatFileStore::storeData");
	_DBG("\t["+EnviData::toCSVString(dataToStore)+"]");
	if (isValid())
	{
		if (storeFile.appendText (EnviData::toCSVString(dataToStore), false, false))
		{
			return (Result::ok());
		}
		else
		{
			return (Result::fail("EnviFlatFileStore::storeData appendText failed"));
		}
	}

	return (Result::fail("EnviFlatFileStore::storeData store is invalid"));
}

const Result EnviFlatFileStore::rotate()
{
	return (Result::ok());
}

int	EnviFlatFileStore::getNumEntries() const
{
	return (0);
}

EnviData EnviFlatFileStore::getEntry(const int entryIndex)
{
	return (EnviData());
}

Array<EnviData> EnviFlatFileStore::getEntries(const int startIndex, const int numEntries)
{
	Array<EnviData> result;
	return (result);
}

Array<EnviData> EnviFlatFileStore::getEntries(const Time entryStartTime, const Time entryEndTime)
{
	Array<EnviData> result;
	return (result);
}
