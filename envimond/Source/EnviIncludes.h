/*
  ==============================================================================

    EnviIncludes.h
    Created: 2 Dec 2013 1:54:19pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIINCLUDES_H_INCLUDED
#define ENVIINCLUDES_H_INCLUDED

// #define	_DBG(x)		Logger::writeToLog(x)
#include "../JuceLibraryCode/JuceHeader.h"
#include <iostream>

#define	_DBG(x)					Logger::writeToLog(x)
#define ENVI_TIMER_OFFSET		10

namespace Ids
{
	#define DECLARE_ID(name)      const Identifier name (#name)
	DECLARE_ID (envi);
	DECLARE_ID (dataSource);
	DECLARE_ID (name);
	DECLARE_ID (interval);
	DECLARE_ID (timeout);
	DECLARE_ID (type);
};

#endif  // ENVIINCLUDES_H_INCLUDED
