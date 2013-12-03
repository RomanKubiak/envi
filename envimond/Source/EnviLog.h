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
		EnviLog();
		~EnviLog();
		void setOwner (EnviApplication *_owner);
		void handleAsyncUpdate();
		void logMessage(const int level, const String &message);
		void logMessage(const String &message);

		juce_DeclareSingleton (EnviLog, false);

	private:
		EnviApplication *owner;
};


#endif  // ENVILOG_H_INCLUDED