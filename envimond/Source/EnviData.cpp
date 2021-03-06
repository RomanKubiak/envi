/*
  ==============================================================================

    EnviData.cpp
    Created: 20 Dec 2013 10:34:06am
    Author:  rk09631

  ==============================================================================
*/

#include "EnviData.h"

EnviData::EnviData() : sourceId(-1)
{
}

EnviData::EnviData(const EnviData &other)
	:	values(other.values),
		name(other.name),
		instance(other.instance),
		type(other.type),
		sourceId(other.sourceId)
{
}

EnviData::EnviData(const String &firstValueName, const var firstValueValue, const Time firstValueSampleTime)
	: instance(0), sourceId(-1)
{
	if (firstValueName != String::empty)
	{
		EnviData::Value value;
		value.name			= firstValueName;
		value.value			= firstValueValue;
		value.timestamp		= firstValueSampleTime;

		values.add (value);
	}
}

EnviData::EnviData(const String &firstValueName, const Unit valueUnit)
	: instance(0), sourceId(-1)
{
	if (firstValueName != String::empty)
	{
		EnviData::Value value;
		value.name		= firstValueName;
		value.unit		= valueUnit;

		values.add (value);
	}
}

void EnviData::operator= (const EnviData& other) noexcept
{
	values 		= other.values;
	name 		= other.name;
	instance 	= other.instance;
	type		= other.type;
	sourceId 	= other.sourceId;
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
	for (int i=0; i<values.size(); i++)
	{
		values[i].value 	= valuesToUpdateFrom[i].value;
        values[i].timestamp	= valuesToUpdateFrom[i].timestamp;
	}
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
	return (sizeof(instance) + sizeof(type)+name.length() + sizeof(Value) * getNumValues());
}

const EnviData EnviData::fromJSON(const String &jsonString)
{
	EnviData enviData;
	var data				= JSON::parse (jsonString);
	var values			= data["values"];

	for (int i=0; i<values.size(); i++)
	{
		EnviData::Value value(values[i]["name"], stringToUnit(values[i]["unit"]));

		value.value			= values[i]["value"];
		value.error			= values[i]["error"];
		value.timestamp		= ((int64)values[i]["timestamp"] > 0) ? Time(values[i]["timestamp"]) : Time::getCurrentTime();

		enviData.addValue (value);
	}

	return (enviData);
}

const var EnviData::toVAR(const EnviData &enviData)
{
	String ret;
	DynamicObject *ds = new DynamicObject();
	ds->setProperty ("name", enviData.name);
	ds->setProperty ("type", enviData.type);
	ds->setProperty ("instance", enviData.instance);
	var values;

	for (int i=0; i<enviData.getNumValues(); i++)
	{
		DynamicObject *value = new DynamicObject();
		value->setProperty ("name", enviData[i].name);
		value->setProperty ("value", enviData[i].value.toString());
		value->setProperty ("unit", enviData[i].unit);
		value->setProperty ("error", enviData[i].error);
		value->setProperty ("timestamp", enviData[i].timestamp.toMilliseconds());

		values.append (value);
	}
	ds->setProperty ("values", values);

	return (var(ds));
}

const String EnviData::toJSON(const EnviData &enviData)
{
	return (JSON::toString(toVAR(enviData)));
}

const String EnviData::toCSVString(const EnviData &enviData, const String &separator)
{
	String ret;
	for (int i=0; i<enviData.getNumValues(); i++)
	{
		ret
			<< enviData.name								<< separator
			<< _STR(enviData.instance)						<< separator
			<< enviData.type								<< separator
			<< enviData[i].name 							<< separator
			<< enviData[i].unit				 				<< separator
			<< enviData[i].value.toString() 				<< separator
			<< _STR(enviData[i].timestamp.toMilliseconds())	<< separator
			<< _STR(enviData[i].error) 						<< separator
			<< _STR(enviData.sourceId)						<< separator
			<< _STR(enviData[i].valueId)					<< separator

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
			<< "(sourceId, valueId, valueValue, timestamp, valueError)"
			<< " VALUES ("
			<< enviData.sourceId						<< ","
			<< enviData[i].valueId						<< ","
			<< (float)enviData[i].value					<< ","
			<< enviData[i].timestamp.toMilliseconds()	<< ","
			<< (int)enviData[i].error					<< ")";

		queries.add (sql);
	}

	return (queries);
}

void EnviData::setValueId(const int valueIndex, const int valueId)
{
    if (values.size() >= valueIndex)
		values.getReference(valueIndex).valueId = valueId;
}
