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
		JUCE_LEAK_DETECTOR(EnviHTTP);

	private:
		EnviHTTPProvider *provider;
		ScopedPointer <StreamingSocket> serverSocket;
		OwnedArray <EnviHTTPConnection> connectionPool;
};

#endif  // ENVIHTTP_H_INCLUDED
