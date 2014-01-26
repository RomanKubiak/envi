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
	: socket(_socket), Thread("EnviHTTPConnection"), gotRequestHeaders(false), owner(_owner),
		httpMethod(UNKNOWN), responseSize(0), responseCode(200)
{
	{
		MessageManagerLock mmlock;
		addChangeListener (&owner);
	}
	startThread();
}

EnviHTTPConnection::~EnviHTTPConnection()
{
	removeChangeListener (&owner);
	signalThreadShouldExit();
	waitForThreadToExit (500);
}

void EnviHTTPConnection::run()
{
	while (1)
	{
		const int ret = socket->waitUntilReady (true, 2500);

		if (threadShouldExit())
		{
			return;
		}

		if (ret == 1)
		{
			/* normal */
			if (!respond())
			{
				_WRN("\tEnviHTTPConnection respond() failed");
			}
		}
		
		sendChangeMessage ();
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

const bool EnviHTTPConnection::respond()
{
	_DBG("EnviHTTPConnection::respond");
	if (readRequestData())
	{
		httpMethod = getRequestMethod(requestHeaders);
		
		if (httpMethod != UNKNOWN)
		{
			requestURL 					= getRequestURL (httpMethod, requestHeaders);

			_DBG("\t"+requestURL.toString(true));

			if (requestURL.toString(false) == "/")
				requestURL = requestURL.withNewSubPath("index.html");

			const File staticFileToSend	= owner.isStaticURL(requestURL.toString(false));

			if (staticFileToSend != File::nonexistent)
			{
				_DBG("\tstatic response");

				/** handle a static url request */
				if (staticFileToSend.existsAsFile())
					return (sendStaticResponse (staticFileToSend));
				else
					return (sendNotFoundResponse());
			}
			else if (requestURL.toString(true).startsWith ("/status"))
			{
				_DBG("\tstatus response");

				return (sendStatusResponse (requestURL));
			}
			else if (owner.getProvider() && owner.getProvider()->isValidURL(requestURL))
			{
				_DBG("\thandled response");

				StringPairArray responseHeaders;
				String responseData;

				Result res = owner.getProvider()->getResponse(requestURL, httpMethod, requestData, requestHeaders, requestBody, responseHeaders, responseData);

				if (res.wasOk())
				{
					return (sendResponse(responseHeaders, responseData));
				}
			}
			else
			{
				return (sendNotFoundResponse());
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

	return (false);
}

const bool EnviHTTPConnection::sendDefaultResponse(const String &message)
{
	responseSize = message.length();
	return (writeStringToSocket(socket, getStatndardResponseHeaders()+"Content-Length: "+_STR(message.length())+"\nContent-type: text/html; charset=UTF-8\nConnection: close\n\n"+message));
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

	responseSize = responseData.length();

	return (writeStringToSocket(socket, resp));
}

const String EnviHTTPConnection::getStatndardResponseHeaders()
{
	return ("HTTP/1.1 200 OK\nServer: EnviHTTP/"+SystemStats::getJUCEVersion()+"\n");
}

const bool EnviHTTPConnection::writeHeadersForBlob (const EnviHTTPCacheBlob &blob)
{
	String headers = getStatndardResponseHeaders();
	headers << "Content-length: " + _STR(blob.getSize()) + "\n";
	headers << "Connection: close\n";
	headers << "Content-type: " + blob.getMime() + "\n\n";
	
	const int ret = socket->waitUntilReady(false, 1000);

	if (ret == 1)
	{
		if (socket->write (headers.toUTF8(), headers.length()) < 0)
		{
			return (false);
		}

		return (true);
	}

	return (false);
}

const bool EnviHTTPConnection::writeHeadersForFile(const File &file)
{
	String headers = getStatndardResponseHeaders();
	headers << "Content-length: " + _STR(file.getSize()) + "\n";
	headers << "Connection: close\n";
	headers << "Content-type: " + owner.getMimeTypeFor(file) + "\n\n";

	const int ret = socket->waitUntilReady(false, 1000);

	if (ret == 1)
	{
		if (socket->write (headers.toUTF8(), headers.length()) < 0)
		{
			return (false);
		}
		else
		{
			return (true);
		}
	}

	return (false);
}

const bool EnviHTTPConnection::writeDataFromFile(const File &file)
{
	return (true);
}

const bool EnviHTTPConnection::writeDataFromBlob(const EnviHTTPCacheBlob &blob)
{
	int64 numBytesToWrite = blob.getSize();
	ScopedPointer <InputStream> source(blob.createInputStream());

	if (numBytesToWrite < 0)
        numBytesToWrite = std::numeric_limits<int64>::max();

    int numWritten = 0;

    while (numBytesToWrite > 0)
    {
        char buffer [8192];
        const int num = source->read (buffer, (int) jmin (numBytesToWrite, (int64) sizeof (buffer)));

        if (num <= 0)
            break;

        if (socket->write (buffer, (size_t) num) < 0)
			return (false);

        numBytesToWrite -= num;
        numWritten += num;
    }
    return (numWritten > 0);
}

const bool EnviHTTPConnection::sendFile(const File &fileToSend)
{
	if (owner.canCache (fileToSend))
	{
		EnviHTTPCacheBlob blob = EnviHTTPCache::getFromFile (fileToSend, owner.getMimeTypeFor(fileToSend));

		if (blob.isValid())
		{
			if (writeHeadersForBlob (blob))
			{
				return (writeDataFromBlob(blob));
			}
		}
	}
	else
	{
		if (writeHeadersForFile(fileToSend))
		{
			return (writeDataFromFile(fileToSend));
		}
	}

	return (false);
}

const bool EnviHTTPConnection::sendStaticResponse(const File &fileToSend)
{
	responseSize = fileToSend.getSize();

	if (fileToSend.existsAsFile())
	{
		if (sendFile (fileToSend))
		{
			return (true);
		}
		{
			return (sendDefaultResponse ("Can't send file ["+fileToSend.getFileName()+"]"));
		}
	}
	else
	{
		return (sendDefaultResponse ("Requested static resource not found"));
	}
}

const bool EnviHTTPConnection::sendStatusResponse(const URL &requestURL)
{
	String resp = "<html><body>Status<br />";
	resp << "</body></html>";

	return (sendDefaultResponse (resp));
}

const bool EnviHTTPConnection::sendNotFoundResponse()
{
	String response = "HTTP/1.1 404 Not Found\nServer: EnviHTTP/"+SystemStats::getJUCEVersion()+"\nContent-Type: text/html; charset=utf-8\n\n404 Not Found";
	responseCode = 404;
	responseSize = 13;
	const int ret = socket->waitUntilReady(false, 1000);

	if (ret == 1)
	{
		if (socket->write (response.toUTF8(), response.length()) < 0)
		{
			return (false);
		}

		return (true);
	}

	return (true);
}

const String EnviHTTPConnection::getHostname()
{
	return (socket->getHostName());
}

const int EnviHTTPConnection::getPort()
{
	return (socket->getPort());
}

const EnviHTTPMethod EnviHTTPConnection::getHTTPMethod()
{
	return (httpMethod);
}

const URL EnviHTTPConnection::getRequestURL()
{
	return (requestURL);
}

const int EnviHTTPConnection::getResponseCode()
{ 
	return (responseCode); 
}

const int64 EnviHTTPConnection::getResponseSize()
{
	return (responseSize); 
}