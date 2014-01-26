/*
  ==============================================================================

    EnviHTTP.cpp
    Created: 2 Dec 2013 1:39:13pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviHTTP.h"
#include "EnviHTTPConnection.h"
#include "EnviApplication.h"

EnviHTTP::EnviHTTP(EnviHTTPProvider *_provider, const int listenPort) : provider(_provider), Thread("EnviHTTP")
{
	serverSocket = new StreamingSocket();

	if (serverSocket != nullptr)
	{
		if (!serverSocket->createListener(listenPort))
		{
			_ERR("Can't listen on specified TCP port: "+_STR(serverSocket->getPort()));
		}
		else
		{
			_INF("Listening for JSON-RPC rquests on TCP port: "+_STR(serverSocket->getPort()));
			startThread();
		}
	}
}

EnviHTTP::~EnviHTTP()
{
	if (isThreadRunning())
	{
		if (serverSocket != nullptr)
			serverSocket->close();

		stopThread (4000);
		serverSocket = nullptr;
	}
}

EnviHTTPProvider *EnviHTTP::getProvider()
{
	return (provider);
}

void EnviHTTP::run()
{
	while ((! threadShouldExit()) && serverSocket != nullptr)
	{
		ScopedPointer<StreamingSocket> streamingSocket (serverSocket->waitForNextConnection());

		if (streamingSocket != nullptr)
		{
			processConnection (streamingSocket.release());
		}
		else
		{
			_ERR("EnviHTTP accepted connection, failed to create conversation socket");
		}
	}
}

void EnviHTTP::processConnection (StreamingSocket *socket)
{
	if (socket)
	{
		const ScopedLock sl(connectionPool.getLock());
		connectionPool.add (new EnviHTTPConnection (*this, socket));
	}
}

const File EnviHTTP::isStaticURL(const String &urlAsString) const
{
	const ScopedLock sl(staticUrlMap.getLock());
	for (int i=0; i<staticUrlMap.size(); i++)
	{
		StaticMapEntry &mapEntry = staticUrlMap.getReference(i);

		if (urlAsString.startsWith (mapEntry.url))
		{
			const File file = mapEntry.filesystemLocation.getChildFile(urlAsString.fromFirstOccurrenceOf (mapEntry.url,false,false));
			if (mapEntry.fileFilter.isFileSuitable(file))
				return (file);
		}
	}

	return (File::nonexistent);
}

void EnviHTTP::setStaticFolder (const String &urlToMap, const File filesystemLocation, const WildcardFileFilter fileWildcard)
{
	const ScopedLock sl(staticUrlMap.getLock());
	staticUrlMap.add (StaticMapEntry (urlToMap, filesystemLocation, fileWildcard));
}

void EnviHTTP::setMimeTypes (const char *mimeTypeData, const int mimeTypeDataSize)
{
    StringArray lines = StringArray::fromLines (String(mimeTypeData, mimeTypeDataSize));

    for (int i=0; i<lines.size(); i++)
	{
        StringArray entries = StringArray::fromTokens (lines[i].trim()," ", "\"'");
        const String mimetype = entries[0];
        entries.remove (0);
        for (int j=0; j<entries.size(); j++)
		{
			mimeTypes.set (entries[j], mimetype);
		}
	}
}

const String EnviHTTP::getMimeTypeFor(const File &fileToCheck)
{
	const ScopedLock sl (mimeTypes.getLock());
	const String ext = fileToCheck.getFileExtension().substring(1);

	if (mimeTypes.contains (ext))
	{
		return (mimeTypes[ext]);
	}
	else
	{
		return ("application/octet-stream");
	}
}

int EnviHTTP::getNumClientConnections()
{
	const ScopedLock sl(connectionPool.getLock());
	return (connectionPool.size());
}

void EnviHTTP::changeListenerCallback (ChangeBroadcaster* source)
{
	EnviHTTPConnection *conn = dynamic_cast<EnviHTTPConnection*>(source);
	if (conn != nullptr)
	{
		const ScopedLock sl(connectionPool.getLock());
		logAccess (conn);
		connectionPool.removeObject (conn);
	}
}

const bool EnviHTTP::canCache(const File &fileToCheck) const
{
	if (fileToCheck.getSize() < (1024 * 1024 * 5))
	{
		return (true);
	}

	return (false);
}

void EnviHTTP::setAccessLog (const File &fileToLogTo)
{
	if (fileToLogTo.hasWriteAccess())
	{
		accessLog = new FileLogger (fileToLogTo, String::empty);
	}
}

void EnviHTTP::setErrorLog (const File &fileToLogTo)
{
	if (fileToLogTo.hasWriteAccess())
	{
		errorLog = new FileLogger (fileToLogTo, String::empty);
	}
}

const String EnviHTTP::getLogTimestamp()
{
	return (Time::getCurrentTime().formatted("[%d/%b/%Y %H:%M:%S]"));
}

void EnviHTTP::logError(EnviHTTPConnection *source, const String &messageIfAny)
{
	if (errorLog)
	{
		accessLog->logMessage (	source->getHostname()
								+" "
								+getLogTimestamp()
								+" \""
								+EnviHTTPConnection::getMethodName(source->getHTTPMethod())
								+" "
								+source->getRequestURL().toString(true)
								+"\" "
								+_STR(source->getResponseCode())
								+" "
								+_STR(source->getResponseSize())
								+ (messageIfAny.isEmpty() ? (" "+messageIfAny) : String::empty)
							);
	}
}

void EnviHTTP::logAccess(EnviHTTPConnection *source, const String &messageIfAny)
{
	if (accessLog)
	{
		accessLog->logMessage (	source->getHostname()
								+" "
								+getLogTimestamp()
								+" \""
								+EnviHTTPConnection::getMethodName(source->getHTTPMethod())
								+" "
								+source->getRequestURL().toString(true)
								+"\" "
								+_STR(source->getResponseCode())
								+" "
								+_STR(source->getResponseSize())
								+ (messageIfAny.isEmpty() ? (" "+messageIfAny) : String::empty)
							);
	}
}