/*
  ==============================================================================

    EnviDSCommand.cpp
    Created: 3 Dec 2013 12:32:39pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDSCommand.h"
#include "EnviApplication.h"

EnviDSCommand::EnviDSCommand(EnviApplication &_owner, const ValueTree _instanceConfig)
	: EnviDataSource(_owner, _instanceConfig), Thread("EnviDSCommand"), timeout(0)
{
	if (instanceConfig.isValid())
	{
		index				= result.dataSourceId	= Command_DS + (int)getProperty(Ids::index);
		timeout				= instanceConfig.hasProperty (Ids::timeout)	? (int)getProperty(Ids::timeout) : 5000;
		cmd					= instanceConfig.hasProperty (Ids::cmd)		? getProperty(Ids::cmd).toString()		: String::empty;
	}
}

EnviDSCommand::~EnviDSCommand()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
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
	return (EnviData::fromJSON(commandOutput,index));
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

			if (cmd.isEmpty())
			{
				_WRN("EnviDSCommand::run command is an empty string (not set in XML?), disabling");
				setDisabled (true);
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
