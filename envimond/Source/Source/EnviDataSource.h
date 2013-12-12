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

		void copyValues(const EnviData &valuesToUpdateFrom);
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
		String dataSourceType;
		int dataSourceInstanceNumber;
		JUCE_LEAK_DETECTOR(EnviData);

	protected:

};

class EnviDataSource : public ChangeBroadcaster, public Expression::Scope
{
	public:
		EnviDataSource(EnviApplication &_owner, const Identifier &_type);
		virtual ~EnviDataSource() {}
		virtual const Result execute() 		= 0;
		virtual const Result initialize(const ValueTree _instanceConfig);

		void collectFinished(const Result collectStatus);
		const var getProperty (const Identifier &identifier) const;
		void setProperty (const Identifier identifier, const var &value);
		const int getInterval() const;
		const int getInstanceNumber() const;
		void setInstanceNumber(const int instanceNumber);
		const String getName() const;
		void setName(const String &name);
		const Identifier getType() const;
		const int getTimeout() const;
		const EnviData getResult() const;
		ValueTree getConfig() const;
		bool startSource();
		void stopSource();
		void setDisabled(const bool shouldBeDisabled);
		bool isDisabled() const;
		String getScopeUID() const;
		Expression getSymbolValue(const String &symbol) const;
		double evaluateFunction (const String &functionName, const double *parameters, int numParameters) const;

		JUCE_LEAK_DETECTOR(EnviDataSource);

	protected:
		ValueTree instanceConfig;
		EnviApplication &owner;
		CriticalSection dataSourceLock;
		EnviData result;

	private:
		Time startTime, endTime;
		bool disabled;
		int instanceNumber;
};
#endif  // ENVIDATASOURCE_H_INCLUDED
