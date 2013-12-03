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
		return (true);
	}
	else
	{
		_WRN("database file does not exist, attempt to create ["+databaseFile.getFullPathName()+"]");
		return (createDatabase());
	}
}

void EnviDB::run()
{
}

int EnviDB::dbCallback(void *object, int argc, char **argv, char **azColName)
{
	EnviDB *enviDB = (EnviDB*)object;
	return (0);
}

void EnviDB::writeResult(EnviDataSource *dataSource)
{
	_DBG("EnviDB::writeResult");
}

bool EnviDB::createDatabase()
{
	lastResult = sqlite3_open (databaseFile.getFullPathName().toUTF8(), &db);

	if (lastResult)
	{
		_ERR("EnviDB::createDatabase can't create: ["+String(sqlite3_errmsg(db))+"]");
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