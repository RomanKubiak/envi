/*
  ==============================================================================

    EnviData.h
    Created: 20 Dec 2013 10:34:06am
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDATA_H_INCLUDED
#define ENVIDATA_H_INCLUDED

#include "EnviIncludes.h"

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
			Pascal,
			Unknown
		};

		struct Value
		{
			Value()
				: error(0), unit(Unknown), timestamp(Time::getCurrentTime())
			{}

			Value(const Value &other)
				:	name(other.name), unit(other.unit), value(other.value), error(other.error), timestamp(other.timestamp)
			{}
			Value(const String _name, const Unit _unit)
				: name(_name), unit(_unit), error(0)
			{}
			String name;
			var value;
			bool error;
			Unit unit;
			Time timestamp;
		};

		EnviData();
		EnviData(const EnviData &other);
		EnviData(const String &firstValueName, const Unit valueUnit);
		EnviData(const String &firstValueName, const var firstValueValue=var::null, const Time firstValueSampleTime=Time());
		bool operator== (const EnviData& other) noexcept;
		void operator= (const EnviData& other) noexcept;
		EnviData::Value& operator[] (int arrayIndex) const;

		void copyValues(const EnviData &valuesToUpdateFrom);
		const int getNumValues() const;
		void addValue(const EnviData::Value valueToAdd);
		Array <Value> values;
		const int getSize() const;

		static const EnviData fromJSON(const String &jsonString, const String &dsName, const int dsInstance, const String &dsType);
		static const String toJSON(const EnviData &enviData);
		static const String toCSVString(const EnviData &enviData, const String &separator=";");
		static const StringArray toSQL(const EnviData &enviData, const String &dataTable="data", const String &unitTable="units");
		static const var toVAR(const EnviData &enviData);
		static const String unitToString(const Unit unit);
		static const Unit stringToUnit(const String &unit);

		String name;
		String type;
		int instance;
		JUCE_LEAK_DETECTOR(EnviData);

	protected:

};



#endif  // ENVIDATA_H_INCLUDED