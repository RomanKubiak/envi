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

EnviHTTP::EnviHTTP(EnviHTTPProvider *_provider, const int listenPort) : provider(_provider), Thread("EnviHTTP")
{
	serverSocket = new StreamingSocket();

	if (serverSocket != nullptr)
	{
		if (!serverSocket->createListener(listenPort))
		{
			_ERR("Can't listen on specified TCP port: "+_STR(serverSocket->getPort()));
		}
		else
		{
			_INF("Listening for JSON-RPC rquests on TCP port: "+_STR(serverSocket->getPort()));
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

EnviHTTPProvider *EnviHTTP::getProvider()
{
	return (provider);
}

void EnviHTTP::run()
{
	while ((! threadShouldExit()) && serverSocket != nullptr)
	{
		ScopedPointer<StreamingSocket> streamingSocket (serverSocket->waitForNextConnection());

		if (streamingSocket != nullptr)
		{
			_DBG("EnviHTTP accepted connection from: "+streamingSocket->getHostName());
			processConnection (streamingSocket.release());
		}
		else
		{
			_DBG("EnviHTTP accepted connection, failed to create conversation socket");
		}
	}
}

void EnviHTTP::processConnection (StreamingSocket *socket)
{
	if (socket)
	{
		connectionPool.add (new EnviHTTPConnection (*this, socket));
	}
}
