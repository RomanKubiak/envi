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
	ScopedPointer <EnviApplication> enviApplication = new EnviApplication(argc, argv);
	enviApplication->messageLoop();
	return (1);
}
