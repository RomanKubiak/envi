/*
  ==============================================================================

    EnviApplication.cpp
    Created: 2 Dec 2013 1:54:30pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviApplication.h"

EnviApplication::EnviApplication(int argc, char* argv[])
	: enviCLI(argc, argv, "dn:c")
{
	_DBG("EnviApplication::ctor");
}

bool EnviApplication::messageLoop()
{
	return (true);
}

void EnviApplication::cleanupAndExit()
{
}
