/*
  ==============================================================================

    EnviFlatFileStore.cpp
    Created: 5 Dec 2013 1:29:55pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviFlatFileStore.h"
#include "EnviApplication.h"

/*
 * Generic file type with compression handling
 */
EnviDataFile::EnviDataFile(File &_fileToWriteTo, const int _bufferSize)
	: fileToWriteTo(_fileToWriteTo), openResult(Result::ok()), bufferSize(_bufferSize)
{
}

Result EnviDataFile::openDataFile()
{
	return (openResult = Result::ok());
}

const Result EnviDataFile::getOpenResult()
{
	return (openResult);
}

EnviDataFile::~EnviDataFile()
{
}

/*
 * Specifc file types implementation
 */
const Result EnviDataFileCSV::flush()
{
	_DBG("EnviDataFileCSV::flush");
	dataSize = 0;

	if (!fileToWriteTo.appendText (csvData.joinIntoString(String::empty), false, false))
	{
		csvData.clear();
		return (Result::fail("EnviDataFileCSV::flush failed to write data to disk"));
	}

	csvData.clear();
	return (Result::ok());
}

const Result EnviDataFileCSV::appendData(const var &data)
{
	dataSize += data.getArray()->size();

	if (dataSize >= bufferSize)
	{
		return (flush());
	}

	csvData.add (EnviFlatFileStore::toCSVString(data));
	return (Result::ok());
}

EnviDataFileJSON::EnviDataFileJSON(File &fileToWriteTo) : EnviDataFile(fileToWriteTo), dataSize(0)
{
	if (fileToWriteTo.hasWriteAccess())
	{
		if (fileToWriteTo.getSize() == 0)
		{
			fileToWriteTo.appendText ("[");
		}
	}
}

EnviDataFileJSON::~EnviDataFileJSON()
{
	fileToWriteTo.appendText ("]");
}

const Result EnviDataFileJSON::flush()
{
	_DBG("EnviDataFileJSON::flush");
	dataSize = 0;

	for (int i=0; i<storedData.size(); i++)
	{
		if (!fileToWriteTo.appendText (JSON::toString(storedData[i])))
		{
			storedData.clear();
			return (Result::fail ("EnviDataFileJSON::flush failed to append data to file: ["+fileToWriteTo.getFullPathName()+"]"));
		}
	}

	storedData.clear();
	return (Result::ok());
}

const Result EnviDataFileJSON::appendData(const var &data)
{
	dataSize += data.getArray()->size();

	if (dataSize >= bufferSize)
	{
		return (flush());
	}

	storedData.add (data);

	return (Result::ok());
}


/*
 * File store implementation
 */
EnviFlatFileStore::EnviFlatFileStore(EnviApplication &_owner, const FileType _fileType)
	: EnviDataStore(_owner), fileType(_fileType)
{
}

EnviFlatFileStore::~EnviFlatFileStore()
{
	closeStore();
}

const bool EnviFlatFileStore::isValid()
{
	if (enviDataFile)
	{
		return (enviDataFile->getOpenResult().wasOk());
	}
	else
	{
		return (false);
	}
}

const String EnviFlatFileStore::getExtension()
{
	switch (fileType)
	{
		case JSON:
			return ("json");
		case CSV:
		default:
			return ("csv");
	}
}

const EnviFlatFileStore::FileType EnviFlatFileStore::getFileType(const String &extensionToCheck)
{
	if (extensionToCheck == "json")
		return (EnviFlatFileStore::JSON);
	else
		return (EnviFlatFileStore::CSV);
}

const Result EnviFlatFileStore::openStore()
{
	storeFile = owner.getEnviStoreFile();

	if (!storeFile.existsAsFile())
	{
		if (storeFile.create().failed())
		{
			return (Result::fail ("EnviFlatFileStore::ctor can't create new store file at: ["+storeFile.getFullPathName()+"]"));
		}
		else
		{
			_INF("Data store file set to: "+storeFile.getFullPathName());
			return (createDataFile());
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
			_INF("Data store file set to: "+storeFile.getFullPathName());
			return (createDataFile());
		}
	}
}

const Result EnviFlatFileStore::closeStore()
{
	return (Result::ok());
}

const Result EnviFlatFileStore::storeData(const var &dataToStore)
{
	if (isValid())
	{
		return (enviDataFile->appendData (dataToStore));
	}

	return (Result::fail("EnviFlatFileStore::storeData store is invalid"));
}

const Result EnviFlatFileStore::flush()
{
	return (Result::ok());
}

const Result EnviFlatFileStore::rotate()
{
	_LOG(LOG_INFO, "EnviFlatFileStore::rotate");
	return (Result::ok());
}

const Result EnviFlatFileStore::createDataFile()
{
	if (enviDataFile)
		delete enviDataFile.release();

	switch (fileType)
	{
		case JSON:
			enviDataFile = new EnviDataFileJSON(storeFile);
			break;
		default:
		case CSV:
            enviDataFile = new EnviDataFileCSV(storeFile);
			break;
	}

	return (enviDataFile->openDataFile());
}

const String EnviFlatFileStore::toCSVString(const var &enviValue)
{
	return (String::empty);
}
