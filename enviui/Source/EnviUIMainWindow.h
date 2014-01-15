/*
  ==============================================================================

    EnviUIMainWindow.h
    Created: 15 Jan 2014 4:10:23pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIUIMAINWINDOW_H_INCLUDED
#define ENVIUIMAINWINDOW_H_INCLUDED

#include "EnviUIIncludes.h"

class EnviUILogWindow : public DocumentWindow, public Logger
{
	public:
		EnviUILogWindow() : DocumentWindow ("Log", Colours::white, DocumentWindow::allButtons, true)
		{
			editor.setMultiLine (true, true);
			editor.setReadOnly (true);
			editor.setFont (Font (Font::getDefaultMonospacedFontName(), 10.0f, Font::plain));

			setContentNonOwned (&editor, true);
			setUsingNativeTitleBar (true);
			setResizable (true, false);
			setVisible (true);
			setSize (400,320);
		}

		void logMessage (const String &message)
		{
			editor.insertTextAtCaret (message + "\n");
		}

		void resized()
		{
            DocumentWindow::resized();
		}
	private:
		TextEditor editor;
};

class EnviUIMainWindow    : public DocumentWindow
{
	public:
		EnviUIMainWindow();
		~EnviUIMainWindow();
        void closeButtonPressed();
        void resized();
        void moved();
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnviUIMainWindow)

	private:
		ScopedPointer <EnviUILogWindow> enviUILogWindow;
};



#endif  // ENVIUIMAINWINDOW_H_INCLUDED
