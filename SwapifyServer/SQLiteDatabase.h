#pragma once
#include "Packet.h"
#include <iostream>
#include <winsqlite/winsqlite3.h>
#include <string>
#include <sstream>

class SQLiteDatabase
{
private:
    sqlite3* db;

public:
    SQLiteDatabase(const std::string& dbPath);

    ~SQLiteDatabase();

    bool executeQuery(const char* sqlQuery);

    int SignUpWithImageDataInsert(sqlite3_stmt** stmt, Packet* Pkt, SignUp& signup);

    int SignUpWithoutImageDataInsert(sqlite3_stmt** stmt, Packet* Pkt, SignUp& signup);

    int ListingPostInsert(sqlite3_stmt** stmt, Packet* Pkt, Listing& Listing);

    int FetchListingImage(sqlite3_stmt** stmt, std::string title, char** imageArray, int& imageSize);

    int FetchImage(sqlite3_stmt** stmt, std::string email, char** imageArray, int& imageSize);

    sqlite3* getDB();

    bool isOpen();

    void closeDatabase(sqlite3_stmt** stmt);

};