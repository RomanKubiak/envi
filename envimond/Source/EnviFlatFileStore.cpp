/*
  ==============================================================================

    EnviFlatFileStore.cpp
    Created: 5 Dec 2013 1:29:55pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviFlatFileStore.h"
#include "EnviApplication.h"

LZOOutputStream::LZOOutputStream(OutputStream *_destStream)
	: destStream(_destStream), tempCompressedData(8192,true), valid(true)
{
	if (lzo_init() != LZO_E_OK)
	{
		valid = false;
		// lzo_init failed
		jassertfalse;
	}
}

void LZOOutputStream::flush()
{
}

int64 LZOOutputStream::getPosition()
{
	return destStream->getPosition();
}

bool LZOOutputStream::setPosition(int64 position)
{
	jassertfalse; // can't do it!
    return false;
}

bool LZOOutputStream::write (const void *data, size_t size)
{
	lzo_uint compressedSize;
	const int r = lzo1x_1_compress ((unsigned char *)data, size, (unsigned char *)tempCompressedData.getData(), &compressedSize, wrkmem);

	if (r != LZO_E_OK)
	{
		_DBG("LZOOutputStream::write lzo1x_1_compress failed");
		return (false);
	}
	else
	{
		_DBG("LZOOutputStream::write lzo1x_1_compress success, data size: "+_STR(size)+", compressed size: "+_STR((int)compressedSize));
        return (destStream->write (tempCompressedData.getData(), compressedSize));
	}
}

/*
 * Generic file type with compression handling
 */
EnviDataFile::EnviDataFile(File &_fileToWriteTo, const bool _compressed)
	: fileToWriteTo(_fileToWriteTo), compressed(_compressed), openResult(Result::ok())
{
}

Result EnviDataFile::openDataFile()
{
	if (compressed)
	{
		compressedOutputStream = new LZOOutputStream(fileToWriteTo.createOutputStream());
		if (compressedOutputStream->isValid())
		{
			return (openResult = Result::ok());
		}
		else
		{
			return (Result::fail("EnviDataFile::openDataFile can't initialize LZO output stream"));
		}
	}
	else
	{
		return (openResult = Result::ok());
	}
}

const Result EnviDataFile::getOpenResult()
{
	return (openResult);
}

EnviDataFile::~EnviDataFile()
{
	if (compressedOutputStream)
	{
		compressedOutputStream->flush();
	}
}

/*
 * Specifc file types implementation
 */
const Result EnviDataFileCSV::appendData(const EnviData &data)
{
	const String stringToWrite = EnviData::toCSVString(data);
    if (compressed && compressedOutputStream)
	{
		if (compressedOutputStream->writeString (stringToWrite))
		{
			return (Result::ok());
		}
		else
		{
			return (Result::fail("EnviDataFileCSV::appendData writeToString failed on output stream"));
		}
	}
	else
	{
		_DBG("EnviDataFileCSV::appendData writing to output file");
		if (fileToWriteTo.appendText (stringToWrite, false, false))
		{
			return (Result::ok());
		}
		else
		{
			return (Result::fail("EnviDataFileCSV::appendData Can't append data to CSV file: ["+fileToWriteTo.getFullPathName()+"]"));
		}
	}
	return (Result::ok());
}

const Result EnviDataFileJSON::appendData(const EnviData &data)
{
	return (Result::ok());
}

const Result EnviDataFileXML::appendData(const EnviData &data)
{
	return (Result::ok());
}

/*
 * File store implementation
 */
EnviFlatFileStore::EnviFlatFileStore(EnviApplication &_owner, const FileType _fileType, const bool _compression)
	: EnviDataStore(_owner), fileType(_fileType), compression(_compression)
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
			return (".json" + (compression ? ".z" : String::empty));
		case XML:
			return (".xml" + (compression ? ".z" : String::empty));
		case CSV:
		default:
			return (".csv" + (compression ? ".z" : String::empty));
	}
}

const Result EnviFlatFileStore::openStore()
{
	_DBG("EnviFlatFileStore::openStore");
	if (owner.getOption(Ids::storeFile).toString().isEmpty())
	{
		storeFile = owner.getPropertiesFolder().getChildFile("storage"+getExtension());
	}
	else
	{
		storeFile = File (owner.getOption(Ids::storeFile));
	}

	_DBG("EnviFlatFileStore::openStore file: ["+storeFile.getFullPathName()+"]");

	if (!storeFile.existsAsFile())
	{
		if (storeFile.create().failed())
		{
			return (Result::fail ("EnviFlatFileStore::ctor can't create new store file at: ["+storeFile.getFullPathName()+"]"));
		}
		else
		{
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
			return (createDataFile());
		}
	}
}

const Result EnviFlatFileStore::closeStore()
{
	return (Result::ok());
}

const Result EnviFlatFileStore::storeData(const EnviData &dataToStore)
{
	if (isValid())
	{
		return (enviDataFile->appendData (dataToStore));
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

const Result EnviFlatFileStore::createDataFile()
{
	if (enviDataFile)
		delete enviDataFile.release();

	switch (fileType)
	{
		case XML:
			enviDataFile = new EnviDataFileXML(storeFile, compression);
			break;
		case JSON:
			enviDataFile = new EnviDataFileJSON(storeFile, compression);
			break;
		default:
		case CSV:
            enviDataFile = new EnviDataFileCSV(storeFile, compression);
			break;
	}

	return (enviDataFile->openDataFile());
}
