/*
  ==============================================================================

    EnviDSJavascript.h
    Created: 23 Dec 2013 7:11:04pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDSJAVASCRIPT_H_INCLUDED
#define ENVIDSJAVASCRIPT_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataSource.h"
class EnviApplication;
class EnviDSJavascript;

class EnviJS : public DynamicObject
{
	public:
		EnviJS (EnviDSJavascript &_owner);
		~EnviJS();
		EnviDSJavascript &getOwner();

		static Identifier getClassName();
		static var logMessage (const var::NativeFunctionArgs& args);

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnviJS)

	private:
		EnviDSJavascript &owner;
};

class EnviDSJavascript : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSJavascript(EnviApplication &_owner);
		~EnviDSJavascript();
		const Result initialize(const ValueTree _instanceConfig);
		const Result execute();
		void run();
		void handleAsyncUpdate();

		JUCE_LEAK_DETECTOR(EnviDSJavascript);

	private:
		JavascriptEngine engine;
};

#endif  // ENVIDSJAVASCRIPT_H_INCLUDED
