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

		const Result transactionBegin();
        const Result transactionExecute(const String &query);
        const Result transactionCommit();
        const Result transactionRollback();

	private:
		StringArray sqlQueries;
		sqlite3 *db;
		File storeFile;
		int queryCacheSize;
};

#endif  // ENVISQLITE3STORE_H_INCLUDED
