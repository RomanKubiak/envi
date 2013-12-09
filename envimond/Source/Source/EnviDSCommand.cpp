/*
  ==============================================================================

    EnviDSCommand.cpp
    Created: 3 Dec 2013 12:32:39pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDSCommand.h"
#include "EnviApplication.h"

EnviDSCommand::EnviDSCommand(EnviApplication &owner, const ValueTree instanceConfig)
	: EnviDataSource(owner), Thread("EnviDSCommand"), timeout(0)
{
	instanceState = instanceConfig.createCopy();

	if (instanceState.isValid())
	{
		timeout = (bool)instanceState.hasProperty (Ids::timeout) ? (int)getProperty(Ids::timeout) : 5000;

		if (!instanceState.hasProperty(Ids::cmd))
		{
			_WRN("EnviDSCommand::ctor no cmd set, disabling source");
			setDisabled(true);
		}
		else
		{
			cmd		= getProperty(Ids::cmd);
		}
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
	return (getProperty(Ids::name));
}

const int EnviDSCommand::getInterval()
{
	return (getProperty(Ids::interval));
}

const int EnviDSCommand::getTimeout()
{
	return (getProperty(Ids::timeout));
}

const bool EnviDSCommand::execute()
{
	if (!isDisabled())
	{
		if (isThreadRunning())
		{
			notify();
		}
		else
		{
			startThread();
		}
		return (true);
	}

	return (false);
}

const EnviData EnviDSCommand::getResult()
{
	ScopedLock sl (dataSourceLock);
	return (EnviData::fromJSON(commandOutput));
}

const var EnviDSCommand::getProperty (const Identifier &identifier)
{
	ScopedLock sl (dataSourceLock);
	return (instanceState.getProperty (identifier));
}

void EnviDSCommand::run()
{
	while (1)
	{
		do
		{
			if (threadShouldExit())
			{
				_DBG("EnviDSCommand::run thread signalled to exit");
				return;
			}

			ScopedLock sl (dataSourceLock);
			ChildProcess childProc;
			commandOutput = String::empty;

			if (childProc.start(cmd, ChildProcess::wantStdOut))
			{
				if (childProc.waitForProcessToFinish(getTimeout()))
				{
					commandOutput = childProc.readAllProcessOutput().trim();
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
		} while (wait (-1));
	}
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
