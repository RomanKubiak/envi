/*
  ==============================================================================

    EnviIPCServer.cpp
    Created: 16 Jan 2014 11:35:21am
    Author:  rk09631

  ==============================================================================
*/

#include "EnviIPCServer.h"
#include "EnviApplication.h"

EnviIPCServer::EnviIPCServer(EnviApplication &_owner) : owner(_owner), Thread("EnviIPCServer")
{
}

void EnviIPCServer::run()
{
}

const bool EnviIPCServer::isValidURL (const URL &url)
{
	if (	url.toString(false).startsWith ("/jsonrpc")
		|| 	url.toString(false).startsWith ("/ping"))
	{
		return (true);
	}
	return (false);
}

const Result EnviIPCServer::getResponse (	const URL &requestUrl,
											const EnviHTTPMethod methodUsed,
											const MemoryBlock &requestData,
											const String &requestHeaders,
											const String &requestBody,
											StringPairArray &responseHeaders,
											String &responseData)
{
	if (requestUrl.toString(false).startsWith ("/jsonrpc"))
	{
		responseHeaders.set ("Content-type", "application/json");

		if (methodUsed == GET)
		{
			responseData = "Can't use GET with json-rpc";
		}
		else
		{
			Result res = EnviJSONRPC::isValid(requestBody);

			if (res.wasOk())
			{
				responseData = processJSONRequest(requestBody);
			}
			else
			{
				responseData = respondWithJSONError(res);
			}
		}
	}
	else if (requestUrl.toString(false) == "/ping")
	{
		responseData = "PONG/"+_STR(Time::getCurrentTime().toMilliseconds())+"/ENVIMOND/"+_STR(ProjectInfo::versionString)+"\r\n";
	}
	return (Result::ok());
}

const String EnviIPCServer::processJSONRequest(const String &request)
{
	return (String::empty);
}

const String EnviIPCServer::respondWithJSONError(const Result &whyRequestFailed)
{
	return (String::empty);
}
