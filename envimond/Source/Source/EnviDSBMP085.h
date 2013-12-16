/*
  ==============================================================================

    EnviDSBMP085.h
    Created: 7 Dec 2013 11:40:35pm
    Author:  Administrator

  ==============================================================================
*/

#ifndef ENVIDSBMP085_H_INCLUDED
#define ENVIDSBMP085_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataSource.h"
#include "EnviWiringPi.h"

#define __BMP085_ULTRALOWPOWER  0
#define __BMP085_STANDARD       1
#define __BMP085_HIGHRES        2
#define __BMP085_ULTRAHIGHRES   3
// BMP085 Registers

#define  __BMP085_CAL_AC1           0xAA  // R   Calibration data (16 bits)
#define  __BMP085_CAL_AC2           0xAC  // R   Calibration data (16 bits)
#define  __BMP085_CAL_AC3           0xAE  // R   Calibration data (16 bits)
#define  __BMP085_CAL_AC4           0xB0  // R   Calibration data (16 bits)
#define  __BMP085_CAL_AC5           0xB2  // R   Calibration data (16 bits)
#define  __BMP085_CAL_AC6           0xB4  // R   Calibration data (16 bits)
#define  __BMP085_CAL_B1            0xB6  // R   Calibration data (16 bits)
#define  __BMP085_CAL_B2            0xB8  // R   Calibration data (16 bits)
#define  __BMP085_CAL_MB            0xBA  // R   Calibration data (16 bits)
#define  __BMP085_CAL_MC            0xBC  // R   Calibration data (16 bits)
#define  __BMP085_CAL_MD            0xBE  // R   Calibration data (16 bits)
#define  __BMP085_CONTROL           0xF4
#define  __BMP085_TEMPDATA          0xF6
#define  __BMP085_PRESSUREDATA      0xF6
#define  __BMP085_READTEMPCMD       0x2E
#define  __BMP085_READPRESSURECMD   0x34

class EnviApplication;

class EnviDSBMP085 : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSBMP085(EnviApplication &_owner);
		~EnviDSBMP085();
		const Result initialize(const ValueTree _instanceConfig);
		const Result execute();
		void run();
		void handleAsyncUpdate();
                void readCalibrationData();
                unsigned int readRawPressure();
                unsigned int readRawTemperature();
                unsigned int calculateTemperature(unsigned int ut);
                long calculatePressure(long rawPressure);
		JUCE_LEAK_DETECTOR(EnviDSBMP085);

	private:
	        int fd;
	        int mode;
	        bool debug;
		int address;
                int ac1,ac2,ac3;
                int ac4,ac5,ac6;
                int b1,b2,mb,mc,md;
                int b5;
};

#endif  // ENVIDSBMP085_H_INCLUDED
