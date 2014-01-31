/*
  ==============================================================================

    EnviJSONRPC.h
    Created: 17 Jan 2014 1:27:15pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIJSONRPC_H_INCLUDED
#define ENVIJSONRPC_H_INCLUDED

#include "EnviIncludes.h"

class EnviJSONRPC
{
	public:
		EnviJSONRPC();
		EnviJSONRPC(const EnviJSONRPC &other);
		EnviJSONRPC(const String &initialData, const bool isRequest=true);
		EnviJSONRPC(var data, const bool isRequest=true);
		~EnviJSONRPC() {}
		void setResponseErrorString (const String &errorString);
		void setRequestErrorString (const String &errorString);
		const String getRequestMethodName() const;
		void setRequestMethodName(const String &methodName);
		const var getRequestParameters() const;
		void setResponseResult(const var result);
		const var getResponseResult() const;
		void setRequestParameters(const var parameters);
		const int64 getRequestId() const;
		const int64 getResponseId() const;
		void setRequestId(const int64 id);
		void setResponseId(const int64 id);
		var &getResponse();
		var &getRequest();
		const String responseToString();
		const String requestToString();
		const String getRequestNamespace();
		const String getResponseNamespace();
		const var getResponseWithResult(const var responseResult);
		static const var toArray (const StringPairArray &stringPairArray);
		static EnviJSONRPC fromRequest(const String &jsonEncodedRequest);
		static EnviJSONRPC error (const String &errorMessage, const int id=0);
		static Result isValid(const String &jsonEncodedData);

		static var empty(const bool addDefaultProperties=true, const bool isRequest=true)
		{
			DynamicObject *dso = new DynamicObject();
			if (addDefaultProperties)
			{
				dso->setProperty("jsonrpc", "2.0");
				dso->setProperty("id", "-1");
			}

			if (isRequest)
			{
				dso->setProperty("method", var::null);
				dso->setProperty("params", var::null);
			}
			else
			{
				dso->setProperty ("result", var::null);
			}

			return (var(dso));
		}

	private:
		var response;
		var request;
};

#endif  // ENVIJSONRPC_H_INCLUDED
