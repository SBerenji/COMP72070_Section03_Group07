#include "SQLiteDatabase.h"
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <fstream>

//database.db
SQLiteDatabase::SQLiteDatabase(const std::string& dbPath) : db(nullptr) {
    // Check if the database file exists
    std::ifstream file(dbPath);
    bool fileExists = file.good();
    file.close();

    // If the file does not exist, create it
    if (!fileExists) {
        std::ofstream createFile(dbPath);
        createFile.close();
    }

    // Open the database
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << std::endl;
    }
}

SQLiteDatabase::~SQLiteDatabase() {
        if (db) {
            sqlite3_close(db);
        }
    }

    bool SQLiteDatabase::executeQuery(const std::string& query) {
        char* errMsg;
        int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "Error executing SQLite query: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return false;
        }
        return true;
    }




