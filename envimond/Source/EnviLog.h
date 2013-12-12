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

class EnviLog : public Logger
{
	public:
		EnviLog();
		~EnviLog();
		void setOwner (EnviApplication *_owner);
		void logMessage(const int level, const String &message);
		void logMessage(const String &message);
		const Result setLogToFile(const File _fileToLogTo);
		void setLogToConsole(const bool _logToConsole);
		static const String levelToString(const int logLevel);

		juce_DeclareSingleton (EnviLog, false);

	private:
		EnviApplication *owner;
		ScopedPointer <FileLogger> fileLogger;
		bool logToConsole;

};


#endif  // ENVILOG_H_INCLUDED
