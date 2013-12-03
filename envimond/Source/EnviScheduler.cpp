/*
  ==============================================================================

    EnviScheduler.cpp
    Created: 2 Dec 2013 1:39:29pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviScheduler.h"
#include "EnviApplication.h"

EnviScheduler::EnviScheduler(EnviApplication &_owner) : owner(_owner)
{
	_DBG("EnviScheduler::ctor");
	startTimer (100);
}

EnviScheduler::~EnviScheduler()
{
	stopTimer();
}

void EnviScheduler::timerCallback()
{
}
