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
	: disabled(false), owner(_owner), instanceConfig(Ids::dataSource), enviExpScope(*this),
		info(new DynamicObject()), values(new DynamicObject())
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

void EnviDataSource::setStorageIndex(const int64 sourceIndex)
{
	setProperty(Ids::index, sourceIndex);
	{
		ScopedLock sl(dataSourceLock);
	}
}

const int64 EnviDataSource::getStorageIndex() const
{
	return (getProperty(Ids::index));
}

void EnviDataSource::setValueStorageId(const int valueIndex, const int storageId)
{
	setValueProperty (valueIndex, Ids::index, storageId);
}

const int EnviDataSource::getNumValues() const
{
	ScopedLock sl(dataSourceLock);
	if (values.getArray())
		return (values.getArray()->size());
	else 
		return (-1);
}

const String EnviDataSource::getValueName(const int valueIndex) const
{
	return (getValueProperty(valueIndex, Ids::name));
}

const Unit EnviDataSource::getValueUnit(const int valueIndex) const
{
	return ((Unit)(int)getValueProperty(valueIndex, Ids::unit));
}

var &EnviDataSource::getResultRef()
{
	ScopedLock sl(dataSourceLock);
	return (values);
}

var EnviDataSource::getResult() const
{
	return (values);
}

var EnviDataSource::getHistory()
{
	ScopedLock sl(dataSourceLock);
	return (history);
}

void EnviDataSource::collectFinished(const Result collectStatus)
{
	ScopedLock sl(dataSourceLock);

	history.append (getResult());

	if (history.getArray()->size() >= getDataCacheSize())
	{
		history.getArray()->removeLast ();
	}

	owner.sourceWrite (this, collectStatus);
}

void EnviDataSource::setValueProperty(const int valueIndex, const Identifier &propertyName, const var &propertyValue)
{
	ScopedLock sl(dataSourceLock);
	values[valueIndex].getDynamicObject()->setProperty (propertyName, propertyValue);
}

const var EnviDataSource::getValueProperty(const int valueIndex, const Identifier &propertyName) const
{
	ScopedLock sl(dataSourceLock);
	return (values[valueIndex].getDynamicObject()->getProperty (propertyName));
}

void EnviDataSource::setValue (const unsigned int valueIndex, const var value)
{
	ScopedLock sl(dataSourceLock);
	if (valueIndex >= values.getArray()->size())
	{
		_INF("Trying to set a value that's not defined index ["+_STR(valueIndex)+"] value ["+value.toString()+"]");
		return;
	}

	setValueProperty (valueIndex, Ids::value, value);
	setValueProperty (valueIndex, Ids::timestamp, Time::getCurrentTime().toMilliseconds());
}

const int EnviDataSource::addValue(const String &valueName, const Unit unit)
{
	ScopedLock sl(dataSourceLock);
	values.append (var (new DynamicObject()));
	return (values.getArray()->size() - 1);
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

	for (int i=0; i<getResult().getArray()->size(); i++)
	{
		if (hasExpression(getValueProperty(i, Ids::name)))
		{
			String error;
			const double result = valueExpressions[getValueProperty(i, Ids::name)].evaluate(enviExpScope.setData(inputData[i]), error);

			if (!error.isEmpty())
			{
				return (Result::fail ("Evaluating expressions failed for value name ["+getValueProperty(i, Ids::name).toString()+"] reson ["+error+"]"));
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
 * Scope
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
