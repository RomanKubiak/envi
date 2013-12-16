/*
  ==============================================================================

    EnviDSBMP085.cpp
    Created: 7 Dec 2013 11:40:35pm
    Author:  Administrator

  ==============================================================================
*/
#ifdef JUCE_LINUX
#include "EnviDSBMP085.h"
#include "EnviApplication.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>

EnviDSBMP085::EnviDSBMP085(EnviApplication &owner)
	: Thread("EnviDSBMP085"), address(-1), EnviDataSource(owner, "bmp085"),fd(-1),
	  ac1(0),ac2(0),ac3(0),ac4(0),ac5(0),ac6(0),b1(0),b2(0),mb(0),mc(0),md(0)
{
}

EnviDSBMP085::~EnviDSBMP085()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
}

const Result EnviDSBMP085::initialize(const ValueTree _instanceConfig)
{
	instanceConfig = _instanceConfig.createCopy();

	if (instanceConfig.isValid())
	{
		address 	= (bool)instanceConfig.hasProperty(Ids::i2cAddr) ? (int)getProperty(Ids::i2cAddr)	: 0x77;
		mode		= (bool)instanceConfig.hasProperty(Ids::i2cAddr) ? (int)getProperty(Ids::mode)		: 0;
	}

	fd = wiringPiI2CSetup(address);

	if (fd < 0)
	{
		return (Result::fail ("wiringPiI2CSetup failed for address: "+String(address)+" mode: "+String(mode)));
	}
	else
	{
		return (Result::ok());
	}
}

const Result EnviDSBMP085::execute()
{
	if (!isDisabled())
	{
		if (isThreadRunning())
		{
			notify();
		}
		else
		{
			startThread();
		}
		return (Result::ok());
	}

	return (Result::ok());
}

void EnviDSBMP085::run()
{
	while (1)
	{
		do
		{
			if (threadShouldExit())
			{
				_DBG("EnviDSBMP085::run thread signalled to exit");
				return;
			}

			triggerAsyncUpdate();
		} while (wait (-1));
	}
}

void EnviDSBMP085::handleAsyncUpdate()
{
        readCalibrationData();
        unsigned int temp = calculateTemperature(readRawTemperature());
        float pres = calculatePressure(readRawPressure());

        _DBG("Temp: "+_STR(temp)+" Pressure: "+String(pres,2));
}

void EnviDSBMP085::readCalibrationData()
{
	_DBG("readCalibrationData()");
	ac1	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_AC1);
        ac2	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_AC2);
        ac3	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_AC3);
        ac4	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_AC4);
        ac5	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_AC5);
        ac6	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_AC6);
        b1	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_B1);
        b2	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_B2);
        mb	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_MB);
        mc	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_MC);
        md	= wiringPiI2CReadReg16 (fd, __BMP085_CAL_MD);

        _DBG("\tAC1 ="+_STR(ac1));
        _DBG("\tAC2 ="+_STR(ac2));
        _DBG("\tAC3 ="+_STR(ac3));
        _DBG("\tAC4 ="+_STR(ac4));
        _DBG("\tAC5 ="+_STR(ac5));
        _DBG("\tAC6 ="+_STR(ac6));
        _DBG("\tB1 ="+_STR(b1));
        _DBG("\tB2 ="+_STR(b2));
        _DBG("\tMB ="+_STR(mb));
        _DBG("\tMC ="+_STR(mc));
        _DBG("\tMD ="+_STR(md));
}

unsigned int EnviDSBMP085::readRawPressure()
{
        wiringPiI2CWriteReg8 (fd, __BMP085_CONTROL, __BMP085_READPRESSURECMD + (mode << 6));

        usleep((2 + (3<<mode)) * 1000);

        const unsigned char msb 	= wiringPiI2CReadReg8 (fd, __BMP085_PRESSUREDATA);
        const unsigned char lsb 	= wiringPiI2CReadReg8 (fd, __BMP085_PRESSUREDATA+1);
        const unsigned char xlsb	= wiringPiI2CReadReg8 (fd, __BMP085_PRESSUREDATA+2);
        const unsigned int rawPressure	= (((unsigned long)msb << 16) | ((unsigned long)lsb << 8) | (unsigned long)xlsb) >> (8 - mode);

        _DBG("RAW pressure data msb: "+_STR(msb)+" lsb: "+_STR(lsb)+" xlsb: "+_STR(xlsb));
        _DBG("RAW pressure:"+_STR(rawPressure));
        return (rawPressure);
}

unsigned int EnviDSBMP085::readRawTemperature()
{
        wiringPiI2CWriteReg8 (fd, __BMP085_CONTROL, __BMP085_READTEMPCMD);
        usleep (7000);
        const unsigned int rawTemperature = wiringPiI2CReadReg16 (fd, __BMP085_TEMPDATA);

        _DBG("RAW temperature: "+_STR(rawTemperature));
        return (rawTemperature);
}

unsigned int EnviDSBMP085::calculateTemperature(unsigned int ut)
{
	int x1, x2;

	x1 = (((int)ut - (int)ac6)*(int)ac5) >> 15;
	x2 = ((int)mc << 11)/(x1 + md);
	b5 = x1 + x2;

	unsigned int result = ((b5 + 8)>>4);

	return result;
}

long EnviDSBMP085::calculatePressure(long rawPressure)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<mode) + 2)>>2;

  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(rawPressure - b3) * (50000>>mode));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  long temp = p;
  return temp;
}

#endif
