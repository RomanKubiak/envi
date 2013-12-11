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

	EnviApplication *enviApplication = new EnviApplication(argc, argv);

	if (enviApplication->isValid())
	{
		enviApplication->runDispatchLoop();
	}

	deleteAndZero (enviApplication);

	return (0);
}
