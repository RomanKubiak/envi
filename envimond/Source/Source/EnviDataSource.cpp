/*
  ==============================================================================

    EnviDataSource.cpp
    Created: 4 Dec 2013 1:54:19pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDataSource.h"
#include "EnviApplication.h"



EnviDataSource::EnviDataSource(EnviApplication &_owner, const Identifier &_type)
	: disabled(false), owner(_owner), instanceConfig(Ids::dataSource), enviExpScope(*this)
{
	setType (_type);

	if (owner.getCLI().isSet("data-cache"))
	{
        dataCacheSize = owner.getCLI().getParameter("data-cache").getIntValue();
	}
}

const Result EnviDataSource::initialize(const ValueTree _instanceConfig)
{
	{
		ScopedLock sl (dataSourceLock);
		instanceConfig 	= _instanceConfig.createCopy();
		result.name		= getName();
	}
	return (Result::ok());
}

const var EnviDataSource::getProperty (const Identifier &identifier) const
{
	ScopedLock sl (dataSourceLock);
	return (instanceConfig.getProperty (identifier));
}

const var EnviDataSource::getProperty (const Identifier &identifier, const var defaultReturnValue) const
{
	ScopedLock sl (dataSourceLock);
	return (instanceConfig.getProperty (identifier, defaultReturnValue));
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

const String EnviDataSource::getType() const
{
	return (getProperty(Ids::type).toString());
}

void EnviDataSource::setType (const Identifier _type)
{
	{
		ScopedLock sl (dataSourceLock);
		result.type = _type.toString();
	}

	setProperty (Ids::type, _type.toString());
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

const int EnviDataSource::getDataCacheSize()
{
	return (getProperty (Ids::dataCacheSize, 10));
}

void EnviDataSource::setInstanceNumber(const int instanceNumber)
{
	setProperty (Ids::instance, instanceNumber);
	result.instance = instanceNumber;
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

const Result EnviDataSource::startSource()
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

	if (history.size() >= getDataCacheSize())
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
		_INF("Trying to set a value that's not defined index ["+_STR(valueIndex)+"] value ["+value.toString()+"]");
		return;
	}
	result[valueIndex].value	= value;
	result[valueIndex].timestamp	= Time::getCurrentTime();
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
		result[0].timestamp	= Time::getCurrentTime();
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
		result[0].timestamp	= Time::getCurrentTime();
		result[1].value 	= value1;
		result[1].timestamp	= Time::getCurrentTime();
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
		result[0].timestamp	= Time::getCurrentTime();
		result[1].value 	= value1;
		result[1].timestamp	= Time::getCurrentTime();
		result[2].value 	= value2;
		result[2].timestamp	= Time::getCurrentTime();
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

	if (expressionString.isEmpty() || expressionString == "result")
		return (Result::ok());

	if (valueExpressions.contains (valueName))
	{
		_WRN("Data source: ["+getName()+"], re-setting expression for value: "+valueName);
	}

	try
	{
		Expression exp (expressionString.isEmpty() ? "result" : expressionString);
	}
	catch (Expression::ParseError parseError)
	{
		return (Result::fail("EnviDSCommand failed to parse expression for value: ["+valueName+"] expression: ["+expressionString+"]"));
	}

    valueExpressions.set (valueName, Expression(expressionString.isEmpty() ? "result" : expressionString));
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

	if (hasExpression(valueName))
		return (0.0);

	String error;

	const double result = valueExpressions[valueName].evaluate(enviExpScope.setData(inputData), error);

	if (!error.isEmpty())
	{
		_WRN("Evaluating expression failed source: ["+getName()+"], error: ["+error+"]");
	}
	return (result);
}

const Result EnviDataSource::evaluateAllExpressions(Array <double> inputData)
{
	ScopedLock sl(dataSourceLock);

	for (int i=0; i<getResult().getNumValues(); i++)
	{
		if (hasExpression(getResult()[i].name))
		{
			String error;
			const double result = valueExpressions[getResult()[i].name].evaluate(enviExpScope.setData(inputData[i]), error);

			if (!error.isEmpty())
			{
				return (Result::fail ("Evaluating expressions failed for value name ["+getResult()[i].name+"] reson ["+error+"]"));
			}
			else
			{
				setValue (i, result);
			}
		}
	}

	return (Result::ok());
}

/*
 * Scoped
 */
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

void EnviDataSource::setIndex(const int64 sourceIndex)
{
	setProperty(Ids::index, sourceIndex);
}

const int64 EnviDataSource::getIndex() const
{
	return (getProperty(Ids::index));
}
