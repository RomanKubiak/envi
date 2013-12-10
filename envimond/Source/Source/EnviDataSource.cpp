/*
  ==============================================================================

    EnviDataSource.cpp
    Created: 4 Dec 2013 1:54:19pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDataSource.h"

EnviData::EnviData()
{
}

EnviData::EnviData(const EnviData &other)
	: values(other.values), dataSourceName(other.dataSourceName)
{
}

EnviData::EnviData(const String &firstValueName, const var firstValueValue, const Time firstValueSampleTime)
{
	if (firstValueName != String::empty)
	{
		EnviData::Value value;
		value.name			= firstValueName;
		value.value			= firstValueValue;
		value.sampleTime	= firstValueSampleTime;

		values.add (value);
	}
}

EnviData::EnviData(const String &firstValueName, const Unit valueUnit)
{
	if (firstValueName != String::empty)
	{
		EnviData::Value value;
		value.name		= firstValueName;
		value.unit		= valueUnit;

		values.add (value);
	}
}

bool EnviData::operator== (const EnviData& other) noexcept
{
	if (values.size() != other.values.size())
		return (false);

	for (int i=0; i<values.size(); i++)
	{
		if (values[i].unit != other.values[i].unit || values[i].value != other.values[i].value)
			return (false);
	}

	return (true);
}

EnviData::Value& EnviData::operator[] (int arrayIndex) const
{
	return (values.getReference (arrayIndex));
}

const int EnviData::getNumValues() const
{
	return (values.size());
}

void EnviData::addValue(const EnviData::Value valueToAdd)
{
	values.add (valueToAdd);
}

const int EnviData::getSize() const
{
	return (dataSourceName.length() + sizeof(Value) * getNumValues());
}

const EnviData EnviData::fromJSON(const String &jsonString)
{
	EnviData enviData;
	var data				= JSON::parse (jsonString);
	enviData.dataSourceName = data["name"];
	var values				= data["values"];

	for (int i=0; i<values.size(); i++)
	{
		EnviData::Value value(values[i]["name"], stringToUnit(values[i]["unit"]));

		value.value			= values[i]["value"];
		value.error			= values[i]["error"];
		value.sampleTime	= Time(values[i]["sampleTime"]);
		value.index			= values[i]["index"];

		enviData.addValue (value);
	}

	return (enviData);
}

const String EnviData::toJSON(const EnviData &enviData)
{
	String ret;
	DynamicObject *ds = new DynamicObject();
	ds->setProperty ("name", enviData.dataSourceName);
	var values;

	for (int i=0; i<enviData.getNumValues(); i++)
	{
		DynamicObject *value = new DynamicObject();
		value->setProperty ("name", enviData[i].name);
		value->setProperty ("value", enviData[i].value.toString());
		value->setProperty ("unit", unitToString(enviData[i].unit));
		value->setProperty ("error", enviData[i].error);
		value->setProperty ("sampleTime", enviData[i].sampleTime.getMilliseconds());
		value->setProperty ("index", enviData[i].index);

		values.append (value);
	}
	ds->setProperty ("values", values);

	return (JSON::toString(var(ds)));
}

const String EnviData::toCSVString(const EnviData &enviData, const String &separator)
{
	String ret;
	for (int i=0; i<enviData.getNumValues(); i++)
	{
		ret
			<< enviData.dataSourceName				<< separator
			<< enviData[i].name 					<< separator
			<< unitToString(enviData[i].unit) 			<< separator
			<< enviData[i].value.toString() 			<< separator
			<< _STR(enviData[i].sampleTime.toMilliseconds())	<< separator
			<< _STR(enviData[i].error) 				<< separator
			<< "\n";
	}
	return (ret);
}

const StringArray EnviData::toSQL(const EnviData &enviData, const String &dataTable, const String &unitTable)
{
	StringArray queries;
	String sql;

	for (int i=0; i<enviData.getNumValues(); i++)
	{
		sql << "INSERT INTO "
			<< dataTable
			<< "(sourceName, valueName, valueValue, valueTime, valueError, valueUnit)"
			<< " VALUES ("
			<< "'" << enviData.dataSourceName << "',"
			<< "'" << enviData[i].name << "',"
			<< "'" << enviData[i].value.toString() << "',"
			<< "'" << enviData[i].sampleTime.toMilliseconds() << "',"
			<< enviData[i].error << ","
			<< (int)enviData[i].unit << ");";

		queries.add (sql);
	}

	return (queries);
}

const String EnviData::unitToString(const EnviData::Unit unit)
{
	switch (unit)
	{
		case Integer:
			return ("Integer");
		case Float:
			return ("Float");
		case Text:
			return ("Text");
		case Percent:
			return ("%");
		case Volt:
			return ("V");
		case Amp:
			return ("A");
		case Celsius:
			return ("degC");
		case Fahrenheit:
			return ("degF");
		case Decibel:
			return ("dB");
		case Lux:
			return ("lx");
		case Hertz:
			return ("Hz");
		case Ohm:
			return ("Ohm");
		case Farad:
			return ("F");
		case Watt:
			return ("W");
		case KiloWattHour:
			return ("kWH");
		case Unknown:
		default:
			break;
	}
	return ("U");
}

const EnviData::Unit EnviData::stringToUnit(const String &unit)
{
	if (unit == "Integer")
		return (EnviData::Integer);
	if (unit == "Float")
		return (EnviData::Float);
	if (unit == "Text")
		return (EnviData::Text);
	if (unit == "A")
		return (EnviData::Amp);
	if (unit == "V")
		return (EnviData::Volt);
	if (unit == "degC")
		return (EnviData::Celsius);
	if (unit == "dB")
		return (EnviData::Decibel);
	if (unit == "lx")
		return (EnviData::Lux);
	if (unit == "Hz")
		return (EnviData::Hertz);
	if (unit == "Ohm")
		return (EnviData::Ohm);
	if (unit == "F")
		return (EnviData::Farad);
	if (unit == "W")
		return (EnviData::Watt);
	if (unit == "kWH")
		return (EnviData::KiloWattHour);
	if (unit == "%")
		return (EnviData::Percent);
	if (unit == "degF")
		return (EnviData::Fahrenheit);
	return (EnviData::Unknown);
}
