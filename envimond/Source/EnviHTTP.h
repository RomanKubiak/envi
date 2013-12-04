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

class EnviHTTP : public Thread
{
	public:
		EnviHTTP(EnviApplication &_owner);
		~EnviHTTP();
		void run();
		void processConnection (StreamingSocket *connectedSocket);
		int writeStringToSocket(StreamingSocket *socket, const String &stringToWrite);
	private:
		EnviApplication &owner;
		StreamingSocket serverSocket;
		OwnedArray <EnviHTTPConnection> connectionPool;
};

#endif  // ENVIHTTP_H_INCLUDED
