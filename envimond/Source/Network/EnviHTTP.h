/*
  ==============================================================================

    EnviHTTP.h
    Created: 2 Dec 2013 1:39:13pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIHTTP_H_INCLUDED
#define ENVIHTTP_H_INCLUDED

#include "EnviIncludes.h"
#include "Externals/juce_WildcardFileFilter.h"
class EnviApplication;
class EnviHTTPConnection;

class EnviHTTPProvider
{
	public:
		virtual const bool isValidURL (const URL &url)											= 0;
        virtual const Result getResponse (	const URL &requestUrl,
											const EnviHTTPMethod methodUsed,
											const MemoryBlock &requestData,
											const String &requestHeaders,
											const String &requestBody,
											StringPairArray &responseHeaders,
											String &responseData) = 0;
};

class EnviHTTP : public Thread
{
	public:
		EnviHTTP(EnviHTTPProvider *_provider, const int listenPort);
		~EnviHTTP();
		void run();
		void processConnection (StreamingSocket *connectedSocket);
		int writeStringToSocket(StreamingSocket *socket, const String &stringToWrite);
		EnviHTTPProvider *getProvider();
		void setStaticFolder (const String &urlToMap, const File filesystemLocation, const WildcardFileFilter fileWildcard);
		const File isStaticURL(const String &urlToCheck) const;
		JUCE_LEAK_DETECTOR(EnviHTTP);

	private:
		struct StaticMapEntry
		{
			StaticMapEntry(const String _url, const File _filesystemLocation, const WildcardFileFilter _fileFilter)
				: url(_url), filesystemLocation(_filesystemLocation), fileFilter(_fileFilter) {}
			StaticMapEntry(const StaticMapEntry &other)
				: url(other.url), filesystemLocation(other.filesystemLocation), fileFilter(other.fileFilter) {}
			StaticMapEntry()
				: fileFilter(String::empty, String::empty, String::empty) {}
			String				url;
			File 				filesystemLocation;
			WildcardFileFilter 	fileFilter;
		};
		Array <StaticMapEntry,CriticalSection> staticUrlMap;
		EnviHTTPProvider *provider;
		ScopedPointer <StreamingSocket> serverSocket;
		OwnedArray <EnviHTTPConnection> connectionPool;
};

#endif  // ENVIHTTP_H_INCLUDED
