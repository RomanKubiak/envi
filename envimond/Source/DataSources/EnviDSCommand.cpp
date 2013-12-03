/*
  ==============================================================================

    EnviDSCommand.cpp
    Created: 3 Dec 2013 12:32:39pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDSCommand.h"

EnviDSCommand::EnviDSCommand(const ValueTree instanceConfig)
{
	_DBG("EnviDSCommand::ctor");
	_DBG("\t"+instanceConfig.getProperty(Ids::name).toString());
	instanceState = instanceConfig.createCopy();
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
	return (true);
}

const var EnviDSCommand::getResult()
{
	return (var::null);
}

const var EnviDSCommand::getProperty (const Identifier &identifier)
{
	return (instanceState.getProperty (identifier));
}
