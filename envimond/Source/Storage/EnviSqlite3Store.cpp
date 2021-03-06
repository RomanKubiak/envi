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

const Result EnviSqlite3Store::storeData(const var &dataToStore)
{
	sqlQueries.addArray (toSQL (dataToStore), 0, -1);

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
		ds->setSourceStorageId (registrationId);
		return (Result::ok());
	}
    else
	{
		return (res);
	}
}

const Result EnviSqlite3Store::registerUnits()
{
	Result transactionResult = transactionBegin ();

	if (transactionResult.wasOk())
	{
		Result deleteResult = transactionExecute ("DELETE FROM units");
		Result insertResult = Result::ok();

		if (deleteResult.wasOk())
		{
			String sql;
			for (int i=0; i<Unknown; i++)
			{
				sql = 	"INSERT INTO units (enumId, symbol, name) VALUES(";
				sql << (int)i;
				sql << ",'";
				sql << unitToSymbol((Unit)i);
				sql << "','";
				sql << unitToName((Unit)i);
				sql << "')";

				insertResult = transactionExecute (sql);
				 if (!insertResult.wasOk())
				 {
				 	break;
				 }
			}

			if (insertResult.wasOk())
			{
				return (transactionCommit());
			}
			else
			{
				transactionRollback();
				return (insertResult);
			}
		}
		else
		{
			return (deleteResult);
		}
	}

	return (transactionResult);
}

const Result EnviSqlite3Store::registerValues()
{
	Result transactionResult = transactionBegin ();

	if (transactionResult.wasOk())
	{
		Result deleteResult = transactionExecute ("DELETE FROM vals");
		Result insertResult = Result::ok();

		if (deleteResult.wasOk())
		{
			for (int i=0; i<owner.getNumDataSources(); i++)
			{
				EnviDataSource *ds = owner.getDataSource(i);

				for (int j=0; j<ds->getNumValues(); j++)
				{
					String sql;
					int64 registrationId;
					sql = 	"INSERT INTO vals (name, unit) VALUES('";
					sql << ds->getValueName (j)
						<< "',"
						<< ds->getValueUnit (j)
						<< ")";

					insertResult = insert (sql, registrationId);

					if (insertResult.wasOk())
					{
                        ds->setValueStorageId (j, registrationId);
					}
					else
					{
						return (transactionRollback());
					}
				}
			}
		}
		else
		{
			return (transactionRollback());
		}
	}

	return (transactionCommit());
}

const Result EnviSqlite3Store::registerSources()
{
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
					const Result writeResult = writeRegistration (ds);
					if (!writeResult.wasOk())
					{
						return (writeResult);
					}
					else
					{
						continue;
					}
				}

				if (data.isArray() && data[0][0] != var::null)
				{
					ds->setSourceStorageId (data[0][0]);
				}
			}
			else
			{
				_WRN("Can't get information for ds ["+res.getErrorMessage()+"]");
			}
		}
	}

	return (Result::ok());
}

const StringArray EnviSqlite3Store::toSQL(const var &enviValue)
{
	StringArray result;

	if (enviValue.getArray() == nullptr)
		return (result);

	for (int i=0; i<enviValue.getArray()->size(); i++)
	{
        var &v = enviValue.getArray()->getReference (i);

        String sql = "INSERT INTO data (sourceId, valueId, value, error, timestamp) VALUES (";
        sql << (int64)v.getProperty(Ids::sourceStorageId, var::null) 	<< ","
			<< (int64)v.getProperty(Ids::valueStorageId, var::null) 	<< ","
			<< (double)v.getProperty(Ids::value, var::null) 			<< ","
			<< (int)v.getProperty(Ids::error, var::null) 				<< ","
			<< (int64)v.getProperty(Ids::timestamp, var::null)			<< ")";

        result.add (sql);
	}
	return (result);
}
