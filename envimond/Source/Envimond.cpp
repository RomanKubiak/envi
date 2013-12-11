/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "EnviIncludes.h"
#include "EnviApplication.h"

//==============================================================================

int main (int argc, char* argv[])
{
	ScopedJuceInitialiser_GUI plattform;
	Result returnValue = Result::ok();

	EnviApplication *enviApplication = new EnviApplication(argc, argv);

	if (enviApplication->isValid())
	{
		returnValue = enviApplication->runDispatchLoop();
	}

	deleteAndZero (enviApplication);

	return (returnValue.wasOk() ? 0 : 1);
}
