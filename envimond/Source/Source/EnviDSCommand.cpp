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
	: Thread("EnviDSCommand"), commandLine(String::empty), EnviDataSource(owner, "cmd")
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
		commandLine	= instanceConfig.hasProperty (Ids::cmd)	? getProperty(Ids::cmd).toString() : String::empty;

		if (instanceConfig.getNumChildren() > 0)
			return (setAllExpressions());
	}

	return (Result::ok());
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

			if (commandLine.isEmpty())
			{
				_WRN("Command is an empty string (not set in XML?), disabling");
				setDisabled (true);
				return;
			}

			ChildProcess childProc;
			commandOutput = String::empty;

			if (childProc.start(commandLine))
			{
				if (childProc.waitForProcessToFinish(getTimeout()))
				{
					commandOutput = childProc.readAllProcessOutput().trim();
					triggerAsyncUpdate();
				}
				else
				{
					_DBG("timeout reached: "+_STR(getTimeout()));
					_DBG("command: "+commandLine);

					_WRN("["+getName()+"] timeout");

					if (!childProc.kill())
					{
						_WRN("["+getName()+"] can't kill child process");
					}
				}
			}
			else
			{
				_WRN("["+getName()+"] failed to start child process");
			}
		} while (wait (-1));
	}
}

void EnviDSCommand::handleAsyncUpdate()
{
	EnviData data = EnviData::fromJSON(commandOutput, getInstanceNumber());

	for (int i=0; i<data.getNumValues(); i++)
	{
		if (hasExpression(data[i].name))
		{
			const double result= evaluateExpression (data[i].value, data[i].name);

			if (result != (double)data[i].value)
			{
				data[i].value = result;
			}
		}
	}
	result.copyValues (data);
	collectFinished (commandOutput.isEmpty() ? Result::fail("Command output empty, command: "+commandLine) : Result::ok());
}
