/*
  ==============================================================================

    EnviDSLua.cpp
    Created: 30 Dec 2013 12:59:23pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDSLua.h"
#include "EnviApplication.h"

static void writeToLog (const int lvl, const char *msg)
{
	EnviLog::getInstance()->logMessage(lvl,_STR(msg));
}

EnviDSLua::EnviDSLua(EnviApplication &owner)
	: 	Thread("EnviDSLua"), EnviDataSource(owner, "lua"), luaState(0)
{
}

EnviDSLua::~EnviDSLua()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}

	lua_close(luaState);
}

const Result EnviDSLua::initialize(const ValueTree _instanceConfig)
{
	EnviDataSource::initialize (_instanceConfig);

	if (instanceConfig.isValid())
	{
		sourceCode = owner.getEnviScriptsDir().getChildFile(instanceConfig.getProperty(Ids::source).toString());

		luaState = lua_open();
		luaL_openlibs(luaState);
		wrap(luaState);

		Result compileRes = compileLua(sourceCode);

		if (compileRes.wasOk())
		{
			luabridge::LuaRef luaEnviInit(luaState);
			luaEnviInit = luabridge::getGlobal (luaState, "envi_init");

			try
			{
				luaEnviInit();

				if (getNumValues() <= 0)
				{
					setDisabled (true);
					return (Result::fail ("Init function did not register any values, source disabled"));
				}
			}
			catch (luabridge::LuaException const& e)
			{
				std::cerr && e.what ();
			}
		}
		else
		{
            return (compileRes);
		}
	}

	return (Result::ok());
}

const Result EnviDSLua::execute()
{
	if (!isDisabled())
	{
		if (isThreadRunning())
		{
			notify();
			return (Result::ok());
		}
		else
		{
			startThread();
			return (Result::ok());
		}
	}
	else
	{
		return (Result::fail(getType()+" ["+getName()+"] is disabled"));
	}
}

void EnviDSLua::run()
{
	luabridge::LuaRef luaEnviRun(luaState);
	luaEnviRun = luabridge::getGlobal (luaState, "envi_run");

	while (1)
	{
		do
		{
			try
			{
				luaEnviRun();
			}
			catch (luabridge::LuaException const& e)
			{
				std::cerr && e.what ();
				return;
			}
			SHOULD_WE_EXIT();

		} while (wait (-1));
	}
}

void EnviDSLua::handleAsyncUpdate()
{
	collectFinished (Result::ok());
}

const Result EnviDSLua::compileLua(const File &_sourceCode)
{
	if (_sourceCode.existsAsFile())
	{
		if (luaL_loadfile (luaState, _sourceCode.getFullPathName().toUTF8()))
		{
			return (Result::fail(lua_tostring(luaState, 1)));
		}

		if (lua_pcall (luaState, 0, 0, 0))
		{
			return (Result::fail(lua_tostring(luaState, 1)));
		}
	}

	return (Result::ok());
}

void EnviDSLua::collectDone()
{
	triggerAsyncUpdate();
}

void EnviDSLua::addResultValue(const char *valueName, const int unit)
{
	addValue (_STR(valueName), (Unit)unit);
}

void EnviDSLua::setResultValue(const int index, const double value)
{
	setValue (index, value);
}

int EnviDSLua::getNumValues()
{
	return (EnviDataSource::getNumValues());
}

void EnviDSLua::wrap(lua_State *L)
{
	luabridge::getGlobalNamespace(L)
		.addFunction ("log", &writeToLog)
		.beginClass<EnviDSLua> ("EnviDSLua")
			.addFunction("collectDone", &EnviDSLua::collectDone)
			.addFunction("setResultValue", &EnviDSLua::setResultValue)
			.addFunction("addResultValue", &EnviDSLua::addResultValue)
			.addFunction("getNumValues", &EnviDSLua::getNumValues)
		.endClass();

	luabridge::push (L, this);
	lua_setglobal (L, "envi");
}
