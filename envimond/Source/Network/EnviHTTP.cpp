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

	coreMethods.set ("systemStatus", std::bind (&EnviHTTP::systemStatus, this, std::placeholders::_1));
	coreMethods.set ("serverStatus", std::bind (&EnviHTTP::serverStatus, this, std::placeholders::_1));
	coreMethods.set ("ping", std::bind (&EnviHTTP::ping, this, std::placeholders::_1));
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
		connectionPool.add (new EnviHTTPConnection (*this, socket));
	}
}

const File EnviHTTP::isStaticURL(const String &urlAsString) const
{
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
	return (connectionPool.size());
}

void EnviHTTP::changeListenerCallback (ChangeBroadcaster* source)
{
	EnviHTTPConnection *conn = dynamic_cast<EnviHTTPConnection*>(source);
	if (conn != nullptr)
	{
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
		errorLog->logMessage (	source->getHostname()
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

const String EnviHTTP::getOsType(const SystemStats::OperatingSystemType type)
{
	switch (type)
	{
	case SystemStats::Linux:
		return ("Linux");
	case SystemStats::Android:
		return ("Android");
	case SystemStats::iOS:
		return ("iOS");
	case SystemStats::MacOSX_10_4:
		return ("Mac OSX 10.4");
	case SystemStats::MacOSX_10_5:
		return ("Mac OSX 10.5");
	case SystemStats::MacOSX_10_6:
		return ("Mac OSX 10.6");
	case SystemStats::MacOSX_10_7:
		return ("Mac OSX 10.7");
	case SystemStats::MacOSX_10_8:
		return ("Mac OSX 10.8");
	case SystemStats::Win2000:
		return ("Windows 2000");
	case SystemStats::WinXP:
		return ("Windows XP");
	case SystemStats::WinVista:
		return ("Windows Vista");
	case SystemStats::Windows7:
		return ("Windows 7");
	case SystemStats::Windows8:
		return ("Windows 8");
	case SystemStats::Windows:
		return ("Windows ?");
	default:
		break;
	}

	return ("Unknown OS");
}

const StringPairArray EnviHTTP::getSystemStats()
{
	StringPairArray result;
	result.set ("JUCE version", SystemStats::getJUCEVersion());
	result.set ("OS Type", EnviHTTP::getOsType(SystemStats::getOperatingSystemType()));
	result.set ("OS Name", SystemStats::getOperatingSystemName());
	result.set ("OS 64bits", SystemStats::isOperatingSystem64Bit() ? "Yes" : "No");
	result.set ("Logon name", SystemStats::getLogonName());
	result.set ("Full user name", SystemStats::getFullUserName());
	result.set ("Computer name", SystemStats::getComputerName());
	result.set ("User language", SystemStats::getUserLanguage());
	result.set ("User region", SystemStats::getUserRegion());
	result.set ("Device description", SystemStats::getDeviceDescription());
	result.set ("Number of CPUs", _STR(SystemStats::getNumCpus()));
	result.set ("CPU speed (MHz)", _STR(SystemStats::getCpuSpeedInMegaherz()));
	result.set ("CPU vendor", SystemStats::getCpuVendor());
	result.set ("Has MMX", SystemStats::hasMMX() ? "Yes" : "No");
	result.set ("Has SSE", SystemStats::hasSSE() ? "Yes" : "No");
	result.set ("Has SSE2", SystemStats::hasSSE2() ? "Yes" : "No");
	result.set ("Has SSE3", SystemStats::hasSSE3() ? "Yes" : "No");
	result.set ("Has 3DNow", SystemStats::has3DNow() ? "Yes" : "No");
	result.set ("Memory size (MB)", _STR(SystemStats::getMemorySizeInMegabytes()));
	result.set ("Page size", _STR(SystemStats::getPageSize()));

	return (result);
}

const Result EnviHTTP::processCoreRPC (EnviJSONRPC &request)
{
	const String methodName = request.getRequestMethodName().fromFirstOccurrenceOf("core.", false, false);

	if (coreMethods.contains (methodName))
	{
        return (coreMethods[methodName] (request));
	}
	else
	{
		return (Result::fail("Unhandled CORE method ["+methodName+"]"));
	}
}

const Result EnviHTTP::systemStatus(EnviJSONRPC &request)
{
	return (Result::ok());
}

const Result EnviHTTP::serverStatus(EnviJSONRPC &request)
{
	return (Result::ok());
}

const Result EnviHTTP::ping(EnviJSONRPC &request)
{	
	request.setResponseResult (EnviJSONRPC::toJSONArray ("pong", Time::getCurrentTime().toMilliseconds()));
    return (Result::ok());
}
