/*
  ==============================================================================

    EnviUIMainWindow.cpp
    Created: 15 Jan 2014 4:09:22pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviUIMainWindow.h"
#include "EnviUIMain.h"

EnviUIMainWindow::EnviUIMainWindow()  : DocumentWindow ("Envi", Colours::lightgrey, DocumentWindow::allButtons)
{
	enviUILogWindow	= new EnviUILogWindow();
	setUsingNativeTitleBar (true);
	setContentOwned (new EnviUIMain(), true);
	centreWithSize (getWidth(), getHeight());
	setVisible (true);
	Logger::setCurrentLogger (enviUILogWindow);
}

EnviUIMainWindow::~EnviUIMainWindow()
{
}

void EnviUIMainWindow::closeButtonPressed()
{
	JUCEApplication::getInstance()->systemRequestedQuit();
}

void EnviUIMainWindow::moved()
{
	enviUILogWindow->setTopLeftPosition (getX()+getWidth()+8, getY());
}

void EnviUIMainWindow::resized()
{
	DocumentWindow::resized();
}
