#include "EnviHTTPFileCache.h"

const EnviHTTPCacheBlob EnviHTTPCacheBlob::null;

class EnviHTTPCache::Pimpl     : private Timer, private DeletedAtShutdown
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

    void addImageToCache (const EnviHTTPCacheBlob& blob, const int64 hashCode)
    {
        if (blob.isValid())
        {
            if (! isTimerRunning())
                startTimer (2000);

            Item* const item = new Item();
            item->hashCode = hashCode;
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

void EnviHTTPCache::addBlobToCache (const EnviHTTPCacheBlob& blob, const int64 hashCode)
{
    Pimpl::getInstance()->addImageToCache (blob, hashCode);
}

EnviHTTPCacheBlob EnviHTTPCache::getFromFile (const File& file)
{
    const int64 hashCode = file.hashCode64();
    EnviHTTPCacheBlob blob (getFromHashCode (hashCode));

    if (blob.isNull())
    {
        blob = EnviHTTPCacheBlob::loadFrom (file);
        addBlobToCache (blob, hashCode);
    }

    return blob;
}

EnviHTTPCacheBlob EnviHTTPCache::getFromMemory (const void* blobData, const int dataSize)
{
    const int64 hashCode = (int64) (pointer_sized_int) blobData;
    EnviHTTPCacheBlob blob (getFromHashCode (hashCode));

    if (blob.isNull())
    {
        blob = EnviHTTPCacheBlob::loadFrom (blobData, (size_t) dataSize);
        addBlobToCache (blob, hashCode);
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
