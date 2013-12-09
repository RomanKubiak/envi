/*
  ==============================================================================

    EnviHTTP.cpp
    Created: 2 Dec 2013 1:39:13pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviHTTP.h"
#include "EnviHTTPConnection.h"
#include "EnviApplication.h"

EnviHTTP::EnviHTTP(EnviApplication &_owner) : owner(_owner), Thread("EnviHTTP")
{
	_DBG("EnviHTTP::ctor");
	startThread();
}

EnviHTTP::~EnviHTTP()
{
}

void EnviHTTP::run()
{
	if (serverSocket.createListener(owner.getProperties() ? owner.getProperties()->getIntValue(Ids::port, 31337) : 31337))
	{
		_DBG("EnviHTTP listening on port: "+String(serverSocket.getPort()));

		while (1)
		{
			StreamingSocket *streamingSocket = serverSocket.waitForNextConnection ();

			if (threadShouldExit())
			{
				_INF("EnviHTTP thread signaled to exit");
				return;
			}

			_DBG("EnviHTTP accepted connection from: "+streamingSocket->getHostName());
			processConnection (streamingSocket);
		}
	}
	else
	{
		_ERR("EnviHTTP can't bind to TCP port: "+String(serverSocket.getPort()));
	}
}

void EnviHTTP::processConnection (StreamingSocket *socket)
{
	if (socket)
	{
		connectionPool.add (new EnviHTTPConnection (socket));
	}
}