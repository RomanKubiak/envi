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
				return;
			}
			else
			{
				_WRN("\tgetRequestHeaders failed");
			}
		}
		else if (ret == 0)
		{
			/* timeout */
			_WRN("\ttimeout");
		}
		else if (ret == -1)
		{
			/* error */
			_WRN("\terror");
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
	_DBG("EnviHTTPConnection::getRequestHeaders");

	MemoryBlock readBuffer(8192, true);
	if (socket->read (readBuffer.getData(), 8192, false))
	{
		if (readBuffer.getSize() > 0)
		{
			// _DBG("EnviHTTPConnection::getRequestHeaders");
			// _DBG("\t"+String::toHexString (readBuffer.getData(), readBuffer.getSize()));
			String request = readBuffer.toString();
			if (request.startsWith("GET"))
			{
				processingUrl = request.fromFirstOccurrenceOf("GET ", false, true).upToFirstOccurrenceOf("HTTP/",false,false);

				if (owner.getProvider() && owner.getProvider()->isValidURL(processingUrl))
					return (sendResponse(processingUrl));
				else
					return (sendDefaultResponse("No handler for url ["+processingUrl.toString(true)+"]"));
			}
			else if (request.startsWith("POST"))
			{
				processingUrl = request.fromFirstOccurrenceOf("POST ", false, true).upToFirstOccurrenceOf("HTTP/",false,false);

				if (owner.getProvider() && owner.getProvider()->isValidURL(processingUrl))
					return (sendResponse(processingUrl));
				else
					return (sendDefaultResponse("No handler for url ["+processingUrl.toString(true)+"]"));
			}
			else
			{
				return (sendDefaultResponse("Can't process this request (must be POST or GET)"));
			}
		}
		else
		{
			return (false);
		}
	}
	else
	{
		_DBG("EnviHTTPConnection read failed");
		return (false);
	}
}

const bool EnviHTTPConnection::sendDefaultResponse(const String &message)
{
	_DBG("EnviHTTPConnection::sendDefaultResponse");

	writeStringToSocket(socket, "HTTP/1.1 200 OK\nServer: Envimond\nContent-Length: "+_STR(message.length())+"\nContent-type: application/json; charset=UTF-8\nConnection: close\n\n"+message);
	return (true);
}

const bool EnviHTTPConnection::sendResponse(const URL &url)
{
	_DBG("EnviHTTPConnection::sendResponse");
	writeStringToSocket(socket, "HTTP/1.1 200 OK\nServer: Envimond\nContent-Length: 4\nContent-type: application/json; charset=UTF-8\nConnection: close\n\nmeh.");
	return (true);
}
