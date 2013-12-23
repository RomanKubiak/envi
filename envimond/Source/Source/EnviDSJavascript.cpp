/*
  ==============================================================================

    EnviDSJavascript.cpp
    Created: 23 Dec 2013 7:11:04pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDSJavascript.h"
#include "EnviApplication.h"

#define GET_ME() 			if (args.numArguments > 0) EnviJS *me = dynamic_cast<EnviJS*> (args.thisObject.getObject());
EnviJS::EnviJS(EnviDSJavascript &_owner) : owner(_owner)
{
	setMethod ("logMessage", logMessage);
}

EnviJS::~EnviJS()
{
}

EnviDSJavascript &EnviJS::getOwner()
{
	return (owner);
}

var EnviJS::logMessage(const var::NativeFunctionArgs& args)
{
	_LOG((int)args.arguments[0], args.arguments[1].toString());
	return var::undefined();
}

/**
 *
 */
EnviDSJavascript::EnviDSJavascript(EnviApplication &_owner)
	: Thread("EnviDSJavascript"), EnviDataSource(owner, "javascript")
{
}

EnviDSJavascript::~EnviDSJavascript()
{
}

const Result EnviDSJavascript::execute()
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

void EnviDSJavascript::run()
{
}

void EnviDSJavascript::handleAsyncUpdate()
{
}

const Result EnviDSJavascript::initialize(const ValueTree _instanceConfig)
{
	EnviDataSource::initialize (_instanceConfig);

	if (instanceConfig.isValid())
	{
		if (instanceConfig.getNumChildren() > 0)
			return (setAllExpressions());
	}

	engine.maximumExecutionTime = RelativeTime::seconds (5);
	engine.registerNativeObject ("EnviJS", new EnviJS (*this));

	return (Result::ok());
}
