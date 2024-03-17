
#include "SQLiteDatabase.h"
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

bool SQLiteDatabase::executeQuery(const char* sqlQuery) {
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sqlQuery, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error executing SQLite query: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    std::cout << "Table created successfully." << std::endl;

    return true;
}

int SQLiteDatabase::ListingPostInsert(sqlite3_stmt* stmt, Packet* Pkt, Listing& Listing) {
    const char* sql = "INSERT INTO listings (id, image, title, estimated_worth, location, condition, delivery, looking_for, listing_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, (int)(Pkt->GetBody()->User));
    sqlite3_bind_blob(stmt, 2, Listing.ImageStructArray, Pkt->GetHead()->Length - (sizeof(Listing.Title) + sizeof(Listing.EstimatedWorth) + sizeof(Listing.Location) + sizeof(Listing.Condition) + sizeof(Listing.Delivery) + sizeof(Listing.LookingFor)), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, Listing.Title, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, Listing.EstimatedWorth, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, Listing.Location, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, Listing.Condition, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, Listing.Delivery, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, Listing.LookingFor, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, Listing.ListingDate, -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;

        sqlite3_finalize(stmt);
        sqlite3_close(db);

        return -1;
    }

    std::cout << "Data inserted successfully!" << std::endl;

    return 0;
}


int SQLiteDatabase::SignUpWithImageDataInsert(sqlite3_stmt* stmt, Packet* Pkt, SignUp& signup) {
    

    const char* sql = "INSERT INTO UsersWithProfile (id, username, password, email, profile_picture) VALUES (?, ?, ?, ?, ?)";
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, (int)(Pkt->GetBody()->User));
    sqlite3_bind_text(stmt, 2, signup.username, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, signup.password, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, signup.email, -1, SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 5, signup.ImageStructArray, Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email)), SQLITE_STATIC);

    int rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;

        sqlite3_finalize(stmt);

        return -1;
    }

    std::cout << "Data inserted successfully!" << std::endl;

    return 0;
}



int SQLiteDatabase::SignUpWithoutImageDataInsert(sqlite3_stmt* stmt, Packet* Pkt, SignUp& signup) {


    const char* sql = "INSERT INTO UsersWithoutProfile (id, username, password, email, profile_picture_SubstituteData) VALUES (?, ?, ?, ?, ?)";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, (int)(Pkt->GetBody()->User));
    sqlite3_bind_text(stmt, 2, signup.username, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, signup.password, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, signup.email, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, signup.ImageStructArray, Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email)), SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;

        sqlite3_finalize(stmt);

        return -1;
    }

    std::cout << "Data inserted successfully!" << std::endl;

    return 0;
}


int SQLiteDatabase::FetchImage(sqlite3_stmt* stmt, int Clientid, char** imageArray, int& imageSize) {
    // Construct the query as a std::string
    std::ostringstream oss;
    oss << "SELECT profile_picture FROM UsersWithProfile WHERE id = " << Clientid;
    std::string query_str = oss.str();



    // Convert the std::string query to a const char*
    const char* query = query_str.c_str();

    //memcpy(query, ("SELECT profile_picture FROM users WHERE id = " + std::to_string(id)).c_str(), ("SELECT profile_picture FROM users WHERE id = " + std::to_string(id)).length());


    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }


    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::cerr << "No data found" << std::endl;
        sqlite3_finalize(stmt);
        return -1;
    }


    // Retrieve BLOB data
    const void* blobArray = sqlite3_column_blob(stmt, 0);

    imageSize = sqlite3_column_bytes(stmt, 0);


    *imageArray = new char[imageSize];

    memcpy(*imageArray, (char*)blobArray, imageSize);

    return 0;
}

sqlite3* SQLiteDatabase::getDB() {
    return db;
}

bool SQLiteDatabase::isOpen() {
    return db != nullptr;
}

void SQLiteDatabase::closeDatabase(sqlite3_stmt* stmt) {
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}