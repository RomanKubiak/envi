/*
  ==============================================================================

    EnviIPCServer.cpp
    Created: 16 Jan 2014 11:35:21am
    Author:  rk09631

  ==============================================================================
*/

#include "EnviIPCServer.h"
#include "EnviApplication.h"

EnviIPCServer::EnviIPCServer(EnviApplication &_owner) : owner(_owner)
{
	methods.set ("getNumDataSources", std::bind (&EnviIPCServer::getNumDataSources, this, std::placeholders::_1));
	methods.set ("getDataSource", std::bind (&EnviIPCServer::getDataSource, this, std::placeholders::_1));
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
	EnviJSONRPC rpc = EnviJSONRPC::fromRequest (request);

	_DBG("EnviIPCServer::processJSONRequest");
	_DBG(request);

	if (rpc.getRequestNamespace() == Ids::envi.toString())
	{
		return (processEnviRPC(rpc));
	}
	else if (rpc.getRequestNamespace() == Ids::core.toString())
	{
		return (owner.getEnviHTTP().processCoreRPC(rpc));
	}

	return (respondWithJSONError(Result::fail("Unhandled RPC method")));
}

const String EnviIPCServer::respondWithJSONError(const Result &whyRequestFailed)
{
	return (EnviJSONRPC::error(whyRequestFailed.getErrorMessage()).responseToString());
}

const String EnviIPCServer::processEnviRPC(const EnviJSONRPC &rpc)
{
	const String methodName = rpc.getRequestMethodName().fromFirstOccurrenceOf("envi.", false, false);

	if (methods.contains (methodName))
	{
        return (methods[methodName] (rpc.getRequestParameters()));
	}
	else
	{
		return (EnviJSONRPC::error("Unhandled ENVI method"));
	}
}

const var EnviIPCServer::getNumDataSources(const var params)
{
	return (owner.getNumDataSources());
}

const var EnviIPCServer::getDataSource(const var params)
{
	if (owner.getDataSource (params[0]))
	{
		return (owner.getDataSource(params[0])->getDataSourceInfoAsJSON());
	}
	return (var::null);
}
