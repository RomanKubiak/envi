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
#include "Ext/minilzo.h"

#define HEAP_ALLOC(var,size) lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) /sizeof(lzo_align_t) ]
static HEAP_ALLOC(wrkmem,LZO1X_1_MEM_COMPRESS);

class EnviApplication;

class LZOOutputStream : public OutputStream
{
	public:
		LZOOutputStream(OutputStream *_destStream);
        void flush();
        int64 getPosition();
        bool setPosition(int64 position);
        bool write (const void *data, size_t size);
        bool isValid() { return (valid); }
	private:
		bool valid;
		MemoryBlock tempCompressedData;
		OutputStream *destStream;
};

class EnviDataFile
{
	public:
		EnviDataFile (File &_fileToWriteTo, const bool _compressed);
		virtual ~EnviDataFile();
		Result openDataFile();
		const Result getOpenResult();
		virtual const Result appendData (const EnviData &data) = 0;

	protected:
		Result openResult;
		File &fileToWriteTo;
		bool compressed;
		ScopedPointer <LZOOutputStream> compressedOutputStream;
};

class EnviDataFileCSV : public EnviDataFile
{
	public:
		EnviDataFileCSV(File &fileToWriteTo, const bool compressed) : EnviDataFile(fileToWriteTo, compressed)
		{}
		const Result appendData(const EnviData &data);

		JUCE_LEAK_DETECTOR(EnviDataFileCSV);
};

class EnviDataFileXML : public EnviDataFile
{
	public:
		EnviDataFileXML(File &fileToWriteTo, const bool compressed) : EnviDataFile(fileToWriteTo, compressed)
		{}
		const Result appendData(const EnviData &data);

		JUCE_LEAK_DETECTOR(EnviDataFileXML);
};

class EnviDataFileJSON : public EnviDataFile
{
	public:
		EnviDataFileJSON(File &fileToWriteTo, const bool compressed) : EnviDataFile(fileToWriteTo, compressed)
		{}
		const Result appendData(const EnviData &data);

		JUCE_LEAK_DETECTOR(EnviDataFileJSON);
};

class EnviFlatFileStore : public EnviDataStore
{
	public:
		enum FileType
		{
			CSV,
			JSON,
			XML
		};

		EnviFlatFileStore(EnviApplication &_owner, const FileType _fileType = CSV, const bool _compression=false);
		~EnviFlatFileStore();
		const Result openStore();
		const Result createDataFile();
		const Result closeStore();
        const Result storeData(const EnviData &dataToStore);
        const Result rotate();
		int	getNumEntries() const;
		EnviData getEntry(const int entryIndex);
        Array<EnviData> getEntries(const int startIndex, const int numEntries);
        Array<EnviData> getEntries(const Time entryStartTime, const Time entryEndTime);
		const bool isValid();
		const String getExtension();

		JUCE_LEAK_DETECTOR(EnviFlatFileStore);

	private:
		File storeFile;
		ScopedPointer <EnviDataFile> enviDataFile;
		FileType fileType;
		bool compression;
};

#endif  // ENVIFLATFILESTORE_H_INCLUDED
