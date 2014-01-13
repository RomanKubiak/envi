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
		{
			for (int i=0; i<instanceConfig.getNumChildren(); i++)
			{
				if (instanceConfig.getChild(i).hasType(Ids::dataValue))
					addValue (instanceConfig.getChild(i).getProperty(Ids::name), stringToUnit(instanceConfig.getChild(i).getProperty(Ids::unit)));
			}

			_DSDBG("EnviDSCommand::initialize values ["+_STR(getNumValues())+"]");

			return (setAllExpressions());
		}
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
			return (Result::ok());
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

			_DSDBG("exec ["+command.joinIntoString(",")+"]");

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
	collectFinished (Result::ok());
}

void EnviDSCommand::processCommandOutput (const String _commandOutput)
{
	commandOutput = _commandOutput;

	if (!commandOutput.isEmpty())
	{
		StringArray lines = StringArray::fromLines (commandOutput);
		Array <double> results;

		for (int i=0; i<lines.size(); i++)
		{
			results.add (lines[i].getDoubleValue());
		}
		Result res = evaluateAllExpressions (results);

		if (res.wasOk())
		{
			triggerAsyncUpdate();
		}
		else
		{
			_DSWRN(res.getErrorMessage());
		}
	}
	else
	{
		_WRN("["+getName()+"] empty output from command: ["+command.joinIntoString(" ").trim()+"]");
	}
}
