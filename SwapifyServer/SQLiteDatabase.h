#pragma once
#include <iostream>
#include <sqlite3.h>
#include <string>

class SQLiteDatabase
{
private:
	sqlite3* db;

public:
    SQLiteDatabase(const std::string& dbPath);

    ~SQLiteDatabase();

    bool executeQuery(const std::string& query);

};