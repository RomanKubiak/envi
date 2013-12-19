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
		dataSourceInstanceNumber(other.dataSourceInstanceNumber),
		dataSourceType(other.dataSourceType)
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

void EnviData::operator= (const EnviData& other) noexcept
{
	values 				= other.values;
	dataSourceName 			= other.dataSourceName;
	dataSourceInstanceNumber 	= other.dataSourceInstanceNumber;
	dataSourceType			= other.dataSourceType;
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

	enviData.dataSourceName 		= data["name"];
	enviData.dataSourceType			= data["type"];
	enviData.dataSourceInstanceNumber	= dataSourceInstanceNumber;
	var values				= data["values"];

	for (int i=0; i<values.size(); i++)
	{
		EnviData::Value value(values[i]["name"], stringToUnit(values[i]["unit"]));

		value.value			= values[i]["value"];
		value.error			= values[i]["error"];
		value.sampleTime		= ((int64)values[i]["sampleTime"] > 0) ? Time(values[i]["sampleTime"]) : Time::getCurrentTime();
		value.index			= values[i]["index"];

		enviData.addValue (value);
	}

	return (enviData);
}

const var EnviData::toVAR(const EnviData &enviData)
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
			<< enviData.dataSourceName				<< separator
			<< _STR(enviData.dataSourceInstanceNumber)		<< separator
			<< enviData.dataSourceType				<< separator
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
			<< "(sourceName, sourceType, sourceInstance, valueName, valueValue, timestamp, valueError, valueUnit)"
			<< " VALUES ("
			<< "'" << enviData.dataSourceName			<< "',"
			<< "'" << enviData.dataSourceType			<< "',"
			<< (int)enviData.dataSourceInstanceNumber		<< ","
			<< "'"	<< enviData[i].name				<< "',"
			<< "'"	<< (float)enviData[i].value			<< "',"
			<< "'"	<< enviData[i].sampleTime.toMilliseconds()	<< "',"
			<< (int)enviData[i].error				<< ","
			<< (int)enviData[i].unit				<< ");";

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
		case Pascal:
			return ("Pa");
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
	if (unit == "A" || unit == "Amp")
		return (EnviData::Amp);
	if (unit == "V" || unit == "Volt")
		return (EnviData::Volt);
	if (unit == "degC" || unit == "Celsius")
		return (EnviData::Celsius);
	if (unit == "dB" || unit == "Decibel")
		return (EnviData::Decibel);
	if (unit == "lx" || unit == "Lux")
		return (EnviData::Lux);
	if (unit == "Hz" || unit == "Hertz")
		return (EnviData::Hertz);
	if (unit == "Ohm")
		return (EnviData::Ohm);
	if (unit == "F" || unit == "Farad")
		return (EnviData::Farad);
	if (unit == "W" || unit == "Watt")
		return (EnviData::Watt);
	if (unit == "kWH" || unit == "KiloWattHour")
		return (EnviData::KiloWattHour);
	if (unit == "%" || unit == "Percent")
		return (EnviData::Percent);
	if (unit == "degF" || unit == "Fahrenheit")
		return (EnviData::Fahrenheit);
	if (unit == "Pa" || unit == "Pascal")
		return (EnviData::Pascal);
	return (EnviData::Unknown);
}

EnviDataSource::EnviDataSource(EnviApplication &_owner, const Identifier &_type)
	: disabled(false), owner(_owner), instanceConfig(Ids::dataSource), enviExpScope(*this)
{
	setProperty (Ids::type, _type.toString());
}

