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
	: Thread("EnviDSCommand"), EnviDataSource(owner, "cmd")
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
	EnviDataSource::initialize (_instanceConfig);

	if (instanceConfig.isValid())
	{
		command.set (0, instanceConfig.hasProperty (Ids::cmd)	? getProperty(Ids::cmd).toString() : String::empty);
		command.set (1, instanceConfig.hasProperty (Ids::args)	? getProperty(Ids::args).toString() : String::empty);

		if (!File::isAbsolutePath(command[0]))
		{
			command.set (0, owner.getEnviScriptsDir().getChildFile(command[0]).getFullPathName());
		}

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

void EnviDSCommand::run()
{
	while (1)
	{
		do
		{
			SHOULD_WE_EXIT();

			if (command[0].isEmpty())
			{
				_WRN("Command is an empty string (not set in XML?), disabling");
				setDisabled (true);
				return;
			}

			ChildProcess childProc;
			commandOutput = String::empty;

			if (childProc.start(command))
			{
				if (childProc.waitForProcessToFinish(getTimeout()))
				{
					processCommandOutput(childProc.readAllProcessOutput().trim());
				}
				else
				{
					_WRN("["+getName()+"] timeout");

					if (!childProc.kill())
					{
						_WRN("["+getName()+"] can't kill child process");
					}
				}
			}
			else
			{
				_WRN("["+getName()+"] failed to start child process ["+command.joinIntoString(" ").trim()+"]");
			}
		} while (wait (-1));
	}
}

void EnviDSCommand::handleAsyncUpdate()
{
	{
		ScopedLock sl(safeResultLock);
		setResult (safeResult);
	}

	_DBG("EnviDSCommand::handleAsyncUpdate");
	_DBG("\n"+EnviData::toCSVString(getResult()));

	collectFinished (Result::ok());
}

void EnviDSCommand::processExpressions()
{
	for (int i=0; i<safeResult.getNumValues(); i++)
	{
		if (hasExpression(safeResult[i].name))
		{
			const double result= evaluateExpression (safeResult[i].value, safeResult[i].name);

			if (result != (double)safeResult[i].value)
			{
				safeResult[i].value = result;
			}
		}
	}
}

void EnviDSCommand::processCommandOutput (const String _commandOutput)
{
	commandOutput = _commandOutput;

	if (!commandOutput.isEmpty())
	{
		ScopedLock sl(safeResultLock);

		safeResult = EnviData::fromJSON(commandOutput, getName(), getInstanceNumber(), getType());

		if (safeResult.getNumValues() > 0)
		{
			processExpressions();
		}

		triggerAsyncUpdate();
	}
	else
	{
		_WRN("["+getName()+"] empty output from command: ["+command.joinIntoString(" ").trim()+"]");
	}
}
