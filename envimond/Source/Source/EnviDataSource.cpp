/*
  ==============================================================================

    EnviDataSource.cpp
    Created: 4 Dec 2013 1:54:19pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDataSource.h"
#include "EnviApplication.h"

EnviData::EnviData()
{
}

EnviData::EnviData(const EnviData &other)
	:	values(other.values),
		dataSourceName(other.dataSourceName),
		dataSourceInstanceNumber(other.dataSourceInstanceNumber)
{
}

EnviData::EnviData(const String &firstValueName, const var firstValueValue, const Time firstValueSampleTime)
	: dataSourceInstanceNumber(0)
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
	: dataSourceInstanceNumber(0)
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

void EnviData::copyValues(const EnviData &valuesToUpdateFrom)
{
	values = valuesToUpdateFrom.values;
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

const EnviData EnviData::fromJSON(const String &jsonString, const int dataSourceInstanceNumber)
{
	EnviData enviData;
	var data				= JSON::parse (jsonString);
	enviData.dataSourceName = data["name"];
	enviData.dataSourceType	= data["type"];
	enviData.dataSourceInstanceNumber	= dataSourceInstanceNumber;
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
	ds->setProperty ("type", enviData.dataSourceType);
	ds->setProperty ("instance", enviData.dataSourceInstanceNumber);
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

	for (int i=0; i<enviData.getNumValues(); i++)
	{
		String sql;
		sql << "INSERT INTO "
			<< dataTable
			<< "(sourceId, name, value, timestamp, error, unit)"
			<< " VALUES ("
			<< enviData.dataSourceInstanceNumber				<< ","
			<< "'"	<< enviData[i].name							<< "',"
			<< "'"	<< (float)enviData[i].value					<< "',"
			<< "'"	<< enviData[i].sampleTime.toMilliseconds()	<< "',"
			<<		(int)enviData[i].error						<< ","
			<<		(int)enviData[i].unit						<< ");";

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

EnviDataSource::EnviDataSource(EnviApplication &_owner, const Identifier &_type)
	: disabled(false), owner(_owner), instanceConfig(Ids::dataSource)
{
	setProperty (Ids::type, _type.toString());
}

const Result EnviDataSource::initialize(const ValueTree _instanceConfig)
{
	ScopedLock sl (dataSourceLock);
	instanceConfig 	= _instanceConfig.createCopy();
}

String EnviDataSource::getScopeUID() const
{
	return ("EnviDataSource");
}

Expression EnviDataSource::getSymbolValue(const String &symbol) const
{
	return (Expression(0.0));
}

double EnviDataSource::evaluateFunction (const String &functionName, const double *parameters, int numParameters) const
{
	return (0.0);
}

const var EnviDataSource::getProperty (const Identifier &identifier) const
{
	ScopedLock sl (dataSourceLock);
	return (instanceConfig.getProperty (identifier));
}

void EnviDataSource::setProperty (const Identifier identifier, const var &value)
{
	ScopedLock sl (dataSourceLock);
	instanceConfig.setProperty (identifier, value, nullptr);
}

const String EnviDataSource::getName() const
{
	return (getProperty(Ids::name));
}

void EnviDataSource::setName(const String &name)
{
	setProperty (Ids::name, name);
}

const Identifier EnviDataSource::getType() const
{
	return (getProperty(Ids::type).toString());
}

const int EnviDataSource::getInterval() const
{
	return (getProperty(Ids::interval));
}

const int EnviDataSource::getTimeout() const
{
	return (getProperty(Ids::timeout));
}

const int EnviDataSource::getInstanceNumber() const
{
	return (getProperty(Ids::instance));
}

void EnviDataSource::setInstanceNumber(const int instanceNumber)
{
	setProperty (Ids::instance, instanceNumber);
}

const EnviData EnviDataSource::getResult() const
{
	ScopedLock sl(dataSourceLock);
	return (result);
}

ValueTree EnviDataSource::getConfig() const
{
	ScopedLock sl(dataSourceLock);
	return (instanceConfig);
}

bool EnviDataSource::startSource()
{
	ScopedLock sl(dataSourceLock);
	startTime = Time::getCurrentTime();
	return (execute());
}

void EnviDataSource::stopSource()
{
	ScopedLock sl(dataSourceLock);
	endTime = Time::getCurrentTime();
}

void EnviDataSource::collectFinished(const Result collectStatus)
{
	ScopedLock sl(dataSourceLock);
	owner.sourceWrite (this, collectStatus);
}

void EnviDataSource::setDisabled(const bool shouldBeDisabled)
{
	ScopedLock sl(dataSourceLock);
	disabled = shouldBeDisabled;
}

bool EnviDataSource::isDisabled() const
{
	ScopedLock sl(dataSourceLock);
	return (disabled);
}
