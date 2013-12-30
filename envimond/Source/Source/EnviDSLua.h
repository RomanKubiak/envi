/*
  ==============================================================================

    EnviDSLua.h
    Created: 30 Dec 2013 12:59:22pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDSLUA_H_INCLUDED
#define ENVIDSLUA_H_INCLUDED

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/RefCountedPtr.h"

#include "EnviIncludes.h"
#include "EnviDataSource.h"

class EnviDSLua : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSLua(EnviApplication &owner);
		~EnviDSLua();
		const Result initialize(const ValueTree _instanceConfig);
		const Result execute();
		void run();
		void handleAsyncUpdate();
		void wrap(lua_State *L);
		const Result startLuaScript(const File &sourceCode);
		void collectDone();
		void setResultValue(const int index, const double value);
		void addResultValue(const char *valueName, const int unit);
		int getNumValues();
		JUCE_LEAK_DETECTOR(EnviDSLua);

	private:
		File sourceCode;
		lua_State *luaState;
};

#endif  // ENVIDSLUA_H_INCLUDED
