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
#include "EnviLog.h"
#include <iostream>

#define LOG_INFO				0
#define LOG_WARN				1
#define LOG_DEBUG				2
#define LOG_JUCE				3
#define LOG_ERROR				4

#define _LOG(lvl,msg)			EnviLog::getInstance()->logMessage(lvl,msg)
#define	_DBG(msg)				_LOG(LOG_DEBUG, msg)
#define _ERR(msg)				_LOG(LOG_ERROR, msg)
#define _WRN(msg)				_LOG(LOG_WARN, msg)
#define _INF(msg)				_LOG(LOG_INFO, msg)

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
	DECLARE_ID (cmd);
	DECLARE_ID (port);
	DECLARE_ID (storeFile);
};

#endif  // ENVIINCLUDES_H_INCLUDED
