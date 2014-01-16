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
		const bool sendResponse(const URL &url);
		const bool sendDefaultResponse(const String &message);
		JUCE_LEAK_DETECTOR(EnviHTTPConnection);

	private:
		URL processingUrl;
		bool gotRequestHeaders;
		StreamingSocket *socket;
		EnviHTTP &owner;
};

#endif  // ENVIHTTPCONNECTION_H_INCLUDED
