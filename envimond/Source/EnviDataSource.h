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
class EnviApplication;

class EnviData
{
	public:
		enum Unit
		{
			Integer,
			Text,
			Float,
			Volt,
			Amp,
			Celsius,
			Decibel,
			Lux,
			Hertz,
			Ohm,
			Farad,
			kUnitCount
		};

		EnviData();
		EnviData(const EnviData &other);
		static EnviData createFromString(const String &dataSource);

	private:
		String name;
		var value;
		bool error;
};

class EnviDataSource : public ChangeBroadcaster
{
	public:
		EnviDataSource(EnviApplication &_owner) : owner(_owner), disabled(false)
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
		virtual EnviData::Unit getUnit()	{ return (EnviData::Integer); }
		bool startSource()
		{
			ScopedLock sl(dataSourceLock);
			startTime = Time::getCurrentTime();
			return (execute());
		}

		void stopSource()
		{
			ScopedLock sl(dataSourceLock);
			endTime = Time::getCurrentTime();
		}

		void setDisabled(const bool shouldBeDisabled)
		{
			ScopedLock sl(dataSourceLock);
			disabled = shouldBeDisabled;
		}

		bool isDisabled()
		{
			ScopedLock sl(dataSourceLock);
			return (disabled);
		}

	protected:
		EnviApplication &owner;
		CriticalSection dataSourceLock;

	private:
		Time startTime, endTime;
		bool disabled;
};


#endif  // ENVIDATASOURCE_H_INCLUDED
