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

class EnviHTTPConnection : public Thread
{
	public:
		EnviHTTPConnection(EnviHTTP &_owner, StreamingSocket *_socket);
		~EnviHTTPConnection();
		void run();
		int writeStringToSocket(StreamingSocket *socket, const String &stringToWrite);
		const bool getRequestHeaders();
		const bool sendResponse(const StringPairArray &responseHeaders, const String &responseBody);
		const bool sendDefaultResponse(const String &message);
		const bool sendStaticResponse(const File &fileToSend);
		const bool sendFile(const File &fileToSend);
		const EnviHTTPMethod getRequestMethod(const String &headers);
		const URL getRequestURL(const EnviHTTPMethod method, const String &headers);
		const File getStaticFileFromURL(const URL &url);
		const bool readRequestData ();
		static const String getMethodName(const EnviHTTPMethod method);
		static const String getStatndardResponseHeaders();
		JUCE_LEAK_DETECTOR(EnviHTTPConnection);

	private:
		String requestBody, requestHeaders, requestString;
		URL requestURL;
		bool gotRequestHeaders;
		StreamingSocket *socket;
		MemoryBlock requestData;
		EnviHTTP &owner;
};

#endif  // ENVIHTTPCONNECTION_H_INCLUDED
