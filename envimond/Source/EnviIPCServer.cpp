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
	if (	url.toString(false) == "/jsonrpc"
		|| 	url.toString(false) == "/ping")
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
	responseHeaders.set ("Content-type", "application/json");

	if (requestUrl.toString(false) == "/jsonrpc")
	{
		responseData = JSON::toString(EnviJSONRPC::fromRequest(requestBody).getResponseWithParam("Responding"));
	}
	else if (requestUrl.toString(false) == "/ping")
	{
		responseData = "ENVIMOND/"+_STR(ProjectInfo::versionString)+"\r\n";
	}
	return (Result::ok());
}
