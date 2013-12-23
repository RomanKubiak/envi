/*
  ==============================================================================

    EnviDSTrigger.cpp
    Created: 23 Dec 2013 5:11:41pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDSTrigger.h"
#include "EnviApplication.h"

EnviDSTrigger::EnviDSTrigger(EnviApplication &_owner)
	: Thread("EnviDSTrigger"), EnviDataSource(owner, "trigger")
{
}

EnviDSTrigger::~EnviDSTrigger()
{
}

const Result EnviDSTrigger::execute()
{
	if (!isDisabled())
	{
		if (isThreadRunning())
		{
			notify();
			return (Result::fail(getType()+" ["+getName()+"] is already running"));
		}
		else
		{
			startThread();
			return (Result::ok());
		}
	}
	else
	{
		return (Result::fail(getType()+" ["+getName()+"] is disabled"));
	}
}

void EnviDSTrigger::run()
{
}

void EnviDSTrigger::handleAsyncUpdate()
{
}

const Result EnviDSTrigger::initialize(const ValueTree _instanceConfig)
{
	EnviDataSource::initialize (_instanceConfig);

	if (instanceConfig.isValid())
	{
		if (instanceConfig.getNumChildren() > 0)
			return (setAllExpressions());
	}

	return (Result::ok());
}
