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
		void setLogLevel(const int _logLevel);
		static const String levelToString(const int logLevel);

		struct EnviLogMessage
		{
			EnviLogMessage (const int _level, const String &_message) : level(_level), message(_message) {}
			EnviLogMessage() : level(-1) {}
			EnviLogMessage (const EnviLogMessage &other) : level(other.level), message(other.message) {}
			int level;
			String message;
		};
		juce_DeclareSingleton (EnviLog, false);

	private:
		Array<EnviLogMessage,CriticalSection> historyLog;
		EnviApplication *owner;
		ScopedPointer <FileLogger> fileLogger;
		bool logToConsole;
		int logLevel;
		Atomic<int> logHistorySize;

};


#endif  // ENVILOG_H_INCLUDED
