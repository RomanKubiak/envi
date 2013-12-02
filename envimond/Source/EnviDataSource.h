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

class EnviDataSource
{
	public:
		EnviDataSource(const String &_sourceName) : sourceName(_sourceName)
		{
		}

		virtual ~EnviDataSource()
		{
		}

	private:
		String sourceName;
};


#endif  // ENVIDATASOURCE_H_INCLUDED
