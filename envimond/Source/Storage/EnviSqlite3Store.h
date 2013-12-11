/*
  ==============================================================================

    EnviSqlite3Store.h
    Created: 9 Dec 2013 8:34:11pm
    Author:  Administrator

  ==============================================================================
*/

#ifndef ENVISQLITE3STORE_H_INCLUDED
#define ENVISQLITE3STORE_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataStore.h"
#include "Externals/sqlite3.h"

#define ENVI_DB_SCHEMA "\
CREATE TABLE IF NOT EXISTS units	(id INTEGER PRIMARY KEY, enumId   INTEGER,  symbol TEXT);\
CREATE TABLE IF NOT EXISTS sources	(id INTEGER PRIMARY KEY, enumId   INTEGER,  name TEXT, interval INTEGER, config TEXT);\
CREATE TABLE IF NOT EXISTS data		(id INTEGER PRIMARY KEY, sourceId INTEGER , name TEXT, value REAL,       unit INTEGER, error INTEGER, timestamp TIMESTAMP);\
"

class EnviSqlite3Store : public EnviDataStore
{
	public:
		EnviSqlite3Store(EnviApplication &owner);
		~EnviSqlite3Store();
		const Result openStore();
		const Result createDataFile();
		const Result closeStore();
        const Result storeData(const EnviData &dataToStore);
        const Result rotate();
		const bool isValid();
		const Result flush();

		const Result createDatabase();
		const Result transactionBegin();
        const Result transactionExecute(const String &query);
        const Result transactionCommit();
        const Result transactionRollback();

		JUCE_LEAK_DETECTOR(EnviSqlite3Store);

	private:
		StringArray sqlQueries;
		sqlite3 *db;
		File storeFile;
		int queryCacheSize;
};

#endif  // ENVISQLITE3STORE_H_INCLUDED
