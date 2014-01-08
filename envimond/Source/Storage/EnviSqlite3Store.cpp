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

const Result EnviSqlite3Store::insert(const String &sql, int64 &lastInsertId)
{
	char *errmsg = nullptr;

	if (sqlite3_exec (db, sql.toUTF8(), nullptr, nullptr, &errmsg) != SQLITE_OK)
	{
		String errorMessage = CharPointer_UTF8(errmsg);
		sqlite3_free (errmsg);
		return (Result::fail("insert() failed [" + errorMessage + "]"));
	}
	else
	{
		lastInsertId = sqlite3_last_insert_rowid (db);
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

const Result EnviSqlite3Store::fetchArray(const String &sql, var &destination)
{
	char *errmsg;
	sqlite3_stmt *statement;

	if (sqlite3_prepare_v2 (db, sql.toUTF8(), -1, &statement, nullptr) == SQLITE_OK)
	{
		const int columns = sqlite3_column_count(statement);

		while (sqlite3_step (statement) == SQLITE_ROW)
		{
			var rowData;

            for (int i=0; i<columns; i++)
			{
				switch (sqlite3_column_type (statement, i))
				{
					case SQLITE_INTEGER:
						rowData.append (sqlite3_column_int (statement, i));
						break;
					case SQLITE_FLOAT:
						rowData.append (sqlite3_column_double (statement, i));
						break;
					case SQLITE_BLOB:
						break;
					case SQLITE_NULL:
						rowData.append (var::null);
						break;
					case SQLITE_TEXT:
					default:
						rowData.append (sqlite3_column_text (statement, i));
						break;
				}
			}

			if (rowData != var::null)
				destination.append (rowData);
		}
		if (sqlite3_finalize (statement) != SQLITE_OK)
		{
			return (Result::fail ("sqlite3_finalize failed ["+_STR(sqlite3_errmsg(db))));
		}
	}
	else
	{
		return (Result::fail ("sqlite3_prepare_v2 failed ["+_STR(sqlite3_errmsg(db))));
	}

	return (Result::ok());
}

const Result EnviSqlite3Store::writeRegistration(EnviDataSource *ds)
{
	String sql;
	int64 registrationId;

	sql << "INSERT INTO sources (name, type, instance) VALUES ('"
		<< ds->getName()
		<< "','"
		<< ds->getType()
		<< "',"
		<< ds->getInstanceNumber()
		<< ")";

	const Result res = insert (sql, registrationId);

	if (res.wasOk())
	{
		return (Result::ok());
	}
    else
	{
		return (res);
	}
}

const Result EnviSqlite3Store::registerSources()
{
	_DBG("EnviSqlite3Store::registerSources");

	for (int i=0; i<owner.getNumDataSources(); i++)
	{
		EnviDataSource *ds = owner.getDataSource(i);

		var data;

		if (ds != nullptr)
		{
			Result res = fetchArray ("SELECT id FROM sources WHERE name='"+ds->getName()+"' AND type='"+ds->getType()+"' AND instance="+_STR(ds->getInstanceNumber()), data);
			if (res.wasOk())
			{
				if (data == var::null)
				{
					// DS not registered yet, do that now
					return (writeRegistration (ds));
				}
				else
				{
					if (data[0][0] != var::null)
					{
						_DBG("\t setting index ["+data[0][0].toString()+"] to ds ["+ds->getName()+"]");
						ds->setIndex (data[0][0]);
					}
				}
				_DBG("\t got data for ds\n"+JSON::toString (data));
			}
			else
			{
				_WRN("Can't get information for ds ["+res.getErrorMessage()+"]");
			}
		}
	}

	return (Result::ok());
}
