/*
  ==============================================================================

    EnviScheduler.h
    Created: 2 Dec 2013 1:39:29pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVISCHEDULER_H_INCLUDED
#define ENVISCHEDULER_H_INCLUDED

#include "EnviIncludes.h"
class EnviApplication;

class EnviScheduler : public Timer
{
	public:
		EnviScheduler(EnviApplication &_owner);
		~EnviScheduler();
		void timerCallback();

	private:
		EnviApplication &owner;
};

#endif  // ENVISCHEDULER_H_INCLUDED
