/*
  ==============================================================================

    EnviUIIncludes.h
    Created: 14 Jan 2014 1:12:04pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIUIINCLUDES_H_INCLUDED
#define ENVIUIINCLUDES_H_INCLUDED

#define _DBG(x)	Logger::writeToLog(x)
#define _ERR(x) Logger::writeToLog ("ERROR: "+String(x));

#include "../JuceLibraryCode/JuceHeader.h"

class EnviIPCallback
{
	public:
		virtual void requestSuccess (const var responseData, const var requestData, StringPairArray responseHeaders) = 0;
		virtual void requestFailed (const var responseData, const var requestData, StringPairArray responseHeaders)  = 0;
};


#endif  // ENVIUIINCLUDES_H_INCLUDED
