/*
  ==============================================================================

    EnviLog.h
    Created: 3 Dec 2013 12:18:09pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVILOG_H_INCLUDED
#define ENVILOG_H_INCLUDED

#include "EnviIncludes.h"
class EnviApplication;

class EnviLog : public AsyncUpdater, public Logger
{
	public:
		EnviLog(EnviApplication &_owner);
		~EnviLog();
		void handleAsyncUpdate();
		void logMessage (const String &message);

	private:
		EnviApplication &owner;
};


#endif  // ENVILOG_H_INCLUDED
