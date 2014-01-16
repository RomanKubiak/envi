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

class EnviIPCServer : public Thread, public EnviHTTPProvider
{
	public:
		EnviIPCServer (EnviApplication &_owner);
		void run();
		const bool isValidURL (const URL &url);
        const MemoryBlock getResponseForURL (const URL &url);

	private:
		EnviApplication &owner;
};

#endif  // ENVIIPCSERVER_H_INCLUDED
