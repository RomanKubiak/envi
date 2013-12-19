/*
  ==============================================================================

    EnviHTTPConnection.cpp
    Created: 4 Dec 2013 9:31:55pm
    Author:  Administrator

  ==============================================================================
*/

#include "EnviHTTPConnection.h"
#include "EnviHTTP.h"
#include "EnviApplication.h"

EnviHTTPConnection::EnviHTTPConnection(EnviHTTP &_owner, StreamingSocket *_socket)
	: socket(_socket), Thread("EnviHTTPConnection"), gotRequestHeaders(false), owner(_owner)
{
	_DBG("EnviHTTPConnection::ctor host connected: "+socket->getHostName());

	startThread();
}

EnviHTTPConnection::~EnviHTTPConnection()
{
}

void EnviHTTPConnection::run()
{
	_DBG("EnviHTTPConnection::run");
	while (1)
	{
		const int ret = socket->waitUntilReady (true, 2500);

		_DBG("\twaitUntilReady returned: "+_STR(ret));

		if (threadShouldExit())
		{
			_DBG("EnviHTTPConnection::run thread signalled to exit: "+socket->getHostName());
			return;
		}

		if (ret == 1)
		{
			/* normal */
			if (getRequestHeaders())
			{
				sendResponse();
			}
		}
		else if (ret == 0)
		{
			/* timeout */
		}
		else if (ret == -1)
		{
			/* error */
		}

		_DBG("EnviHTTPConnection finished");
		return;
	}
}

int EnviHTTPConnection::writeStringToSocket(StreamingSocket *socket, const String &stringToWrite)
{
	return (socket->write (stringToWrite.toUTF8(), stringToWrite.length()));
}

const bool EnviHTTPConnection::getRequestHeaders()
{
	MemoryBlock readBuffer(8192);
	if (socket->read (readBuffer.getData(), 8192, false))
	{
		String request = readBuffer.toString();
		if (request.startsWith("GET"))
		{
			processingUrl = request.fromFirstOccurrenceOf("GET ", false, true).upToFirstOccurrenceOf("HTTP/",false,false);
			return (gotRequestHeaders = true);
		}
		else
		{
			_DBG("EnviHTTPConnection can't process non GET requests");
			return (false);
		}
	}
	else
	{
		_DBG("EnviHTTPConnection read failed");
		return (false);
	}
}

const bool EnviHTTPConnection::sendResponse()
{
	_DBG("EnviHTTPConnection::sendResponse");
	_DBG(processingUrl.toString(true));

	var ret;

	for (int i=0; i<owner.getOwner().getNumDataSources(); i++)
	{
        EnviDataSource *ds = owner.getOwner().getDataSource(i);

        if (ds != nullptr)
		{
			ret.append (ds->getSummary());
		}
	}

	const String summaryString = JSON::toString (ret);

	writeStringToSocket(socket, "HTTP/1.1 200 OK\nServer: Envimond\nContent-Length: "+_STR(summaryString.length())+"\nContent-type: text/html; charset=UTF-8\nConnection: close\n\n"+summaryString);
	return (true);
}
