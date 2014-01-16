/*
  ==============================================================================

    EnviIPCServer.cpp
    Created: 16 Jan 2014 11:35:21am
    Author:  rk09631

  ==============================================================================
*/

#include "EnviIPCServer.h"
#include "EnviApplication.h"

EnviIPCServer::EnviIPCServer(EnviApplication &_owner) : owner(_owner), Thread("EnviIPCServer")
{
}

void EnviIPCServer::run()
{
}

const bool EnviIPCServer::isValidURL (const URL &url)
{
	_DBG("EnviIPCServer::isValidURL");
	_DBG(url.toString(true));
	return (false);
}

const MemoryBlock EnviIPCServer::getResponseForURL (const URL &url)
{
	_DBG("EnviIPCServer::getResponseForURL");
	_DBG(url.toString(true));
	return (MemoryBlock());
}
