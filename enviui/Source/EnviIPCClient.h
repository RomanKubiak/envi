/*
  ==============================================================================

    EnviIPCClient.h
    Created: 14 Jan 2014 1:09:10pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIIPCCLIENT_H_INCLUDED
#define ENVIIPCCLIENT_H_INCLUDED

#include "EnviUIIncludes.h"
class EnviUIMain;
class EnviIPCClient;

class EnviIPCRequestJob : public ThreadPoolJob, public AsyncUpdater
{
	public:
		EnviIPCRequestJob(URL requestURL, const var &_requestData, EnviIPCallback *_requestCallback);
		~EnviIPCRequestJob();
		static bool progressCallback(void *context, int bytesSent, int totalBytes);
		ThreadPoolJob::JobStatus runJob();
		void handleAsyncUpdate();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnviIPCRequestJob)

	private:
		CriticalSection jobLock;
		StringPairArray responseHeaders;
		InputStream *webInputStream;
		URL requestURL;
		var requestData;
		var responseData;
		EnviIPCallback *requestCallback;
};

class EnviIPCClient : public AsyncUpdater
{
	public:
		EnviIPCClient(EnviUIMain &_owner);
		~EnviIPCClient();
		void run();
		void handleAsyncUpdate();
		void request (const var requestData, EnviIPCallback *requestCallback = nullptr);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnviIPCClient)

	private:
		ThreadPool requestPool;
		EnviUIMain &owner;
		URL requestURL;
};

#endif  // ENVIIPCCLIENT_H_INCLUDED
