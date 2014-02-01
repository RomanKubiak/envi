/*
  ==============================================================================

    EnviHTTP.h
    Created: 2 Dec 2013 1:39:13pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIHTTP_H_INCLUDED
#define ENVIHTTP_H_INCLUDED

#include "EnviJSONRPC.h"
#include "EnviHTTPCache.h"
#include <functional>

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

class EnviHTTP : public Thread, public ChangeListener
{
	public:
		EnviHTTP(EnviHTTPProvider *_provider, const int listenPort);
		~EnviHTTP();
		void run();
		void setMimeTypes (const char *mimeTypeData, const int mimeTypeDataSize);
		const String getMimeTypeFor(const File &fileToCheck);
		void processConnection (StreamingSocket *connectedSocket);
		int writeStringToSocket(StreamingSocket *socket, const String &stringToWrite);
		EnviHTTPProvider *getProvider();
		int getNumClientConnections();
		void setStaticFolder (const String &urlToMap, const File filesystemLocation, const WildcardFileFilter fileWildcard);
		const File isStaticURL(const String &urlToCheck) const;
		void changeListenerCallback (ChangeBroadcaster* source);
		const bool canCache(const File &fileToCheck) const;
		void setAccessLog (const File &fileToLogTo);
		void setErrorLog (const File &fileToLogTo);
		void logError(EnviHTTPConnection *source, const String &messageIfAny=String::empty);
		void logAccess(EnviHTTPConnection *source, const String &messageIfAny=String::empty);

		const Result processCoreRPC (EnviJSONRPC &request);
		const Result systemStatus(EnviJSONRPC &request);
		const Result serverStatus(EnviJSONRPC &request);
		const Result ping(EnviJSONRPC &request);

		static const String getLogTimestamp();
		static const String getOsType(const SystemStats::OperatingSystemType type);
		static const StringPairArray getSystemStats();

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
		OwnedArray <EnviHTTPConnection,CriticalSection> connectionPool;
		HashMap<String,String,DefaultHashFunctions,CriticalSection> mimeTypes;
		ScopedPointer <FileLogger> accessLog, errorLog;
		HashMap<String, std::function<const Result(EnviJSONRPC &)> > coreMethods;
};

#endif  // ENVIHTTP_H_INCLUDED
