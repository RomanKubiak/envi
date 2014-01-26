#ifndef ENVIHTTPFILECACHE_H_INCLUDED
#define ENVIHTTPFILECACHE_H_INCLUDED

#include "EnviIncludes.h"

class EnviHTTPCacheBlobData: public ReferenceCountedObject
{
	public:
		EnviHTTPCacheBlobData(const int64 _size = 0, const int64 _hashCode = 0) : hashCode(_hashCode), size(_size)
		{
			blobData.allocate (size,false);
		}

		typedef ReferenceCountedObjectPtr<EnviHTTPCacheBlobData> Ptr;

		int64 hashCode;
		int64 size;

		void copyFromDisk(const File &sourceData)
		{			
			ScopedPointer <FileInputStream> fis(sourceData.createInputStream());
			MemoryOutputStream mo(blobData.getData(), sourceData.getSize());
			
			if (fis != nullptr)
			{
				if (mo.writeFromInputStream (*fis, sourceData.getSize()) != sourceData.getSize())
					jassertfalse;
			}
		}

		void copyFromMemory(const void *data, const size_t dataSize)
		{
			memcpy (blobData.getData(), data, dataSize);
		}

		void *getData() { return (blobData.getData()); }
	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnviHTTPCacheBlobData)
		HeapBlock<uint8> blobData;
};

class EnviHTTPCacheBlob
{
	public:
		EnviHTTPCacheBlob();
		EnviHTTPCacheBlob(const int64 _size, const int64 _hashCode, const String _mime);
		EnviHTTPCacheBlob(const EnviHTTPCacheBlob &other);
		EnviHTTPCacheBlob& operator= (const EnviHTTPCacheBlob&);
		static EnviHTTPCacheBlob loadFrom (const File &file, const String &mime);
		static EnviHTTPCacheBlob loadFrom (const void *data, const size_t dataSize, const String &mime);

		const bool hasHashCode(const int64 hashCode) const noexcept { return data == nullptr ? 0 : data->hashCode; }
		bool isValid() const noexcept 								{ return data != nullptr; }
		int getReferenceCount() const noexcept						{ return data == nullptr ? 0 : data->getReferenceCount(); }
		inline bool isNull() const noexcept                     	{ return data == nullptr; }
		EnviHTTPCacheBlobData* getData() const noexcept				{ return data; }
		const int64 getSize() const noexcept						{ return size; }
		const int64 hashCode64() const noexcept						{ return hash; }
		const String getMime() const noexcept						{ return mime; }
		InputStream *createInputStream() const;
		static const EnviHTTPCacheBlob null;

	private:
		ReferenceCountedObjectPtr<EnviHTTPCacheBlobData> data;
		int64 hash, size;
		String mime;
		JUCE_LEAK_DETECTOR (EnviHTTPCacheBlob)
};

class EnviHTTPCache
{
	public:
		static EnviHTTPCacheBlob getFromFile (const File& file, const String &mime);
		static EnviHTTPCacheBlob getFromMemory (const void* blobData, int dataSize, const String &mime);
		static EnviHTTPCacheBlob getFromHashCode (int64 hashCode);
		static void addFileToCache (const File& file);
		static void addBlobToCache (const EnviHTTPCacheBlob &blob);
		static void setCacheTimeout (int millisecs);
		static void releaseUnusedBlobs();

	private:
		class Pimpl;
		friend class Pimpl;
		EnviHTTPCache();
		~EnviHTTPCache();

		JUCE_DECLARE_NON_COPYABLE (EnviHTTPCache)
};

#endif
