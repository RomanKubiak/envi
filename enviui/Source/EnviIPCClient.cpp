/*
  ==============================================================================

    EnviIPCClient.cpp
    Created: 14 Jan 2014 1:09:10pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviIPCClient.h"
#include "EnviUIMain.h"

EnviIPCClient::EnviIPCClient(EnviUIMain &_owner) : owner(_owner)
{
	_DBG("EnviIPCClient::ctor");
	enviIPCConnectionChecker = new EnviIPCConnectionChecker(*this);
}

EnviIPCClient::~EnviIPCClient()
{
}

void EnviIPCClient::connectionMade()
{
	_DBG("EnviIPCClient::connectionMade");
}

void EnviIPCClient::connectionLost()
{
	_DBG("EnviIPCClient::connectionLost");
}

void EnviIPCClient::messageReceived (const MemoryBlock& message)
{
}

void EnviIPCClient::handleAsyncUpdate()
{
	if (isConnected())
	{
		owner.setStatus (EnviUIMain::UIConnected);
	}
	else
	{
		owner.setStatus (EnviUIMain::UIConnecting);
	}
}

/**
 * Connection checker
 */
EnviIPCConnectionChecker::EnviIPCConnectionChecker(EnviIPCClient &_owner)
	: owner(_owner), Thread("EnviIPCConnectionChecker")
{
	startThread();
}

EnviIPCConnectionChecker::~EnviIPCConnectionChecker()
{
	signalThreadShouldExit();
	waitForThreadToExit(1500);
}

void EnviIPCConnectionChecker::run()
{
}
