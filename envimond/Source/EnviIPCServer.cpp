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
				Result processResult = processJSONRequest(requestBody);

				if (processResult.wasOk())
				{
					responseData = jsonRPC.responseToString();
				}
				else
				{
					responseData = respondWithJSONError(processResult);
				}
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

const Result EnviIPCServer::processJSONRequest(const String &request)
{
	jsonRPC = EnviJSONRPC::fromRequest (request);

	if (jsonRPC.getRequestNamespace() == Ids::envi.toString())
	{
		return (processEnviRPC(jsonRPC));
	}
	else if (jsonRPC.getRequestNamespace() == Ids::core.toString())
	{
		return (owner.getEnviHTTP().processCoreRPC(jsonRPC));
	}

	return (Result::fail("Unhandled RPC method"));
}

const String EnviIPCServer::respondWithJSONError(const Result &whyRequestFailed)
{
	return (EnviJSONRPC::error(whyRequestFailed.getErrorMessage()).responseToString());
}

const Result EnviIPCServer::processEnviRPC(EnviJSONRPC &rpc)
{
	const String methodName = rpc.getRequestMethodName().fromFirstOccurrenceOf("envi.", false, false);

	if (methods.contains (methodName))
	{
        return (methods[methodName] (rpc));
	}
	else
	{
		return (Result::fail ("Unhandled ENVI method ["+methodName+"]"));
	}
}

const Result EnviIPCServer::getNumDataSources(EnviJSONRPC &request)
{
	return (Result::ok());
}

const Result EnviIPCServer::getDataSource(EnviJSONRPC &request)
{
	return (Result::ok());
}
