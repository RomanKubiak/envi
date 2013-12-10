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
			Value()
				: error(0), unit(Unknown), sampleTime(Time::getCurrentTime()), index(0)
			{}

			Value(const Value &other)
				:	name(other.name), unit(other.unit), value(other.value),
					error(other.error), index(other.index),
					sampleTime(other.sampleTime)
			{}
			Value(const String _name, const Unit _unit)
				: name(_name), unit(_unit), error(0)
			{}
			String name;
			var value;
			bool error;
			Unit unit;
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

		const int getSize() const;
		static const EnviData fromJSON(const String &jsonString, const int dataSourceIndex=0);
		static const String toJSON(const EnviData &enviData);
		static const String toCSVString(const EnviData &enviData, const String &separator=";");
		static const StringArray toSQL(const EnviData &enviData, const String &dataTable="data", const String &unitTable="units");
		static const String unitToString(const Unit unit);
		static const Unit stringToUnit(const String &unit);

		String dataSourceName;
		int dataSourceId;
		JUCE_LEAK_DETECTOR(EnviData);

	protected:
		
};

class EnviDataSource : public ChangeBroadcaster, public Expression::Scope
{
	public:
		EnviDataSource(EnviApplication &_owner, const ValueTree _instanceConfig);
		virtual ~EnviDataSource() {}
		virtual const var getProperty (const Identifier &identifier);
		virtual const String getName() 		= 0;
		virtual const int getInterval() 	= 0;
		virtual const int getTimeout()		= 0;
		virtual const bool execute() 		= 0;
		virtual const EnviData getResult()	= 0;
		virtual EnviData::Unit getUnit()	{ return (EnviData::Integer); }

		ValueTree getConfig()				{ return (instanceConfig); }
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

		String getScopeUID() const;
		Expression getSymbolValue(const String &symbol) const;
		double evaluateFunction (const String &functionName, const double *parameters, int numParameters) const;
		JUCE_LEAK_DETECTOR(EnviDataSource);

	protected:
		ValueTree instanceConfig;
		EnviApplication &owner;
		CriticalSection dataSourceLock;
		int index;

	private:
		Time startTime, endTime;
		bool disabled;
};


#endif  // ENVIDATASOURCE_H_INCLUDED
