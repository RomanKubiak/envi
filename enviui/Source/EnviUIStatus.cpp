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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "EnviUIStatus.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include "EnviUIMain.h"
//[/MiscUserDefs]

//==============================================================================
EnviUIStatus::EnviUIStatus (EnviUIMain &_owner)
    : owner(_owner)
{
    addAndMakeVisible (prog = new ProgressBar (progNum));

    addAndMakeVisible (cancelBtn = new TextButton (String::empty));
    cancelBtn->setButtonText (TRANS("Cancel"));
    cancelBtn->addListener (this);
    cancelBtn->setColour (TextButton::buttonColourId, Colours::white);

    addAndMakeVisible (messageLbl = new Label (String::empty,
                                               TRANS("Start")));
    messageLbl->setFont (Font (24.00f, Font::bold));
    messageLbl->setJustificationType (Justification::centred);
    messageLbl->setEditable (false, false, false);
    messageLbl->setColour (Label::textColourId, Colours::white);
    messageLbl->setColour (TextEditor::textColourId, Colours::black);
    messageLbl->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    requestSent 	= false;
    progNum 		= -1;
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    startTimer (250);
    //[/Constructor]
}

EnviUIStatus::~EnviUIStatus()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    prog = nullptr;
    cancelBtn = nullptr;
    messageLbl = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void EnviUIStatus::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xa6000000));

    g.setColour (Colours::white);
    g.setFont (Font (24.00f, Font::plain));
    g.drawText (String::empty,
                0, 0, getWidth() - 0, getHeight() - 0,
                Justification::centred, true);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void EnviUIStatus::resized()
{
    prog->setBounds (proportionOfWidth (0.0501f), (getHeight() / 2) + 24, proportionOfWidth (0.8997f), 24);
    cancelBtn->setBounds (proportionOfWidth (0.2006f), (getHeight() / 2) + 72, proportionOfWidth (0.6003f), 24);
    messageLbl->setBounds (proportionOfWidth (0.0500f), (getHeight() / 2) + -24, proportionOfWidth (0.9000f), 48);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void EnviUIStatus::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == cancelBtn)
    {
        //[UserButtonCode_cancelBtn] -- add your button handler code here..
        //[/UserButtonCode_cancelBtn]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void EnviUIStatus::timerCallback()
{
	if (isVisible())
	{
		if (!requestSent)
		{
			sendRequest();
			return;
		}
		else
		{
			stopTimer();
		}
	}
}

void EnviUIStatus::sendRequest()
{
	_DBG("EnviUIStatus::sendRequest");

	messageLbl->setText ("Connecting", dontSendNotification);
	requestSent 	= true;
}

void EnviUIStatus::requestSuccess (const var responseData, const var requestData, StringPairArray responseHeaders)
{
	_DBG("EnviUIStatus::requestSuccess");
}

void EnviUIStatus::requestFailed (const var responseData, const var requestData, StringPairArray responseHeaders)
{
	_DBG("EnviUIStatus::requestFailed");
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="EnviUIStatus" componentName=""
                 parentClasses="public Component, public Timer, public EnviIPCallback"
                 constructorParams="EnviUIMain &amp;_owner" variableInitialisers="owner(_owner)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="a6000000">
    <TEXT pos="0 0 0M 0M" fill="solid: ffffffff" hasStroke="0" text=""
          fontname="Default font" fontsize="24" bold="0" italic="0" justification="36"/>
  </BACKGROUND>
  <GENERICCOMPONENT name="" id="781af5b297f6f303" memberName="prog" virtualName=""
                    explicitFocusOrder="0" pos="5.014% 24C 89.971% 24" class="ProgressBar"
                    params="progNum"/>
  <TEXTBUTTON name="" id="7cfee92e4563625a" memberName="cancelBtn" virtualName=""
              explicitFocusOrder="0" pos="20.057% 72C 60.029% 24" bgColOff="ffffffff"
              buttonText="Cancel" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="" id="efdef89949a25559" memberName="messageLbl" virtualName=""
         explicitFocusOrder="0" pos="5.014% -24C 89.971% 48" textCol="ffffffff"
         edTextCol="ff000000" edBkgCol="0" labelText="Start" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="24" bold="1" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
