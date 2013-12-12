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

class EnviDataFile
{
	public:
		EnviDataFile (File &_fileToWriteTo, const int _bufferSize = 512);
		virtual ~EnviDataFile();
		Result openDataFile();
		const Result getOpenResult();
		virtual const Result appendData (const EnviData &data) = 0;
		virtual const Result flush() = 0;

	protected:
		Result openResult;
		File &fileToWriteTo;
		int bufferSize;
};

class EnviDataFileCSV : public EnviDataFile
{
	public:
		EnviDataFileCSV(File &fileToWriteTo) : EnviDataFile(fileToWriteTo), dataSize(0)
		{}
		const Result appendData(const EnviData &data);
		const Result flush();
		JUCE_LEAK_DETECTOR(EnviDataFileCSV);

	private:
		int64 dataSize;
		StringArray csvData;
};

class EnviDataFileJSON : public EnviDataFile
{
	public:
		EnviDataFileJSON(File &fileToWriteTo);
		~EnviDataFileJSON();
		const Result appendData(const EnviData &data);
		const Result flush();

		JUCE_LEAK_DETECTOR(EnviDataFileJSON);

	private:
		int64 dataSize;
		Array <EnviData> storedData;
};

class EnviFlatFileStore : public EnviDataStore
{
	public:
		enum FileType
		{
			CSV,
			JSON
		};

		EnviFlatFileStore(EnviApplication &_owner, const FileType _fileType = CSV);
		~EnviFlatFileStore();
		const Result openStore();
		const Result createDataFile();
		const Result closeStore();
        const Result storeData(const EnviData &dataToStore);
        const Result rotate();
		const bool isValid();
		const String getExtension();
		static const FileType getFileType(const String &extensionToCheck);
		const Result flush();
		JUCE_LEAK_DETECTOR(EnviFlatFileStore);

	private:
		File storeFile;
		ScopedPointer <EnviDataFile> enviDataFile;
		FileType fileType;
};

#endif  // ENVIFLATFILESTORE_H_INCLUDED
