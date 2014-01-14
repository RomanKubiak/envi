/*
  ==============================================================================

    EnviIncludes.h
    Created: 2 Dec 2013 1:54:19pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIINCLUDES_H_INCLUDED
#define ENVIINCLUDES_H_INCLUDED

// #define	_DBG(x)		Logger::writeToLog(x)
#include "JuceHeader.h"
#include "EnviLog.h"
#include <iostream>
#include <memory>
#include <string>
#include <map>

#define LOG_ERROR				0
#define LOG_WARN				1
#define LOG_INFO				2
#define LOG_DEBUG				3

#define	_STR(x)					String(x)
#define _LOG(lvl,msg)			EnviLog::getInstance()->logMessage(lvl,msg)
#define	_DBG(msg)				_LOG(LOG_DEBUG, msg)
#define _ERR(msg)				_LOG(LOG_ERROR, msg)
#define _WRN(msg)				_LOG(LOG_WARN, msg)
#define _INF(msg)				_LOG(LOG_INFO, msg)
#define _DSERR(msg)				_ERR(getType()+" ["+getName()+"] "+msg)
#define _DSINF(msg)				_INF(getType()+" ["+getName()+"] "+msg)
#define _DSWRN(msg)				_WRN(getType()+" ["+getName()+"] "+msg)
#define _DSDBG(msg)				_DBG(getType()+" ["+getName()+"] "+msg)

#define ENVI_TIMER_OFFSET		10

enum EnviDataSourceIndex
{
	Command_DS	= 0x0000,
	BMP086_DS	= 0x000f,
	DHT11_DS	= 0x00f0
};

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

namespace Ids
{
	#define DECLARE_ID(name)      const Identifier name (#name)
	DECLARE_ID (envi);
	DECLARE_ID (dataSource);
	DECLARE_ID (dataValue);
	DECLARE_ID (dataExp);
	DECLARE_ID (name);
	DECLARE_ID (interval);
	DECLARE_ID (timeout);
	DECLARE_ID (type);
	DECLARE_ID (cmd);
	DECLARE_ID (args);
	DECLARE_ID (dht11);
	DECLARE_ID (dht22);
	DECLARE_ID (pcf8591);
	DECLARE_ID (ds18b20);
	DECLARE_ID (file);
	DECLARE_ID (lua);
	DECLARE_ID (trigger);
	DECLARE_ID (port);
	DECLARE_ID (storeFile);
	DECLARE_ID (i2cAddr);
	DECLARE_ID (gpioBase);
	DECLARE_ID (gpioPin);
	DECLARE_ID (delay);
	DECLARE_ID (iterations);
	DECLARE_ID (data);
	DECLARE_ID (index);
	DECLARE_ID (instance);
	DECLARE_ID (mode);
	DECLARE_ID (pin);
	DECLARE_ID (unit);
	DECLARE_ID (deviceId);
	DECLARE_ID (sourceStorageId);
	DECLARE_ID (valueStorageId);
	DECLARE_ID (stats);
	DECLARE_ID (dataCacheSize);
	DECLARE_ID (path);
	DECLARE_ID (regex);
	DECLARE_ID (regexMatch);
	DECLARE_ID (source);
	DECLARE_ID (value);
	DECLARE_ID (timestamp);
	DECLARE_ID (error);
};

static inline float getRandomFloat (const float limit)
{
	return (Random::getSystemRandom().nextFloat() * limit);
}

static const String unitToSymbol(const Unit unit)
{
	switch (unit)
	{
		case Integer:
			return ("int");
		case Float:
			return ("f");
		case Text:
			return ("str");
		case Percent:
			return ("%");
		case Volt:
			return ("V");
		case Amp:
			return ("A");
		case Celsius:
			return ("C");
		case Fahrenheit:
			return ("F");
		case Decibel:
			return ("dB");
		case Lux:
			return ("lx");
		case Hertz:
			return ("Hz");
		case Ohm:
			return ("Ohm");
		case Farad:
			return ("Farad");
		case Watt:
			return ("Watt");
		case Pascal:
			return ("Pascal");
		case Unknown:
		default:
			break;
	}
	return ("Unknown");
}

static const String unitToName(const Unit unit)
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
			return ("Percent");
		case Volt:
			return ("Volt");
		case Amp:
			return ("Ampere");
		case Celsius:
			return ("Celsius");
		case Fahrenheit:
			return ("Fahrenheit");
		case Decibel:
			return ("Decibel");
		case Lux:
			return ("Lux");
		case Hertz:
			return ("Hertz");
		case Ohm:
			return ("Ohm");
		case Farad:
			return ("Farad");
		case Watt:
			return ("Watt");
		case Pascal:
			return ("Pascal");
		case Unknown:
		default:
			break;
	}
	return ("Unknown");
}

static const Unit stringToUnit(const String &unit)
{
	if (unit == "Integer" || unit == "int")
		return (Integer);
	if (unit == "Float" || unit == "f")
		return (Float);
	if (unit == "Text" || unit == "str")
		return (Text);
	if (unit == "A" || unit == "Amp")
		return (Amp);
	if (unit == "V" || unit == "Volt")
		return (Volt);
	if (unit == "C" || unit == "Celsius")
		return (Celsius);
	if (unit == "dB" || unit == "Decibel")
		return (Decibel);
	if (unit == "lx" || unit == "Lux")
		return (Lux);
	if (unit == "Hz" || unit == "Hertz")
		return (Hertz);
	if (unit == "Ohm")
		return (Ohm);
	if (unit == "F" || unit == "Farad")
		return (Farad);
	if (unit == "W" || unit == "Watt")
		return (Watt);
	if (unit == "%" || unit == "Percent")
		return (Percent);
	if (unit == "Fahrenheit")
		return (Fahrenheit);
	if (unit == "Pa" || unit == "Pascal")
		return (Pascal);
	return (Unknown);
}
#endif  // ENVIINCLUDES_H_INCLUDED
