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
class EnviDataSource;

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
		const String respondWithJSONError(const Result &whyRequestFailed);
		const Result processJSONRequest(const String &request);

		const Result processEnviRPC(EnviJSONRPC &reqeust);
		const Result getNumDataSources(EnviJSONRPC &reqeust);
		const Result getDataSource(EnviJSONRPC &reqeust);

        static const var dataSourceToJSON(EnviDataSource *dataSource);

		JUCE_LEAK_DETECTOR(EnviIPCServer);

	private:
		CriticalSection ipcCriticalSection;
		EnviJSONRPC jsonRPC;
		EnviApplication &owner;
		HashMap<String, std::function<const Result(EnviJSONRPC &)> > methods;
};

#endif  // ENVIIPCSERVER_H_INCLUDED
