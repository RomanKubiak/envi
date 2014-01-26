#include "EnviHTTPCache.h"

const EnviHTTPCacheBlob EnviHTTPCacheBlob::null;

class EnviHTTPCache::Pimpl : private Timer, private DeletedAtShutdown
{
public:
    Pimpl()  : cacheTimeout (5000)
    {
    }

    ~Pimpl()
    {
        clearSingletonInstance();
    }

    EnviHTTPCacheBlob getFromHashCode (const int64 hashCode)
    {
        const ScopedLock sl (lock);

        for (int i = blobs.size(); --i >= 0;)
        {
            const Item* const item = blobs.getUnchecked(i);

            if (item->hashCode == hashCode)
                return item->blob;
        }

        return EnviHTTPCacheBlob::null;
    }

    void addBlobToCache (const EnviHTTPCacheBlob& blob)
    {
        if (blob.isValid())
        {
            if (! isTimerRunning())
                startTimer (2000);

            Item* const item = new Item();
			item->hashCode = blob.hashCode64();
            item->blob = blob;
            item->lastUseTime = Time::getApproximateMillisecondCounter();

            const ScopedLock sl (lock);
            blobs.add (item);
        }
    }

    void timerCallback() override
    {
        const uint32 now = Time::getApproximateMillisecondCounter();

        const ScopedLock sl (lock);

        for (int i = blobs.size(); --i >= 0;)
        {
            Item* const item = blobs.getUnchecked(i);

            if (item->blob.getReferenceCount() <= 1)
            {
                if (now > item->lastUseTime + cacheTimeout || now < item->lastUseTime - 1000)
                    blobs.remove (i);
            }
            else
            {
                item->lastUseTime = now; // multiply-referenced, so this image is still in use.
            }
        }

        if (blobs.size() == 0)
            stopTimer();
    }

    void releaseUnusedBlobs()
    {
        const ScopedLock sl (lock);

        for (int i = blobs.size(); --i >= 0;)
            if (blobs.getUnchecked(i)->blob.getReferenceCount() <= 1)
                blobs.remove (i);
    }

    struct Item
    {
        EnviHTTPCacheBlob blob;
        int64 hashCode;
        uint32 lastUseTime;
    };

    unsigned int cacheTimeout;

    juce_DeclareSingleton_SingleThreaded_Minimal (EnviHTTPCache::Pimpl);

private:
    OwnedArray<Item> blobs;
    CriticalSection lock;

    JUCE_DECLARE_NON_COPYABLE (Pimpl)
};

juce_ImplementSingleton_SingleThreaded (EnviHTTPCache::Pimpl);


//==============================================================================
EnviHTTPCacheBlob EnviHTTPCache::getFromHashCode (const int64 hashCode)
{
    if (Pimpl::getInstanceWithoutCreating() != nullptr)
        return Pimpl::getInstanceWithoutCreating()->getFromHashCode (hashCode);

    return EnviHTTPCacheBlob::null;
}

void EnviHTTPCache::addBlobToCache (const EnviHTTPCacheBlob& blob)
{
    Pimpl::getInstance()->addBlobToCache (blob);
}

EnviHTTPCacheBlob EnviHTTPCache::getFromFile (const File& file, const String &mime)
{
	EnviHTTPCacheBlob blob (getFromHashCode(file.hashCode64()));

    if (blob.isNull())
    {
        blob = EnviHTTPCacheBlob::loadFrom (file,mime);
        addBlobToCache (blob);
    }

    return blob;
}

EnviHTTPCacheBlob EnviHTTPCache::getFromMemory (const void* blobData, const int dataSize, const String &mime)
{
	const int64 hashCode = (int64) (pointer_sized_int) blobData;
    EnviHTTPCacheBlob blob (getFromHashCode(hashCode));

    if (blob.isNull())
    {
        blob = EnviHTTPCacheBlob::loadFrom (blobData, (size_t) dataSize, mime);
        addBlobToCache (blob);
    }

    return blob;
}

void EnviHTTPCache::setCacheTimeout (const int millisecs)
{
    jassert (millisecs >= 0);
    Pimpl::getInstance()->cacheTimeout = (unsigned int) millisecs;
}

void EnviHTTPCache::releaseUnusedBlobs()
{
    Pimpl::getInstance()->releaseUnusedBlobs();
}

EnviHTTPCacheBlob::EnviHTTPCacheBlob()
{
}

EnviHTTPCacheBlob::EnviHTTPCacheBlob(const int64 _size, const int64 _hash, const String _mime) 
	: size(_size), hash(_hash), mime(_mime), data(new EnviHTTPCacheBlobData(_size, _hash))
{
}

EnviHTTPCacheBlob::EnviHTTPCacheBlob(const EnviHTTPCacheBlob &other) 
	: data(other.data), size(other.size), hash(other.hash), mime(other.mime)
{
}

EnviHTTPCacheBlob& EnviHTTPCacheBlob::operator= (const EnviHTTPCacheBlob &other)
{
	data	= other.data;
	size	= other.size;
	hash	= other.hash;
	mime	= other.mime;
    return *this;
}

InputStream *EnviHTTPCacheBlob::createInputStream() const
{
	return (new MemoryInputStream (data->getData(), size, false));
}

EnviHTTPCacheBlob EnviHTTPCacheBlob::loadFrom (const File &file, const String &mime)
{
	EnviHTTPCacheBlob blob (file.getSize(), file.hashCode64(), mime);
	blob.getData()->copyFromDisk(file);
	return (blob);
}

EnviHTTPCacheBlob EnviHTTPCacheBlob::loadFrom (const void *data, const size_t dataSize, const String &mime)
{
	EnviHTTPCacheBlob blob (dataSize, (int64) (pointer_sized_int) data, mime);
	blob.getData()->copyFromMemory(data,dataSize);
	return (blob);
}