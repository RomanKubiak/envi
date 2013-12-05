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

	ScopedPointer <EnviApplication> enviApplication (new EnviApplication(argc, argv));

	const bool ret = enviApplication->runDispatchLoop();
	return (ret);
}
