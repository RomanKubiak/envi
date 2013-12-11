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

	serverSocket = new StreamingSocket();

	if (serverSocket != nullptr)
	{
		if (!serverSocket->createListener(owner.getProperties() ? owner.getProperties()->getIntValue(Ids::port, 31337) : 31337))
		{
			_ERR("Can't listen on specified TCP port: "+_STR(serverSocket->getPort()));
		}
		else
		{
			_DBG("\tlistening on TCP port: "+_STR(serverSocket->getPort()));
			startThread();
		}
	}
}

EnviHTTP::~EnviHTTP()
{
	if (isThreadRunning())
	{
		if (serverSocket != nullptr)
			serverSocket->close();

		stopThread (4000);
		serverSocket = nullptr;
	}
}

void EnviHTTP::run()
{
	while ((! threadShouldExit()) && serverSocket != nullptr)
	{
		ScopedPointer<StreamingSocket> streamingSocket (serverSocket->waitForNextConnection());

		if (streamingSocket != nullptr)
		{
			_DBG("EnviHTTP accepted connection from: "+streamingSocket->getHostName());
			processConnection (streamingSocket);
		}
	}
}

void EnviHTTP::processConnection (StreamingSocket *socket)
{
	if (socket)
	{
		connectionPool.add (new EnviHTTPConnection (socket));
	}
}
