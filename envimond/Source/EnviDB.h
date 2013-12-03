/*
  ==============================================================================

    EnviDB.h
    Created: 3 Dec 2013 8:44:34pm
    Author:  Administrator

  ==============================================================================
*/

#ifndef ENVIDB_H_INCLUDED
#define ENVIDB_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataSource.h"
#include "sqlite3.h"

class EnviApplication;

#define DATABASE_SCHEME "\
PRAGMA foreign_keys=OFF;\
BEGIN TRANSACTION;\
CREATE TABLE data (id integer primary key autoincrement, name text, type text, result text, time DATETIME);\
CREATE TABLE stats (id integer primary key autoincrement, name text, type text, start DATETIME, end DATETIME);\
COMMIT;\
"

class EnviDB : public Thread
{
	public:
		EnviDB(EnviApplication &_owner);
		const bool init();
		~EnviDB();
		void run();
		void writeResult(EnviDataSource *dataSource);
		bool createDatabase();
		static int dbCallback(void *object, int argc, char **argv, char **azColName);
	private:
		char *lastExecError;
		sqlite3 *db;
		File databaseFile;
		EnviApplication &owner;
		int lastResult;
};


#endif  // ENVIDB_H_INCLUDED
