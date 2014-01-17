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
	_DBG("EnviIPCServer::isValidURL");
	_DBG(url.toString(true));

	if (url.toString(false) == "/")
	{
		return (true);
	}
	return (false);
}

const Result EnviIPCServer::getResponse (const URL &requestUrl, const MemoryBlock &requestData, StringPairArray &responseHeaders, String &responseData)
{
	_DBG("EnviIPCServer::getResponse");
	_DBG(requestUrl.toString(true));

	DynamicObject *dso = new DynamicObject();
	dso->setProperty ("result", "Hello JSON-RPC");
	dso->setProperty ("error", var::null);
	dso->setProperty ("id", 1);
	var response(dso);

	responseHeaders.set ("Content-type", "application/json");

	if (requestUrl.toString(false) == "/")
	{
        responseData = JSON::toString(response);
	}
	return (Result::ok());
}
