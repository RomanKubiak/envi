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
	: EnviDataStore (owner), db(nullptr), queryCacheSize(1),
		queryCount(0), transactionCount(0), transactionTimeAvg(0)
{
}

EnviSqlite3Store::~EnviSqlite3Store()
{
	closeStore();
}

const Result EnviSqlite3Store::openStore()
{
	storeFile = owner.getEnviStoreFile();

	int rc = sqlite3_open (storeFile.getFullPathName().toUTF8(), &db);

	if (rc)
	{
		sqlite3_close (db);
		return (Result::fail("sqlite3_open failed: ["+_STR(sqlite3_errmsg(db))+"]"));
	}
	else
	{
		_INF("Data store file set to: "+storeFile.getFullPathName());
		return (createDatabase());
	}
}

const Result EnviSqlite3Store::closeStore()
{
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
	Result res = transactionBegin();

	if (res.wasOk())
	{
		for (int i=0; i<sqlQueries.size(); i++)
		{
            res = transactionExecute(sqlQueries[i]);

            if (!res.wasOk())
			{
				_WRN("Transaction failed: ["+res.getErrorMessage()+"]");
				sqlQueries.clear();
				break;
			}
		}

		sqlQueries.clear();
		return (res = transactionCommit());
	}

	sqlQueries.clear();
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
	char *errmsg = nullptr;
	const int ret = sqlite3_exec (db, query.toUTF8(), nullptr, nullptr, &errmsg);

	if (ret != SQLITE_OK)
	{
		String errorMessage = CharPointer_UTF8(errmsg);
		sqlite3_free (errmsg);
		return (Result::fail("Failed to execute query: [" + errorMessage + "] query: ["+query+"]"));
	}

	return (Result::ok());
}

const Result EnviSqlite3Store::transactionCommit()
{
	char *errmsg = nullptr;
	const int ret = sqlite3_exec (db, "COMMIT", nullptr, nullptr, &errmsg);

	if (ret != SQLITE_OK)
	{
		String errorMessage = CharPointer_UTF8(errmsg);
		sqlite3_free (errmsg);
		return (Result::fail("Failed to begin transaction: [" + errorMessage + "]"));
	}

	return (Result::ok());
}

const Result EnviSqlite3Store::transactionRollback()
{
	char *errmsg = nullptr;
	const int ret = sqlite3_exec (db, "ROLLBACK", nullptr, nullptr, &errmsg);

	if (ret != SQLITE_OK)
	{
		String errorMessage = CharPointer_UTF8(errmsg);
		sqlite3_free (errmsg);
		return (Result::fail("Failed to rollback transaction: [" + errorMessage + "]"));
	}

	return (Result::ok());
}

const Result EnviSqlite3Store::createDatabase()
{
	char *errmsg = nullptr;
	const int ret = sqlite3_exec (db, ENVI_DB_SCHEMA, nullptr, nullptr, &errmsg);

	if (ret != SQLITE_OK)
	{
		String errorMessage = CharPointer_UTF8(errmsg);
		sqlite3_free (errmsg);
		return (Result::fail("Failed to create database: [" + errorMessage + "]"));
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
