/*
  ==============================================================================

    EnviHTTPConnection.h
    Created: 4 Dec 2013 9:31:55pm
    Author:  Administrator

  ==============================================================================
*/

#ifndef ENVIHTTPCONNECTION_H_INCLUDED
#define ENVIHTTPCONNECTION_H_INCLUDED

#include "EnviIncludes.h"
class EnviApplication;
class EnviHTTP;
class EnviHTTPCacheBlob;

class EnviHTTPConnection : public Thread, public ChangeBroadcaster
{
	public:
		EnviHTTPConnection(EnviHTTP &_owner, StreamingSocket *_socket);
		~EnviHTTPConnection();
		void run();
		int writeStringToSocket(StreamingSocket *socket, const String &stringToWrite);
		const bool respond();
		const bool sendResponse(const StringPairArray &responseHeaders, const String &responseBody);
		const bool sendDefaultResponse(const String &message);
		const bool sendStaticResponse(const File &fileToSend);
		const bool sendStatusResponse(const URL &requestURL);
		const bool sendNotFoundResponse();
		const bool sendFile(const File &fileToSend);
		const EnviHTTPMethod getRequestMethod(const String &headers);
		const URL getRequestURL(const EnviHTTPMethod method, const String &headers);
		const File getStaticFileFromURL(const URL &url);
		const bool readRequestData ();
		static const String getMethodName(const EnviHTTPMethod method);
		static const String getStatndardResponseHeaders();
		const bool writeHeadersForBlob (const EnviHTTPCacheBlob &blob);
		const bool writeHeadersForFile(const File &file);
		const bool writeDataFromFile(const File &file);
		const bool writeDataFromBlob(const EnviHTTPCacheBlob &blob);
		const String getHostname();
		const int getPort();
		const EnviHTTPMethod getHTTPMethod();
		const URL getRequestURL();
		const int getResponseCode();
		const int64 getResponseSize();
		JUCE_LEAK_DETECTOR(EnviHTTPConnection);

	private:
		EnviHTTPMethod httpMethod;
		String requestBody, requestHeaders, requestString;
		URL requestURL;
		bool gotRequestHeaders;
		StreamingSocket *socket;
		MemoryBlock requestData;
		EnviHTTP &owner;
		int64 responseSize;
		int responseCode;
};

#endif  // ENVIHTTPCONNECTION_H_INCLUDED
