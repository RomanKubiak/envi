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

class EnviHTTPConnection : public Thread
{
	public:
		EnviHTTPConnection(StreamingSocket *_socket);
		~EnviHTTPConnection();
		void run();
		int writeStringToSocket(StreamingSocket *socket, const String &stringToWrite);

		JUCE_LEAK_DETECTOR(EnviHTTPConnection);

	private:
		URL processingUrl;
		StreamingSocket *socket;
};

#endif  // ENVIHTTPCONNECTION_H_INCLUDED
