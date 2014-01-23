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
	int ret = socket->write (stringToWrite.toUTF8(), stringToWrite.length());
	return (ret == stringToWrite.length());
}

const bool EnviHTTPConnection::readRequestData()
{
	MemoryBlock temp (8192,true);
	int dataRead = 0;

	while (socket->waitUntilReady(true, 0))
	{
		dataRead = socket->read (temp.getData(), 8192, false);

		if (dataRead > 0)
		{
			requestData.append (temp.getData(), temp.getSize());
			continue;
		}

		if (dataRead < 0)
		{
			return (false);
		}
	}

	const String requestDataAsString = requestData.toString();

	requestHeaders 	= requestDataAsString.upToFirstOccurrenceOf("\r\n\r\n", false, true);
	requestBody		= requestDataAsString.fromFirstOccurrenceOf("\r\n\r\n", false, true);

	return (true);
}

const String EnviHTTPConnection::getMethodName(const EnviHTTPMethod method)
{
	switch (method)
	{
	case GET:
		return ("GET");
	case POST:
		return ("POST");
	case OPTIONS:
		return ("OPTIONS");
	case CONNECT:
		return ("CONNECT");
	case HEAD:
		return ("HEAD");
	default:
		break;
	}

	return ("UNKNOWN");
}

const EnviHTTPMethod EnviHTTPConnection::getRequestMethod(const String &headers)
{
	if (headers.startsWith("GET"))
	{
		return (GET);
	}
	else if (headers.startsWith("POST"))
	{
		return (POST);
	}
	else
	{
		return (UNKNOWN);
	}
}

const URL EnviHTTPConnection::getRequestURL(const EnviHTTPMethod method, const String &headers)
{
	return (URL(headers.fromFirstOccurrenceOf(getMethodName(method),false,true).upToFirstOccurrenceOf("HTTP/1.",false,false).trim()));
}

const bool EnviHTTPConnection::getRequestHeaders()
{
	if (readRequestData())
	{
		const EnviHTTPMethod method = getRequestMethod(requestHeaders);

		if (method != UNKNOWN)
		{
			requestURL 					= getRequestURL (method, requestHeaders);
			const File staticFileToSend	= owner.isStaticURL(requestURL.toString(false));

			if (staticFileToSend != File::nonexistent)
			{
				/** handle a static url request */
				return (sendStaticResponse (staticFileToSend));
			}
			else if (owner.getProvider() && owner.getProvider()->isValidURL(requestURL))
			{
				StringPairArray responseHeaders;
				String responseData;

				Result res = owner.getProvider()->getResponse(requestURL, method, requestData, requestHeaders, requestBody, responseHeaders, responseData);

				if (res.wasOk())
				{
					return (sendResponse(responseHeaders, responseData));
				}
			}
			else
			{
				return (sendDefaultResponse("No handler for url ["+requestURL.toString(true)+"]"));
			}
		}
		else
		{
			return (sendDefaultResponse("Unknown HTTP method used"));
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
	writeStringToSocket(socket, getStatndardResponseHeaders()+"Content-Length: "+_STR(message.length())+"\nContent-type: application/json; charset=UTF-8\nConnection: close\n\n"+message);
	return (true);
}

const bool EnviHTTPConnection::sendResponse(const StringPairArray &responseHeaders, const String &responseData)
{
	String resp = getStatndardResponseHeaders();

	for (int i=0; i<responseHeaders.size(); i++)
	{
		resp << responseHeaders.getAllKeys() [i];
		resp << ": ";
		resp << responseHeaders.getAllValues() [i];
		resp << "\n";
	}
	resp << "Content-length: " << responseData.length() << "\n";
	resp << "Connection: close\n";
	resp << "Date: " << Time::getCurrentTime().formatted ("%a, %d %b %Y %H:%M:%S %Z") << "\n";
	resp << "\n";
	resp << responseData;

	//_DBG(resp);
	return (writeStringToSocket(socket, resp));
}

const String EnviHTTPConnection::getStatndardResponseHeaders()
{
	return ("HTTP/1.1 200 OK\nServer: JUCE/"+SystemStats::getJUCEVersion()+"\n");
}

const bool EnviHTTPConnection::sendFile(const File &fileToSend)
{
	return (true);
}

const bool EnviHTTPConnection::sendStaticResponse(const File &fileToSend)
{
	if (fileToSend.existsAsFile())
	{
		sendFile (fileToSend);
	}
	else
	{
		return (sendDefaultResponse ("Requested static resource not found"));
	}
}
