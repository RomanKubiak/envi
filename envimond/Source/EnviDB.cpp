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
	enviStore = new EnviFlatFileStore(owner);
	enviStore->openStore();
}

EnviDB::~EnviDB()
{
	if (isThreadRunning())
	{
		notify();
		waitForThreadToExit(1500);
	}
}

const bool EnviDB::init()
{
	if (enviStore->isValid())
	{
		startThread();
		return (true);
	}
	else
	{
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
			_DBG("EnviDB::run queue size: "+String(dataQueue.size()));
			for (int i=0; i<dataQueue.size(); i++)
			{
				enviStore->storeData (*dataQueue[i]);
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
		_DBG("EnviDB::writeResult queue size: "+String(dataQueue.size()));
	}

	notify();
}
