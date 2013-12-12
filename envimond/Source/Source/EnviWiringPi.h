#ifndef ENVIWIRINGPI_H_INCLUDED
#define ENVIWIRINGPI_H_INCLUDED

#include "EnviIncludes.h"
#ifdef JUCE_LINUX
#include <wiringPi.h>

class EnviApplication;

class EnviWiringPi
{
	public:
		EnviWiringPi(EnviApplication &_owner);
		~EnviWiringPi();
		void setValid (const bool _valid);
		bool isValid();

	private:
		bool valid;
		EnviApplication &owner;
};

#endif
#endif