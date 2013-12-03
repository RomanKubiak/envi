/*
  ==============================================================================

    EnviDataSource.h
    Created: 2 Dec 2013 1:40:26pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDATASOURCE_H_INCLUDED
#define ENVIDATASOURCE_H_INCLUDED

#include "EnviIncludes.h"

class EnviDataSource : public ChangeBroadcaster
{
	public:
		EnviDataSource()
		{
		}

		virtual ~EnviDataSource()
		{
		}

		virtual const String getName() 		= 0;
		virtual const int getInterval() 	= 0;
		virtual const int getTimeout()		= 0;
		virtual const bool execute() 		= 0;
		virtual const var getResult()		= 0;

	protected:
		CriticalSection dataSourceLock;
};


#endif  // ENVIDATASOURCE_H_INCLUDED
