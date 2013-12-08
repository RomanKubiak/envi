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
			Percent,
			Volt,
			Amp,
			Celsius,
			Fahrenheit,
			Decibel,
			Lux,
			Hertz,
			Ohm,
			Farad,
			Watt,
			KiloWattHour,
			Unknown
		};

		struct Value
		{
			Value() {}
			Value(const String _name, const Unit _unit) : name(_name), unit(_unit) {}
			String name;
			var value;
			bool error;
			Unit unit;
			int interval;
			Time sampleTime;
			int index;
		};

		EnviData();
		EnviData(const EnviData &other);
		EnviData(const String &firstValueName, const Unit valueUnit);
		EnviData(const String &firstValueName, const var firstValueValue=var::null, const Time firstValueSampleTime=Time());
		bool operator== (const EnviData& other) noexcept;
		EnviData::Value& operator[] (int arrayIndex) const;

		const int getNumValues() const;
		void addValue(const EnviData::Value valueToAdd);
		Array <Value> values;

		static EnviData fromJSON(const String &jsonString);
		static const String toJSON(const EnviData &enviData);
		static const String toCSVString(const EnviData &enviData, const String &separator=";");

		static const String unitToString(const Unit unit);
		static const Unit stringToUnit(const String &unit);

		String dataSourceName;
		JUCE_LEAK_DETECTOR(EnviData);
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
		virtual const EnviData getResult()	= 0;
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

		JUCE_LEAK_DETECTOR(EnviDataSource);

	protected:
		EnviApplication &owner;
		CriticalSection dataSourceLock;

	private:
		Time startTime, endTime;
		bool disabled;
};


#endif  // ENVIDATASOURCE_H_INCLUDED