const Result EnviDataSource::initialize(const ValueTree _instanceConfig)
{
	ScopedLock sl (dataSourceLock);
	instanceConfig 	= _instanceConfig.createCopy();
	return (Result::ok());
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

const int EnviDataSource::getMaxHistorySize()
{
	return (getProperty (Ids::historyMaxSize));
}

void EnviDataSource::setInstanceNumber(const int instanceNumber)
{
	setProperty (Ids::instance, instanceNumber);
	result.dataSourceInstanceNumber = instanceNumber;
}

const EnviData EnviDataSource::getResult() const
{
	ScopedLock sl(dataSourceLock);
	return (result);
}

void EnviDataSource::setResult (const EnviData &_result)
{
	ScopedLock sl(dataSourceLock);
	result = _result;
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

Array <EnviData> EnviDataSource::getHistory()
{
	ScopedLock sl(dataSourceLock);
	return (history);
}

const var EnviDataSource::getSummary()
{
	ScopedLock sl(dataSourceLock);
	var result;
	for (int i=0; i<history.size(); i++)
	{
		result.append (EnviData::toVAR (history[i]));
	}

	return (result);
}

void EnviDataSource::collectFinished(const Result collectStatus)
{
	ScopedLock sl(dataSourceLock);

	history.insert (0, getResult());

	if (history.size() >= getMaxHistorySize())
	{
		history.removeLast ();
	}

	owner.sourceWrite (this, collectStatus);
}

void EnviDataSource::setValue (const unsigned int valueIndex, const var value)
{
	ScopedLock sl(dataSourceLock);
	if (valueIndex >= result.getNumValues())
	{
		_INF("Trying to set a value that's not defined");
		return;
	}
	result[valueIndex].value	= value;
	result[valueIndex].sampleTime	= Time::getCurrentTime();
}

const int EnviDataSource::addValue(const String &valueName, const EnviData::Unit unit)
{
	ScopedLock sl(dataSourceLock);
	result.addValue (EnviData::Value (valueName, unit));
	return (result.getNumValues() - 1);
}

void EnviDataSource::copyValues (const EnviData &dataToCopyFrom)
{
	ScopedLock sl(dataSourceLock);
	result = dataToCopyFrom;
}

void EnviDataSource::setValues (const bool finishCollectNow, const Result collectStatus, const var value0)
{
	{
		ScopedLock sl(dataSourceLock);

		result[0].value 	= value0;
		result[0].sampleTime	= Time::getCurrentTime();
	}

	if (finishCollectNow)
	{
		collectFinished (collectStatus);
	}
}

void EnviDataSource::setValues (const bool finishCollectNow, const Result collectStatus, const var value0, const var value1)
{
	{
		ScopedLock sl(dataSourceLock);

		result[0].value 	= value0;
		result[0].sampleTime	= Time::getCurrentTime();
		result[1].value 	= value1;
		result[1].sampleTime	= Time::getCurrentTime();
	}

	if (finishCollectNow)
	{
		collectFinished (collectStatus);
	}
}

void EnviDataSource::setValues (const bool finishCollectNow, const Result collectStatus, const var value0, const var value1, const var value2)
{
	{
		ScopedLock sl(dataSourceLock);

		result[0].value 	= value0;
		result[0].sampleTime	= Time::getCurrentTime();
		result[1].value 	= value1;
		result[1].sampleTime	= Time::getCurrentTime();
		result[2].value 	= value2;
		result[2].sampleTime	= Time::getCurrentTime();
	}

	if (finishCollectNow)
	{
		collectFinished (collectStatus);
	}
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

const Result EnviDataSource::setAllExpressions()
{
	for (int i=0; i<instanceConfig.getNumChildren(); i++)
	{
		if (instanceConfig.getChild(i).hasType(Ids::dataValue))
		{
			if (instanceConfig.getChild(i).hasProperty(Ids::dataExp) && instanceConfig.getChild(i).hasProperty(Ids::name))
			{
				Result res = setValueExpression(instanceConfig.getChild(i).getProperty(Ids::name), instanceConfig.getChild(i).getProperty(Ids::dataExp));
				if (!res.wasOk())
				{
					return (res);
				}
			}
		}
	}
	return (Result::ok());
}

const Result EnviDataSource::setValueExpression (const String &valueName, const String &expressionString)
{
	ScopedLock sl(dataSourceLock);

	if (valueExpressions.contains (valueName))
	{
		_WRN("Data source: ["+getName()+"], re-setting expression for value: "+valueName);
	}

	try
	{
		Expression exp (expressionString);
	}
	catch (Expression::ParseError parseError)
	{
		return (Result::fail("EnviDSCommand failed to parse expression for value: ["+valueName+"] expression: ["+expressionString+"]"));
	}

    valueExpressions.set (valueName, Expression(expressionString));
    return (Result::ok());
}

const bool EnviDataSource::hasExpression(const String &valueName)
{
	ScopedLock sl(dataSourceLock);
	return (valueExpressions.contains(valueName));
}

const double EnviDataSource::evaluateExpression (const double inputData, const String &valueName)
{
	ScopedLock sl(dataSourceLock);
	String error;

	const double result = valueExpressions[valueName].evaluate(enviExpScope.setData(inputData), error);

	if (!error.isEmpty())
	{
		_WRN("Evaluating expression failed source: ["+getName()+"], error: ["+error+"]");
	}
	return (result);
}

EnviDataSource::EnviExpScope::EnviExpScope(EnviDataSource &_owner): owner(_owner)
{
}

EnviDataSource::EnviExpScope &EnviDataSource::EnviExpScope::setData (const double _inputData)
{
	inputData = _inputData;
	return (*this);
}

String EnviDataSource::EnviExpScope::getScopeUID() const
{
	return ("EnviDataSource");
}

Expression EnviDataSource::EnviExpScope::getSymbolValue(const String &symbol) const
{
	if (symbol == "result")
	{
		return (Expression(inputData));
	}

	return (Expression(0.0));
}

double EnviDataSource::EnviExpScope::evaluateFunction (const String &functionName, const double *parameters, int numParameters) const
{
	if (functionName == "rand_int")
	{
		if (numParameters == 0)
			return (Random::getSystemRandom().nextInt());
		if (numParameters == 1)
			return (Random::getSystemRandom().nextInt((const int)*parameters));
	}
	if (functionName == "rand_double")
	{
		if (numParameters == 0)
			return (Random::getSystemRandom().nextDouble());
		if (numParameters == 1)
			return (Random::getSystemRandom().nextDouble());
	}
	else if (functionName == "abs")
	{
		if (numParameters == 1)
			return (abs(*parameters));
	}
	else if (functionName == "ceil")
	{
		if (numParameters == 1)
			return (ceil(*parameters));
	}
	else if (functionName == "floor")
	{
		if (numParameters == 1)
			return (floor(*parameters));
	}
	else if (functionName == "modf_int")
	{
		if (numParameters == 1)
		{
			double intpart;
			const double fractpart = modf (*parameters , &intpart);
			return (intpart);
		}
	}
	else if (functionName == "modf_fract")
	{
		if (numParameters == 1)
		{
			double intpart;
			const double fractpart = modf (*parameters , &intpart);
			return (fractpart);
		}
	}
	else if (functionName == "sqrt")
	{
		if (numParameters == 1)
			return (sqrt (*parameters));
	}
	else if (functionName == "log")
	{
		if (numParameters == 1)
			return (log (*parameters));
	}
	else if (functionName == "log10")
	{
		if (numParameters == 1)
			return (log10 (*parameters));
	}
	else if (functionName == "exp")
	{
		if (numParameters == 1)
			return (exp(*parameters));
	}
	return (0.0);
}
