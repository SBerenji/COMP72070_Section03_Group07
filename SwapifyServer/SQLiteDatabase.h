#pragma once
#include "PacketWrapper.h"
#include <iostream>
//#include <winsqlite/winsqlite3.h>
#include <winsqlite/winsqlite3.h>
#include <string>

class SQLiteDatabase
{
private:
	sqlite3* db;

public:
    SQLiteDatabase(const std::string& dbPath);

    ~SQLiteDatabase();

    bool executeQuery(const std::string& query);

    sqlite3* getDB();

    bool isOpen();

};