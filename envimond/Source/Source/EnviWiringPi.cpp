#include "EnviWiringPi.h"

#ifdef JUCE_LINUX
EnviWiringPi::EnviWiringPi(EnviApplication &_owner) 
	: valid(false), owner(_owner)
{
	_DBG("EnviWiringPi::ctor");

	if (wiringPiSetup() == -1)
	{
		setValid(false);
	}
}

EnviWiringPi::~EnviWiringPi()
{
}

bool EnviWiringPi::isValid()
{
	return (valid);
}

void EnviWiringPi::setValid(const bool _valid)
{
	valid = _valid;
}
#endif