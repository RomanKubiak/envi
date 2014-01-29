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
		const String getResponseMethodName() const;
		void setRequestMethodName(const String &methodName);
		void setResponseMethodName(const String &methodName);
		const var getRequestParameters() const;
		const var getResponseParameters() const;
		void setResponseParameters(const var parameters);
		void setRequestParameters(const var parameters);
		void setResponseResult(const var result);
		void setRequestResult(const var result);
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
		const var getResponseWithParam(const var responseParam);
		static const var toArray (const StringPairArray &stringPairArray);
		static EnviJSONRPC fromRequest(const String &jsonEncodedRequest);
		static EnviJSONRPC error (const String &errorMessage, const int id=0);
		static Result isValid(const String &jsonEncodedData);
	private:
		var response;
		var request;
};

#endif  // ENVIJSONRPC_H_INCLUDED
