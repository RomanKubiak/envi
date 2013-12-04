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
	: values(other.values)
{
}

EnviData::EnviData(const String &firstValueName, const var firstValueValue, const Time firstValueSampleTime)
{
	if (firstValueName != String::empty)
	{
		EnviData::Value value;
		value.name			= firstValueName;
		value.value			= firstValueValue;
		value. sampleTime	= firstValueSampleTime;

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

EnviData EnviData::createFromCommand(const String &dataCommand)
{
	StringArray lines = StringArray::fromLines(dataCommand);
	EnviData data;

	for (int l=0; l<lines.size(); l++)
	{
		String oneLine = lines[l];

		if (oneLine.startsWith("PUTVAL"))
		{
			/* value */
			StringArray lineTokens = StringArray::fromTokens (oneLine, " ", "\"'");
			String valueString;
			if (lineTokens.size() >= 3)
			{
				/* the name is always second */
				EnviData::Value value;
				value.name = lineTokens[1];

				if (lineTokens[2].startsWith("interval"))
				{
					/* only one option, interval */
					/* move the value one position */
					value.interval	= lineTokens[2].fromFirstOccurrenceOf("=", false, true).getIntValue();
					valueString		= lineTokens[3];
				}
				else
				{
					/* no OPTIONS */
					valueString		= lineTokens[2];
				}

				/* values must have 3 fields UNIT:TIME:VALUE */

				StringArray valueTokens = StringArray::fromTokens (valueString, ":", "\"'");
				if (valueTokens.size() != 3)
				{
					_DBG("EnviData::createFromCommand unsupported PUTVAL: ["+oneLine+"]");
					return (EnviData());
				}
				else
				{
					value.unit		= stringToUnit(valueTokens[0]);
					value.sampleTime	= Time(valueTokens[1].getIntValue());
					value.value		= valueTokens[2];
					
					data.addValue (value);
				}
			}

			_DBG("EnviData::createFromCommand unsupported PUTVAL: ["+oneLine+"]");
		}
		else if (dataCommand.startsWith("PUTNOTIF"))
		{
			/* notification */
			_DBG("EnviData::createFromCommand unsupported PUTNOTIF: ["+oneLine+"]");
		}
	}
	
	return (data);
}

const String EnviData::toString(const EnviData &enviData)
{
	String ret;

	for (int i=0; i<enviData.getNumValues(); i++)
	{
		ret << enviData[i].name+", "+enviData[i].value.toString()+", "+enviData[i].sampleTime.toString(true,true,true,true)+"\n";
	}

	return (ret);
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
		case Volt:
			return ("V");
		case Amp:
			return ("A");
		case Celsius:
			return ("C");
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
	if (unit == "C")
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
	return (EnviData::Unknown);
}