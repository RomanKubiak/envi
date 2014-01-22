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

class EnviIPCServer : public Thread, public EnviHTTPProvider
{
	public:
		EnviIPCServer (EnviApplication &_owner);
		void run();
		const bool isValidURL (const URL &url);
        const Result getResponse (	const URL &requestUrl,
									const EnviHTTPMethod methodUsed,
									const MemoryBlock &requestData,
									const String &requestHeaders,
									const String &requestBody,
									StringPairArray &responseHeaders,
									String &responseData);

		JUCE_LEAK_DETECTOR(EnviIPCServer);

	private:
		EnviJSONRPC jsonRPC;
		EnviApplication &owner;
};

#endif  // ENVIIPCSERVER_H_INCLUDED
