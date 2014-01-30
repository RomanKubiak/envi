/*
  ==============================================================================

    EnviIPCServer.h
    Created: 16 Jan 2014 11:35:21am
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIIPCSERVER_H_INCLUDED
#define ENVIIPCSERVER_H_INCLUDED

#include "EnviHTTP.h"
#include "EnviJSONRPC.h"
#include <functional>

class EnviIPCServer;

class EnviIPCServer : public EnviHTTPProvider
{
	public:
		EnviIPCServer (EnviApplication &_owner);
		const bool isValidURL (const URL &url);
        const Result getResponse (	const URL &requestUrl,
									const EnviHTTPMethod methodUsed,
									const MemoryBlock &requestData,
									const String &requestHeaders,
									const String &requestBody,
									StringPairArray &responseHeaders,
									String &responseData);
		const String processJSONRequest(const String &request);
		const String respondWithJSONError(const Result &whyRequestFailed);
		const String processEnviRPC(const EnviJSONRPC &rpc);

		const var getNumDataSources(const var params);
		const var getDataSource(const var params);

		JUCE_LEAK_DETECTOR(EnviIPCServer);

	private:
		CriticalSection ipcCriticalSection;
		EnviJSONRPC jsonRPC;
		EnviApplication &owner;
		HashMap<String, std::function<const var(const var)> > methods;
};

#endif  // ENVIIPCSERVER_H_INCLUDED
