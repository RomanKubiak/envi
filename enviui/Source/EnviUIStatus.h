/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_9E5DE1E68874AC0__
#define __JUCE_HEADER_9E5DE1E68874AC0__

//[Headers]     -- You can add your own extra header files here --
#include "EnviUIIncludes.h"
class EnviUIMain;
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Introjucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class EnviUIStatus  : public Component,
                      public Timer
{
public:
    //==============================================================================
    EnviUIStatus (EnviUIMain &_owner);
    ~EnviUIStatus();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback();
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    EnviUIMain &owner;
    String message;
    double progNum;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ProgressBar> prog;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnviUIStatus)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

#endif   // __JUCE_HEADER_9E5DE1E68874AC0__
