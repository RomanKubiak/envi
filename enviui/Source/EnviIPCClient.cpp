/*
  ==============================================================================

    EnviIPCClient.cpp
    Created: 14 Jan 2014 1:09:10pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviIPCClient.h"
#include "EnviUIMain.h"

EnviIPCClient::EnviIPCClient(EnviUIMain &_owner)
	: owner(_owner), requestPool(5)
{
	_DBG("EnviIPCClient::ctor");
}

EnviIPCClient::~EnviIPCClient()
{
}

void EnviIPCClient::handleAsyncUpdate()
{
}

void EnviIPCClient::request (const var requestData, EnviIPCallback *requestCallback)
{
	requestPool.addJob (new EnviIPCRequestJob (requestURL, requestData, requestCallback), true);
}

/** Request JOB
 *
 */
EnviIPCRequestJob::EnviIPCRequestJob(URL _requestURL, const var &_requestData, EnviIPCallback *_requestCallback)
	: 	requestData(_requestData),
		requestURL(_requestURL),
		requestCallback(_requestCallback),
		ThreadPoolJob("EnviIPCRequestJob"),
		webInputStream(nullptr)
{
}

EnviIPCRequestJob::~EnviIPCRequestJob()
{
	if (webInputStream != nullptr)
	{
		deleteAndZero (webInputStream);
	}
}

void EnviIPCRequestJob::handleAsyncUpdate()
{
	const ScopedLock sl (jobLock);

	if (requestCallback)
	{
		requestCallback->requestSuccess(requestData, responseData, responseHeaders);
	}
}

ThreadPoolJob::JobStatus EnviIPCRequestJob::runJob()
{
	const ScopedLock sl (jobLock);

	webInputStream = requestURL.withPOSTData(JSON::toString(requestData)).createInputStream (true, &EnviIPCRequestJob::progressCallback, this, "Client: EnviUI", 2000, &responseHeaders);

	if (webInputStream)
	{
		Result res = JSON::parse (webInputStream->readEntireStreamAsString(), responseData);

		if (res.wasOk())
		{
			triggerAsyncUpdate();
			return (ThreadPoolJob::jobHasFinished);
		}
		else
		{
			_ERR("Response from server can't be parsed as JSON ["+res.getErrorMessage()+"]");
			return (ThreadPoolJob::jobNeedsRunningAgain);
		}
	}
	else
	{
		_ERR("Unable to create connection to server");
		return (ThreadPoolJob::jobNeedsRunningAgain);
	}
}

bool EnviIPCRequestJob::progressCallback(void *context, int bytesSent, int totalBytes)
{
	return (true);
}
