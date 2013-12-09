/*
  ==============================================================================

    EnviHTTPConnection.cpp
    Created: 4 Dec 2013 9:31:55pm
    Author:  Administrator

  ==============================================================================
*/

#include "EnviHTTPConnection.h"

EnviHTTPConnection::EnviHTTPConnection(StreamingSocket *_socket) 
	: socket(_socket), Thread("EnviHTTPConnection")
{
	_DBG("EnviHTTPConnection::ctor host connected: "+socket->getHostName());
}

EnviHTTPConnection::~EnviHTTPConnection()
{
}

void EnviHTTPConnection::run()
{
	while (1)
	{
		const int ret = socket->waitUntilReady (true, 2500);

		if (threadShouldExit())
		{
			_DBG("EnviHTTPConnection::run thread signalled to exit: "+socket->getHostName());
		}

		if (ret == 1)
		{
			/* normal */
		}
		
		if (ret == 0)
		{
			/* timeout */
		}

		if (ret == -1)
		{
			/* error */
		}
	}
}

int EnviHTTPConnection::writeStringToSocket(StreamingSocket *socket, const String &stringToWrite)
{
	return (socket->write (stringToWrite.toUTF8(), stringToWrite.length()));
}