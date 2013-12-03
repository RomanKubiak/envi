/*
  ==============================================================================

    EnviDSCommand.h
    Created: 3 Dec 2013 12:32:39pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDSCOMMAND_H_INCLUDED
#define ENVIDSCOMMAND_H_INCLUDED

#include "../EnviIncludes.h"
#include "../EnviDataSource.h"

class EnviDSCommand : public EnviDataSource
{
	public:
		EnviDSCommand(const ValueTree instanceConfig);
		const String getName();
		const int getInterval();
		const int getTimeout();
		const bool execute();
		const var getResult();
		const var getProperty (const Identifier &identifier);
	private:
		ValueTree instanceState;
};

#endif  // ENVIDSCOMMAND_H_INCLUDED
