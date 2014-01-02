/*
  ==============================================================================

    EnviDB.cpp
    Created: 3 Dec 2013 8:44:34pm
    Author:  Administrator

  ==============================================================================
*/

#include "EnviDB.h"
#include "EnviApplication.h"

EnviDB::EnviDB(EnviApplication &_owner) : Thread ("Envi/Database"), owner(_owner)
{
	_DBG("EnviDB::ctor");

	if (owner.getCLI().isSet("store-format"))
	{
		if (owner.getCLI().getParameter("store-format") == "csv")
		{
			_INF("Storage format chosen: csv");
			enviStore = new EnviFlatFileStore(owner, EnviFlatFileStore::CSV);
		}
		else
		{
			_INF("Storage format chosen: sqlite3");
			enviStore = new EnviSqlite3Store(owner);
		}
	}
	else
	{
		_INF("Storage format chosen: sqlite3");
		enviStore = new EnviSqlite3Store(owner);
	}
}

EnviDB::~EnviDB()
{
	if (isThreadRunning())
	{
		signalThreadShouldExit();
		waitForThreadToExit(1500);
	}
}

const bool EnviDB::init()
{
	Result res = enviStore->openStore();

	if (res.wasOk())
	{
		res = enviStore->registerSources();
		if (res.wasOk())
		{
			startThread();
			return (true);
		}
		else
		{
			_ERR("EnviDB::init can't register sources ["+res.getErrorMessage()+"]");
			return (false);
		}
	}
	else
	{
		_ERR("EnviDB::init data store failed to open: ["+res.getErrorMessage()+"]");
		return (false);
	}
}

void EnviDB::run()
{
	_DBG("EnviDB::run");

	while (1)
	{
		wait(200);

		if (threadShouldExit())
		{
			_DBG("EnviDB::run thread signaled to exit");
			return;
		}

        ScopedLock sl(dataQueue.getLock());
        if (dataQueue.size() > 0)
		{
			for (int i=0; i<dataQueue.size(); i++)
			{
				const Result res = enviStore->storeData (*dataQueue[i]);
				if (!res.wasOk())
				{
					_WRN("EnviDB::run storeData failed ["+res.getErrorMessage()+"]");
				}
				dataQueue.remove (i, true);
			}
		}
		else
		{
			continue;
		}
	}
}

void EnviDB::writeResult(EnviDataSource *dataSource)
{
	{
		ScopedLock sl(dataQueue.getLock());
		dataQueue.add (new EnviData(dataSource->getResult()));
	}

	notify();
}
