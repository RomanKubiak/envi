#ifndef ENVIHTTPFILECACHE_H_INCLUDED
#define ENVIHTTPFILECACHE_H_INCLUDED

#include "EnviIncludes.h"

class EnviHTTPCacheBlobData: public ReferenceCountedObject
{
	public:
		typedef ReferenceCountedObjectPtr<EnviHTTPCacheBlobData> Ptr;
		int64 hashCode;
		NamedValueSet userData;

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnviHTTPCacheBlobData)
};

class EnviHTTPCacheBlob
{
	public:
		EnviHTTPCacheBlob();
		EnviHTTPCacheBlob(const EnviHTTPCacheBlob &otherItem);
		EnviHTTPCacheBlob(const File &file);
		EnviHTTPCacheBlob& operator= (const EnviHTTPCacheBlob&);
		const bool hasHashCode(const int64 hashCode) const noexcept { return data == nullptr ? 0 : data->hashCode; }
		bool isValid() const noexcept 								{ return data != nullptr; }
		int getReferenceCount() const noexcept						{ return data == nullptr ? 0 : data->getReferenceCount(); }
		inline bool isNull() const noexcept                     	{ return data == nullptr; }
		static const EnviHTTPCacheBlob null;
		static EnviHTTPCacheBlob loadFrom (const File &file);
		static EnviHTTPCacheBlob loadFrom (const void *data, const size_t dataSize);

	private:
		ReferenceCountedObjectPtr<EnviHTTPCacheBlobData> data;
		JUCE_LEAK_DETECTOR (EnviHTTPCacheBlob)
};

class EnviHTTPCache
{
	public:
		static EnviHTTPCacheBlob getFromFile (const File& file);
		static EnviHTTPCacheBlob getFromMemory (const void* blobData, int dataSize);
		static EnviHTTPCacheBlob getFromHashCode (int64 hashCode);
		static void addFileToCache (const File& file);
		static void addBlobToCache (const EnviHTTPCacheBlob &blob, const int64 hashCode);
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
