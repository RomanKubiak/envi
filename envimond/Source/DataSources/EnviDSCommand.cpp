/*
  ==============================================================================

    EnviDSCommand.cpp
    Created: 3 Dec 2013 12:32:39pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDSCommand.h"
#include "../EnviApplication.h"

EnviDSCommand::EnviDSCommand(EnviApplication &owner, const ValueTree instanceConfig) 
	: EnviDataSource(owner), Thread("EnviDSCommand"), timeout(0)
{
	_DBG("EnviDSCommand::ctor");
	_DBG("\t"+instanceConfig.getProperty(Ids::name).toString());
	instanceState = instanceConfig.createCopy();

	if (instanceState.isValid())
	{
		timeout = getProperty(Ids::timeout);
		cmd		= getProperty(Ids::cmd);
	}
}

EnviDSCommand::~EnviDSCommand()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
}

const String EnviDSCommand::getName()
{
	ScopedLock sl (dataSourceLock);
	return (getProperty(Ids::name));
}

const int EnviDSCommand::getInterval()
{
	ScopedLock sl (dataSourceLock);
	return (getProperty(Ids::interval));
}

const int EnviDSCommand::getTimeout()
{
	ScopedLock sl (dataSourceLock);
	return (getProperty(Ids::timeout));
}

const bool EnviDSCommand::execute()
{
	_DBG("EnviDSCommand::execute ["+getName()+"] command: "+getProperty(Ids::cmd)+" timeout: "+getProperty(Ids::timeout));

	startThread();

	return (true);
}

const var EnviDSCommand::getResult()
{
	ScopedLock sl (dataSourceLock);
	return (commandOutput);
}

const var EnviDSCommand::getProperty (const Identifier &identifier)
{
	return (instanceState.getProperty (identifier));
}

void EnviDSCommand::run()
{
	ScopedLock sl (dataSourceLock);
	ChildProcess childProc;
	commandOutput = String::empty;

	if (childProc.start(cmd, ChildProcess::wantStdOut))
	{
		if (childProc.waitForProcessToFinish(getTimeout()))
		{
			commandOutput = childProc.readAllProcessOutput ();
		}
		else
		{
			_WRN("EnviDSCommand::execute ["+getName()+"] timeout");

			if (!childProc.kill())
			{
				_WRN("EnviDSCommand::execute ["+getName()+"] can't kill child process");
			}
		}
	}
	else
	{
		_WRN("EnviDSCommand::execute ["+getName()+"] failed to start child process");
	}

	triggerAsyncUpdate();
}

void EnviDSCommand::handleAsyncUpdate()
{
	ScopedLock sl (dataSourceLock);

	if (commandOutput.isEmpty())
	{
		owner.sourceFailed (this);
	}
	else
	{
		owner.sourceWrite (this);
	}
}