/*
  ==============================================================================

    EnviDSCommand.cpp
    Created: 3 Dec 2013 12:32:39pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDSCommand.h"
#include "EnviApplication.h"

EnviDSCommand::EnviDSCommand(EnviApplication &owner)
	: Thread("EnviDSCommand"), cmd(String::empty), EnviDataSource(owner, "cmd")
{
}

EnviDSCommand::~EnviDSCommand()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
}

const Result EnviDSCommand::initialize(const ValueTree _instanceConfig)
{
	instanceConfig = _instanceConfig.createCopy();

	if (instanceConfig.isValid())
	{
		cmd					= instanceConfig.hasProperty (Ids::cmd)		? getProperty(Ids::cmd).toString()		: String::empty;
	}
}

const Result EnviDSCommand::execute()
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
		return (Result::ok());
	}

	return (Result::ok());
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
	result.copyValues (EnviData::fromJSON(commandOutput));
	collectFinished (commandOutput.isEmpty() ? Result::fail("Command output empty") : Result::ok());
}
