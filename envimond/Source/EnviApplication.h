/*
  ==============================================================================

    EnviApplication.h
    Created: 2 Dec 2013 1:54:29pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIAPPLICATION_H_INCLUDED
#define ENVIAPPLICATION_H_INCLUDED

#include "EnviCLI.h"

class EnviApplication
{
	public:
		EnviApplication(int argc, char* argv[]);
		bool messageLoop();
		void cleanupAndExit();

	private:
		EnviCLI enviCLI;
};

#endif  // ENVIAPPLICATION_H_INCLUDED
