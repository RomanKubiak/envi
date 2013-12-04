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
}

EnviDB::~EnviDB()
{
	sqlite3_close (db);
}

const bool EnviDB::init()
{
	_DBG("EnviDB::init");

	databaseFile = owner.getPropertiesFolder().getChildFile("envi.sqlite3");

	if (databaseFile.existsAsFile())
	{
		_DBG("\tdatabase file exists ["+databaseFile.getFullPathName()+"]");
		if (!openFile())
		{
			_ERR("Failed to open database file");
		}
		else
		{
			_DBG("EnviDB::init file opened, starting thread");

			startThread();

			return (true);
		}
	}
	else
	{
		_WRN("database file does not exist, attempt to create ["+databaseFile.getFullPathName()+"]");

		if (createDatabase())
		{
			startThread();

			return (true);
		}
	}

	return (false);
}

void EnviDB::run()
{
	_DBG("EnviDB::run");

	while (1)
	{
		wait(1000);

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
			}
		}
		else
		{
			continue;
		}
	}
}

int EnviDB::dbCallback(void *object, int argc, char **argv, char **columnName)
{
	EnviDB *enviDB = (EnviDB*)object;
	return (0);
}

void EnviDB::writeResult(EnviDataSource *dataSource)
{
}

bool EnviDB::openFile()
{
	lastResult = sqlite3_open (databaseFile.getFullPathName().toUTF8(), &db);

	if (lastResult)
	{
		_ERR("EnviDB::createDatabase can't create: ["+String(sqlite3_errmsg(db))+"]");
		return (false);
	}

	return (true);
}

bool EnviDB::createDatabase()
{
	if (!openFile())
	{
		return (false);
	}

	_INF("EnviDB::createDatabase database created");

	lastResult = sqlite3_exec (db, DATABASE_SCHEME, dbCallback, this, &lastExecError);

	if (lastResult != SQLITE_OK)
	{
		_ERR("EnviDB::createDatabase can't execute create statement: ["+String(lastExecError)+"]");
		sqlite3_free (lastExecError);
		return (false);
	}

	return (true);
}
