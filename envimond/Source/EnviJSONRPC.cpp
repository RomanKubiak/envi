/*
  ==============================================================================

    EnviJSONRPC.cpp
    Created: 17 Jan 2014 1:27:15pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviJSONRPC.h"

static var empty(const bool addDefaultProperties=true)
{
	DynamicObject *dso = new DynamicObject();
	if (addDefaultProperties)
	{
		dso->setProperty("jsonrpc", "2.0");
		dso->setProperty("id", "-1");
		dso->setProperty("error", String::empty);
	}
	return (var(dso));
}

EnviJSONRPC::EnviJSONRPC() : response(empty()), request(empty())
{
}

EnviJSONRPC::EnviJSONRPC(const EnviJSONRPC &other) : request(other.request), response(other.response)
{
}

EnviJSONRPC::EnviJSONRPC(const String &initialData, const bool isRequest)
{
	if (isRequest)
	{
		request 	= JSON::fromString(initialData);
		response 	= empty();
		setResponseId (getRequestId());
	}
	else
	{
		request 	= empty();
		response 	= JSON::fromString(initialData);
	}
}

EnviJSONRPC::EnviJSONRPC(var data, const bool isRequest)
{
	if (isRequest)
	{
		request 	= data;
		response 	= empty();
		setResponseId (getRequestId());
	}
	else
	{
		request 	= empty();
		response 	= data;
	}
}

void EnviJSONRPC::setResponseErrorString (const String &errorString)
{
	if (response.getDynamicObject()) response.getDynamicObject()->setProperty ("error", errorString);
}

void EnviJSONRPC::setRequestErrorString (const String &errorString)
{
	if (request.getDynamicObject()) request.getDynamicObject()->setProperty ("error", errorString);
}

const String EnviJSONRPC::getResponseMethodName() const
{
	return (response.getProperty("method", var::null));
}

const String EnviJSONRPC::getRequestMethodName() const
{
	return (request.getProperty("method", var::null));
}

void EnviJSONRPC::setRequestMethodName(const String &methodName)
{
	if (request.getDynamicObject()) request.getDynamicObject()->setProperty ("method", methodName);
}

void EnviJSONRPC::setResponseMethodName(const String &methodName)
{
	if (response.getDynamicObject()) response.getDynamicObject()->setProperty ("method", methodName);
}

const String EnviJSONRPC::getRequestNamespace()
{
	return (getRequestMethodName().upToFirstOccurrenceOf(".", false, true));
}

const String EnviJSONRPC::getResponseNamespace()
{
	return (getResponseMethodName().upToFirstOccurrenceOf(".", false, true));
}

const var EnviJSONRPC::getRequestParameters() const
{
	return (request.getDynamicObject() ? request.getDynamicObject()->getProperty("params") : var::null);
}

const var EnviJSONRPC::getResponseParameters() const
{
	return (response.getDynamicObject() ? response.getDynamicObject()->getProperty("params") : var::null);
}

void EnviJSONRPC::setResponseParameters(const var parameters)
{
	if (response.getDynamicObject()) response.getDynamicObject()->setProperty ("params", parameters);
}

void EnviJSONRPC::setRequestParameters(const var parameters)
{
	if (request.getDynamicObject()) request.getDynamicObject()->setProperty ("params", parameters);
}

const int64 EnviJSONRPC::getRequestId() const
{
	return (request.getDynamicObject() ? (int64)request.getDynamicObject()->getProperty("id") : 0.0);
}

const int64 EnviJSONRPC::getResponseId() const
{
	return (response.getDynamicObject() ? (int64)response.getDynamicObject()->getProperty("id") : 0.0);
}

void EnviJSONRPC::setRequestId(const int64 id)
{
	if (request.getDynamicObject()) request.getDynamicObject()->setProperty("id", id);
}

void EnviJSONRPC::setResponseId(const int64 id)
{
	if (response.getDynamicObject()) response.getDynamicObject()->setProperty("id", id);
}

void EnviJSONRPC::setResponseResult(const var result)
{
	if (response.getDynamicObject()) response.getDynamicObject()->setProperty("result", result);
}

void EnviJSONRPC::setRequestResult(const var result)
{
	if (request.getDynamicObject()) request.getDynamicObject()->setProperty("result", result);
}

var &EnviJSONRPC::getResponse()
{
	return (response);
}

var &EnviJSONRPC::getRequest()
{
	return (request);
}

const String EnviJSONRPC::responseToString()
{
	return (JSON::toString(response));
}

const String EnviJSONRPC::requestToString()
{
	return (JSON::toString(request));
}

const var EnviJSONRPC::getResponseWithParam(const var responseParam)
{
    setResponseParameters (responseParam);
    return (response);
}

EnviJSONRPC EnviJSONRPC::fromRequest(const String &jsonEncodedRequest)
{
	return (EnviJSONRPC (JSON::parse (jsonEncodedRequest), true));
}

EnviJSONRPC EnviJSONRPC::error (const String &errorMessage, const int id)
{
	return (EnviJSONRPC("{\"jsonrpc\": \"2.0\", \"error\": \""+errorMessage+"\", \"id\":"+_STR(id)+"}", false));
}

Result EnviJSONRPC::isValid(const String &jsonEncodedData)
{
	var temp;
	Result res = JSON::parse (jsonEncodedData, temp);

	if (res.wasOk())
	{
		if (temp.getDynamicObject())
		{
			DynamicObject *dso = temp.getDynamicObject();

			if (dso->hasProperty("jsonrpc"))
			{
				if (dso->getProperty("jsonrpc") != "2.0")
				{
					return (Result::fail("JSON-RPC request version is not 2.0 ["+dso->getProperty("jsonrpc").toString()+"]"));
				}

				if (dso->hasProperty("method") && dso->hasProperty("params"))
				{
					return (Result::ok());
				}

				if (!dso->hasProperty("method"))
				{
					return (Result::fail("JSON-RPC missing required method request parameter"));
				}

				if (!dso->hasProperty("params"))
				{
					return (Result::fail("JSON-RPC missing required params request parameter"));
				}
			}
			else
			{
				return (Result::fail("JSON-RPC missing jsonrpc property"));
			}
		}
		else
		{
			return (Result::fail("JSON-RPC json data is not an object"));
		}
	}
	else
	{
		return (Result::fail("JSON-RPC json parser failed ["+res.getErrorMessage()+"]"));
	}

	return (Result::fail("JSON-RPC Undefined parser state"));
}

const var EnviJSONRPC::toArray (const StringPairArray &stringPairArray)
{
	var ret = empty(false);
	DynamicObject *dso = ret.getDynamicObject();

	for (int i=0; i<stringPairArray.size(); i++)
	{
		dso->setProperty (stringPairArray.getAllKeys() [i], stringPairArray.getAllValues() [i]);
	}

	return (ret);
}
