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
	: EnviDataStore (owner), db(nullptr), queryCacheSize(4),
		queryCount(0), transactionCount(0), transactionTimeAvg(0)
{
	if (owner.getCLI().isSet("query-cache"))
	{
		queryCacheSize = owner.getCLI().getParameter("query-cache").getIntValue();
	}
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

const var EnviSqlite3Store::fetchArray(const String &sql)
{
	var result;

	char *errmsg;
	sqlite3_stmt *statement;
	//char *zSQL = sqlite3_mprintf ("SELECT id FROM sources WHERE name='%q' AND type='%q' AND instance=%d", (const char *)ds->getName().toUTF8(), (const char *)ds->getType().toUTF8(), ds->getInstanceNumber());

	if (sqlite3_prepare_v2 (db, sql.toUTF8(), -1, &statement, nullptr) == SQLITE_OK)
	{
		while (sqlite3_step (statement) == SQLITE_ROW)
		{
            for (int i=0; i<sqlite3_column_count(statement); i++)
			{
				switch (sqlite3_column_type (statement, i))
				{
					case SQLITE_INTEGER:
					case SQLITE_FLOAT:
					case SQLITE_BLOB:
					case SQLITE_NULL:
					case SQLITE_TEXT:
					default:
						break;
				}
			}
		}

		if (sqlite3_finalize (statement) != SQLITE_OK)
		{
			lastSqlite3Error = _STR(sqlite3_errmsg(db));
		}
	}
	else
	{
		lastSqlite3Error = _STR(sqlite3_errmsg(db));
	}
}

const Result EnviSqlite3Store::registerSources()
{
	_DBG("EnviSqlite3Store::registerSources");

	for (int i=0; i<owner.getNumDataSources(); i++)
	{
		EnviDataSource *ds = owner.getDataSource(i);

		if (ds != nullptr)
		{

		}
	}

	return (Result::ok());
}
