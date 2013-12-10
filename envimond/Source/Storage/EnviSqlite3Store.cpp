/*
  ==============================================================================

    EnviSqlite3Store.cpp
    Created: 9 Dec 2013 8:34:11pm
    Author:  Administrator

  ==============================================================================
*/

#include "EnviSqlite3Store.h"
#include "EnviApplication.h"

EnviSqlite3Store::EnviSqlite3Store(EnviApplication &owner)
	: EnviDataStore (owner), db(nullptr), queryCacheSize(2)
{
}

EnviSqlite3Store::~EnviSqlite3Store()
{
	closeStore();
}

const Result EnviSqlite3Store::openStore()
{
	_DBG("EnviSqlite3Store::openStore");
	if (owner.getOption(Ids::storeFile).toString().isEmpty())
	{
		storeFile = owner.getPropertiesFolder().getChildFile(Ids::data.toString()+".sqlite3");
	}
	else
	{
		storeFile = File (owner.getOption(Ids::storeFile));
	}

	int rc = sqlite3_open (storeFile.getFullPathName().toUTF8(), &db);

	if (rc)
	{
		sqlite3_close (db);
		return (Result::fail("EnviSqlite3Store::openStore sqlite3_open failed: ["+_STR(sqlite3_errmsg(db))+"]"));
	}
	else
	{
		_DBG("EnviSqlite3Store::openStore sqlite3_opened success for file: ["+storeFile.getFullPathName()+"]");
		return (Result::ok());
	}
}

const Result EnviSqlite3Store::closeStore()
{
	_DBG("EnviSqlite3Store::closeStore");
	sqlite3_close (db);
	return (Result::ok());
}

const Result EnviSqlite3Store::storeData(const EnviData &dataToStore)
{
	sqlQueries.addArray (EnviData::toSQL (dataToStore), 0, -1);

	if (sqlQueries.size() >= queryCacheSize)
	{

		return (flush());
	}

	return (Result::ok());
}

const Result EnviSqlite3Store::flush()
{
	_DBG("EnviSqlite3Store::flush");

	Result res = transactionBegin();

	if (res.wasOk())
	{
		_DBG("\ttransaction started");
		for (int i=0; i<sqlQueries.size(); i++)
		{
			_DBG("\texecute: ["+sqlQueries[i]+"]");
            res = transactionExecute(sqlQueries[i]);

            if (!res.wasOk())
			{
				break;
			}
		}

		return (res = transactionCommit());
	}

	return (res);
}

const Result EnviSqlite3Store::transactionBegin()
{
	char *errorMessage = nullptr;
	const int ret = sqlite3_exec (db, "BEGIN", nullptr, nullptr, &errorMessage);

	if (ret != SQLITE_OK)
	{
		sqlite3_free (errorMessage);
		return (Result::fail("Failed to begin transaction: ["+_STR(errorMessage)+"]"));
	}

	return (Result::ok());
}

const Result EnviSqlite3Store::transactionExecute(const String &query)
{
	char *errorMessage = nullptr;
	const int ret = sqlite3_exec (db, query.toUTF8(), nullptr, nullptr, &errorMessage);

	if (ret != SQLITE_OK)
	{
		sqlite3_free (errorMessage);
		return (Result::fail("Failed to execute query: ["+_STR(errorMessage)+"] query: ["+query+"]"));
	}

	return (Result::ok());
}

const Result EnviSqlite3Store::transactionCommit()
{
	char *errorMessage = nullptr;
	const int ret = sqlite3_exec (db, "COMMIT", nullptr, nullptr, &errorMessage);

	if (ret != SQLITE_OK)
	{
		sqlite3_free (errorMessage);
		return (Result::fail("Failed to begin transaction: ["+_STR(errorMessage)+"]"));
	}

	return (Result::ok());
}

const Result EnviSqlite3Store::transactionRollback()
{
	char *errorMessage = nullptr;
	const int ret = sqlite3_exec (db, "ROLLBACK", nullptr, nullptr, &errorMessage);

	if (ret != SQLITE_OK)
	{
		sqlite3_free (errorMessage);
		return (Result::fail("Failed to begin transaction: ["+_STR(errorMessage)+"]"));
	}

	return (Result::ok());
}

const Result EnviSqlite3Store::rotate()
{
	return (Result::ok());
}

const bool EnviSqlite3Store::isValid()
{
	return (db != nullptr);
}
