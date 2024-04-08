#include <windows.networking.sockets.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SQLiteDatabase.h"
#include "file_utils.h"
#include "RequestLogger.h"
#include <random>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <unordered_map>

#pragma comment(lib, "ws2_32.lib") // Link to the Winsock library

#include "Packet.h"

using namespace std;

///////////////////////////////////////////
extern "C" __declspec(dllexport) SOCKET setupConnection2(WSAStartupFunc wsaStartup = WSAStartup, socketFunc socketfunc = socket, connectFunc connectfunc = connect, WSACleanupFunc wsacleanupfunc = WSACleanup);
extern "C" __declspec(dllexport) int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc = send);
extern "C" __declspec(dllexport) int CloseSocket(SOCKET ClientSocket, closesocketFunc closeFunc = closesocket, WSACleanupFunc wsacleanupfunc = WSACleanup);




int PostReceivedListing(Packet*** Pkt, Listing** doublePtrToList) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    const char* sqlCreateTableListing = "CREATE TABLE IF NOT EXISTS listings ("
        "id INTEGER NOT NULL,"
        "title TEXT NOT NULL,"
        "location TEXT NOT NULL,"
        "condition TEXT NOT NULL,"
        "estimated_worth TEXT NOT NULL,"
        "delivery TEXT NOT NULL,"
        "looking_for TEXT NOT NULL,"
        "listing_picture BLOB NOT NULL,"
        "postID INTEGER PRIMARY KEY);";

    bool query_exe_result = sqldb.executeQuery(sqlCreateTableListing);

    if (!query_exe_result) {
        return -1;
    }

    sqlite3_stmt* stmt = nullptr;

    int ListingPostInsertionReturn = sqldb.ListingPostInsert(&stmt, **Pkt, **doublePtrToList);

    if (ListingPostInsertionReturn == -1) {
        return ListingPostInsertionReturn;
    }

    sqldb.closeDatabase(&stmt);
    stmt = nullptr;


    return 0;
}


int GenerateStartupIDForClient(SOCKET& ConnectionSocket, RequestLogger** loggingInfo, int& clientID) {
    std::string source = "127.0.0.1";
    std::string destination = "127.0.0.1";
    std::string route = "STARTUP_GETID";
    bool auth = false;
    unsigned int DataLength = 0;

    Packet* pkt = CreatePacket();

    memcpy(pkt->GetHead()->Source, source.c_str(), source.length());
    memcpy(pkt->GetHead()->Destination, destination.c_str(), destination.length());
    memcpy(pkt->GetHead()->Route, route.c_str(), route.length());

    pkt->GetHead()->Authorization = auth;
    pkt->GetHead()->Length = DataLength;


    pkt->GetBody()->User = clientID;

    int size = sizeof(pkt->GetBody()->User);


    char* TxBuffer = new char[sizeof(*(pkt->GetHead())) + sizeof(pkt->GetBody()->User)];

    memset(TxBuffer, 0, sizeof(*(pkt->GetHead())) + sizeof(pkt->GetBody()->User));


    memcpy(TxBuffer, pkt->GetHead(), sizeof(*(pkt->GetHead())));

    memcpy(TxBuffer + sizeof(*(pkt->GetHead())), &clientID, sizeof(clientID));

    (**loggingInfo).logPacketSend(&pkt);


    int sendSize = send(ConnectionSocket, TxBuffer, sizeof(*(pkt->GetHead())) + sizeof(pkt->GetBody()->User), 0);


    if (sendSize < 0) {
        std::cout << "Corrupted Data" << std::endl;

        return -1;
    }


    return 0;
}



int DeleteUserRequestedPost(Packet*** Pkt, Listing** list) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    sqlite3_stmt* stmt = nullptr;

    std::ostringstream oss;

    std::cout << (**Pkt)->GetBody()->User << std::endl;

    oss << "SELECT postID FROM listings WHERE " << "(id = " << (int)(**Pkt)->GetBody()->User << ") " << "AND " << "(title = '" << (**list).Title << "') " << "AND " << "(location = '" << (**list).Location << "') " << "AND " << "(condition = '" << (**list).Condition << "') " << "AND " << "(estimated_worth = '" << (**list).EstimatedWorth << "') " << "AND " << "(delivery = '" << (**list).Delivery << "') " << "AND " << "(looking_for = '" << (**list).LookingFor << "') " << ";";



    std::string query_str = oss.str();

    const char* sqlGetPostID = query_str.c_str();



    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlGetPostID, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

        return -1;
    }

    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        std::cout << "Row Returned by Select statement!!" << std::endl;
        std::cout << "Deleting...." << std::endl;
    }

    int postID = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    oss.clear();
    oss.str("");

    query_str.clear();

    oss << "DELETE FROM listings WHERE " << "postID = '" << postID << "';";

    query_str = oss.str();

    const char* sqlDeletePost = query_str.c_str();


    rc = sqlite3_prepare_v2(sqldb.getDB(), sqlDeletePost, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

        return -1;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        std::cout << "Post Successfully Deleted" << std::endl;
    }
    else {
        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

        return -1;
    }

    sqldb.closeDatabase(&stmt);
    stmt = nullptr;

    return 0;
}



int GetCountOfUsersPost(SOCKET& ConnectionSocket, Packet*** Pkt, RequestLogger** logginginfo, Packet** dataTransferPkt) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    sqlite3_stmt* stmt;

    int numOfRows;

    std::ostringstream oss;
    oss << "SELECT COUNT(*) FROM listings WHERE id = '" << (**Pkt)->GetBody()->User << "';";
    std::string query_str = oss.str();

    const char* sqlCountListingRows = query_str.c_str();



    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlCountListingRows, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

        return -1;
    }



    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        numOfRows = sqlite3_column_int(stmt, 0);
        std::cout << "Number of rows in the table: " << numOfRows << std::endl;
    }

    sqldb.closeDatabase(&stmt);
    stmt = nullptr;



    std::string source = "127.0.0.1";
    std::string destination = "127.0.0.1";
    std::string route = "MYPOSTS_COUNT";
    bool auth = true;
    unsigned int length = numOfRows;


    memcpy((*dataTransferPkt)->GetHead()->Source, source.c_str(), source.length());
    memcpy((*dataTransferPkt)->GetHead()->Destination, destination.c_str(), destination.length());
    memcpy((*dataTransferPkt)->GetHead()->Route, route.c_str(), route.length());

    (*dataTransferPkt)->GetHead()->Authorization = auth;
    (*dataTransferPkt)->GetHead()->Length = length;

    char* TxBuffer = new char[sizeof(*((*dataTransferPkt)->GetHead()))];

    memset(TxBuffer, 0, sizeof(*((*dataTransferPkt)->GetHead())));


    memcpy(TxBuffer, (*dataTransferPkt)->GetHead(), sizeof(*((*dataTransferPkt)->GetHead())));

    (**logginginfo).logPacketSend(dataTransferPkt);


    int sendSize = send(ConnectionSocket, TxBuffer, sizeof(*((*dataTransferPkt)->GetHead())), 0);

    if (sendSize < 0) {
        std::cout << "Corrupted Data" << std::endl;
        
        return -1;
    }


    return 0;
}



int SendUsersPostOneAtATime(SOCKET& ConnectionSocket, RequestLogger** logginginfo, Packet*** Pkt, Packet** dataTransferPkt) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    sqlite3_stmt* stmt;

    int numOfRows;

    std::ostringstream oss;

    std::string query_str;

    if ((*dataTransferPkt)->GetHead()->Length > 0) {
        /*oss.clear();
        oss.str("");*/

        oss << "SELECT * FROM listings WHERE id = '" << (**Pkt)->GetBody()->User << "';";
        query_str.clear();

        query_str = oss.str();

        const char* sqlSelectRows = query_str.c_str();


        int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlSelectRows, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

            return -1;
        }


        // Step through each row
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Listing list;

            memset(list.Title, 0, sizeof(list.Title));
            memset(list.Location, 0, sizeof(list.Location));
            memset(list.Condition, 0, sizeof(list.Condition));
            memset(list.EstimatedWorth, 0, sizeof(list.EstimatedWorth));
            memset(list.Delivery, 0, sizeof(list.Delivery));
            memset(list.LookingFor, 0, sizeof(list.LookingFor));

            // Retrieve data from the current row
            int id = sqlite3_column_int(stmt, 0);
            int idSize = sqlite3_column_bytes(stmt, 0);

            const unsigned char* title = sqlite3_column_text(stmt, 1);
            int titleSize = sqlite3_column_bytes(stmt, 1);

            const unsigned char* location = sqlite3_column_text(stmt, 2);
            int locationSize = sqlite3_column_bytes(stmt, 2);

            const unsigned char* condition = sqlite3_column_text(stmt, 3);
            int conditionSize = sqlite3_column_bytes(stmt, 3);

            const unsigned char* estimated_worth = sqlite3_column_text(stmt, 4);
            int worthSize = sqlite3_column_bytes(stmt, 4);

            const unsigned char* delivery = sqlite3_column_text(stmt, 5);
            int deliverySize = sqlite3_column_bytes(stmt, 5);

            const unsigned char* looking_for = sqlite3_column_text(stmt, 6);
            int looking_for_size = sqlite3_column_bytes(stmt, 6);

            // Retrieve BLOB data
            const void* blobArray = sqlite3_column_blob(stmt, 7);

            unsigned int postImageSize = sqlite3_column_bytes(stmt, 7);


            memcpy(&(list.Title), title, titleSize);
            memcpy(&(list.Location), location, locationSize);
            memcpy(&(list.Condition), condition, conditionSize);
            memcpy(&(list.EstimatedWorth), estimated_worth, worthSize);
            memcpy(&(list.Delivery), delivery, deliverySize);
            memcpy(&(list.LookingFor), looking_for, looking_for_size);

            list.ImageStructArray = new char[postImageSize];

            memset(list.ImageStructArray, 0, postImageSize);

            memcpy(list.ImageStructArray, (char*)blobArray, postImageSize);




            int TotalSize = (6 * sizeof(list.Title)) + postImageSize + sizeof(unsigned int);

            char* TxBuffer = new char[TotalSize];

            memset(TxBuffer, 0, TotalSize);

            unsigned int sizeOfImage = postImageSize;


            memcpy(TxBuffer, &(list.Title), sizeof(list.Title));

            memcpy(TxBuffer + sizeof(list.Title), &(list.Location), sizeof(list.Location));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location), &(list.Condition), sizeof(list.Condition));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition), &(list.EstimatedWorth), sizeof(list.EstimatedWorth));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth), &(list.Delivery), sizeof(list.Delivery));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery), &(list.LookingFor), sizeof(list.LookingFor));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor), &sizeOfImage, sizeof(sizeOfImage));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor) + sizeof(sizeOfImage), list.ImageStructArray, sizeOfImage);

            (**logginginfo).logListingSend(&list);
            int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);

            if (sendSize < 0) {
                std::cout << "Corrupted Data" << std::endl;

                return -1;
            }

            delete[] TxBuffer;
            TxBuffer = nullptr;


            TxBuffer = new char[100];

            memset(TxBuffer, 0, 100);

            int recvSize = recv(ConnectionSocket, TxBuffer, 100, 0);


            std::string messageReceived(TxBuffer);

            std::cout << "Message Sent By Client: " << messageReceived << std::endl;
            (**logginginfo).logResponse(messageReceived.c_str());

            delete[] TxBuffer;
            TxBuffer = nullptr;
        }

        sqldb.closeDatabase(&stmt);
        stmt = nullptr;

        return 0;
    }


    else {
        std::cout << "User has No posts!!";
    }

    if (sqlite3_close(sqldb.getDB()) != SQLITE_OK) {
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
    }


    return 1;;
}



int GetUserUploadedProfilePhoto(SOCKET& ConnectionSocket, RequestLogger** logginginfo, Packet*** Pkt, SignUp** signup) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    std::ostringstream oss;

    std::string query;

    int clientID;

    // SQL command to create table
    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithProfile ("
        "id INTEGER NOT NULL,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "email TEXT PRIMARY KEY,"
        "profile_picture BLOB NOT NULL);";


    bool query_exe_result = sqldb.executeQuery(sqlCreateTable);

    if (!query_exe_result) {
        return -1;
    }



    sqlite3_stmt* stmt = nullptr;

    int SignUpdataInsertionReturn = sqldb.SignUpWithImageDataInsert(&stmt, **Pkt, **signup);



    /*const char* sql = "INSERT INTO UsersWithProfile (username, password, email, profile_picture) VALUES (?, ?, ?, ?, ?)";

    sqlite3_prepare_v2(sqldb.getDB(), sql, -1, &stmt, nullptr);

    sqlite3_bind_int(stmt, 1, clientID);
    sqlite3_bind_text(stmt, 2, (**signup).username, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, (**signup).password, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, (**signup).email, -1, SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 5, (**signup).ImageStructArray, (**Pkt)->GetHead()->Length - (sizeof((**signup).username) + sizeof((**signup).password) + sizeof((**signup).email)), SQLITE_STATIC);

    int rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

        sqlite3_finalize(stmt);

        return -1;
    }

    std::cout << "Data inserted successfully!" << std::endl;*/

    if (SignUpdataInsertionReturn == -1) {
        return SignUpdataInsertionReturn;
    }



    // Retrieve BLOB data
    char* imageArray = nullptr;
    int imageSize = 0;

    sqldb.FetchImage(&stmt, (**signup).email, &imageArray, imageSize);



    (**logginginfo).logImageSend(&imageArray, imageSize);

    int sendSize = send(ConnectionSocket, imageArray, imageSize, 0);

    if (sendSize < 0) {
        std::cout << "Sending Image Failed!!" << std::endl;

        return -1;
    }

    else {
        std::cout << "Image Successfully sent!! Wohoooooo" << std::endl;
    }

    delete[] imageArray;

    /*Finalize the statement and close the database connection*/
    sqldb.closeDatabase(&stmt);
    stmt = nullptr;


    return 0;
}



int RegisterUsersWithoutProfilePhoto(Packet*** Pkt, SignUp** signup) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    // SQL command to create table
    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithoutProfile ("
        "id INTEGER NOT NULL,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "email TEXT PRIMARY KEY,"
        "profile_picture_SubstituteData TEXT NOT NULL);";


    bool query_exe_result = sqldb.executeQuery(sqlCreateTable);

    if (!query_exe_result) {
        return -1;
    }



    sqlite3_stmt* stmt = nullptr;

    int SignUpdataInsertionReturn = sqldb.SignUpWithoutImageDataInsert(&stmt, **Pkt, **signup);

    if (SignUpdataInsertionReturn == -1) {
        return SignUpdataInsertionReturn;
    }

    sqldb.closeDatabase(&stmt);
    stmt = nullptr;


    return 0;
}


int LoginCheckUserExists(LogIn** log, char** nameOfTable) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);


    sqlite3_stmt* stmt;

    std::ostringstream oss;
    oss << "SELECT 'UsersWithProfile' "
        "FROM UsersWithProfile "
        "WHERE (username = '" << (**log).username << "')" << " AND" << " (password = '" << (**log).password << "') "
        "UNION ALL "
        "SELECT 'UsersWithoutProfile' "
        "FROM UsersWithoutProfile "
        "WHERE (username = '" << (**log).username << "')" << " AND" << " (password = '" << (**log).password << "');";

    std::string query_str = oss.str();

    const char* sqlTableWhereUserFound = query_str.c_str();



    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlTableWhereUserFound, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

        return -1;
    }

    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

        return -1;
    }

    const char* tableName = (char*)sqlite3_column_text(stmt, 0);

    if (tableName) {
        *nameOfTable = new char[sqlite3_column_bytes(stmt, 0) + 1];

        memset(*nameOfTable, 0, sqlite3_column_bytes(stmt, 0) + 1);

        memcpy(*nameOfTable, tableName, sqlite3_column_bytes(stmt, 0));
    }

    else {
        *nameOfTable = nullptr;
    }

    sqldb.closeDatabase(&stmt);
    stmt = nullptr;

    return 0;
}



int LoginCheck_UserDoesntExist(SOCKET& ConnectionSocket, RequestLogger** logginginfo, char** tableName) {
    if (!(*tableName)) {
        Packet* pkt_login = CreatePacket();

        char source[20] = "127.0.0.1";
        int source_size = sizeof(source);

        char destination[20] = "127.0.0.1";
        int destination_size = sizeof(destination);

        char Route[40] = "LOGIN_USERNOTFOUND";
        int Route_size = sizeof(Route);

        bool Authorization = false;

        int bodyLength = 0;


        memcpy(pkt_login->GetHead()->Source, source, source_size);
        memcpy(pkt_login->GetHead()->Destination, destination, destination_size);
        memcpy(pkt_login->GetHead()->Route, Route, Route_size);

        pkt_login->GetHead()->Authorization = Authorization;

        pkt_login->GetHead()->Length = bodyLength;


        int totalSize = sizeof(*(pkt_login->GetHead())) + pkt_login->GetHead()->Length;

        char* TxBuffer = new char[totalSize];
        memset(TxBuffer, 0, totalSize);


        memcpy(TxBuffer, pkt_login->GetHead(), sizeof(*(pkt_login->GetHead())));

        (**logginginfo).logPacketSend(&pkt_login);
        int sendSize = send(ConnectionSocket, TxBuffer, totalSize, 0);

        if (sendSize < 0) {
            std::cout << "Failed to Send the Data" << std::endl;

            return -1;
        }

        delete[] TxBuffer;
        TxBuffer = nullptr;

        return 0;
    }

    else {
        std::cout << "User Exists" << std::endl;
    }

    return 1;
}


int LoginCheck_UserExistsIn_UsersWithProfile(SOCKET& ConnectionSocket, RequestLogger** logginginfo, char** tableName, LogIn** log, UserCredentials* cred) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);


    sqlite3_stmt* stmt;

    std::ostringstream oss;

    std::string query_str;


    if (strcmp(*tableName, "UsersWithProfile") == 0) {

        query_str.clear();

        oss << "SELECT * FROM " << *tableName << " WHERE (username = '" << (**log).username << "') " << "AND " << "(password = '" << (**log).password << "');";

        query_str = oss.str();

        const char* sqlgetUserCredentials = query_str.c_str();

        // Prepare the SQL statement
        int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlgetUserCredentials, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

            return -1;
        }

        Packet* pkt_login = CreatePacket();

        int imageSize = 0;

        // Execute the SQL statement
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {

            pkt_login->GetBody()->User = sqlite3_column_int(stmt, 0);

            memset((*cred).username, 0, sizeof((*cred).username));
            memcpy((*cred).username, sqlite3_column_text(stmt, 1), sqlite3_column_bytes(stmt, 1));

            memset((*cred).password, 0, sizeof((*cred).password));
            memcpy((*cred).password, sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));

            memset((*cred).email, 0, sizeof((*cred).email));
            memcpy((*cred).email, sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3));

            /*Retrieve BLOB data*/
            const void* blobArray = sqlite3_column_blob(stmt, 4);

            imageSize = sqlite3_column_bytes(stmt, 4);


            (*cred).imageStructArray = new char[imageSize];

            memset((*cred).imageStructArray, 0, imageSize);

            memcpy((*cred).imageStructArray, (char*)blobArray, imageSize);

            blobArray = nullptr;
        }

        else if (rc != SQLITE_DONE) {
            std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

            return -1;
        }


        char source[20] = "127.0.0.1";
        int source_size = sizeof(source);

        char destination[20] = "127.0.0.1";
        int destination_size = sizeof(destination);

        char Route[40] = "LOGIN_USERFOUNDWITHIMAGE";
        int Route_size = sizeof(Route);

        bool Authorization = true;

        int length = sizeof((*cred).username) + sizeof((*cred).password) + sizeof((*cred).email) + imageSize;


        memcpy(pkt_login->GetHead()->Source, source, source_size);
        memcpy(pkt_login->GetHead()->Destination, destination, destination_size);
        memcpy(pkt_login->GetHead()->Route, Route, Route_size);

        pkt_login->GetHead()->Authorization = Authorization;

        pkt_login->GetHead()->Length = length;


        pkt_login->GetBody()->Data = new char[pkt_login->GetHead()->Length];

        memset(pkt_login->GetBody()->Data, 0, pkt_login->GetHead()->Length);

        memcpy(pkt_login->GetBody()->Data, (*cred).username, sizeof((*cred).username));
        memcpy(pkt_login->GetBody()->Data + sizeof((*cred).username), (*cred).password, sizeof((*cred).password));
        memcpy(pkt_login->GetBody()->Data + sizeof((*cred).username) + sizeof((*cred).password), (*cred).email, sizeof((*cred).email));

        memcpy(pkt_login->GetBody()->Data + sizeof((*cred).username) + sizeof((*cred).password) + sizeof((*cred).email), (*cred).imageStructArray, imageSize);


        delete[](*cred).imageStructArray;
        (*cred).imageStructArray = nullptr;



        int totalSize = sizeof(*(pkt_login->GetHead())) + sizeof(pkt_login->GetBody()->User) + pkt_login->GetHead()->Length;

        char* TxBuffer = new char[totalSize];
        memset(TxBuffer, 0, totalSize);


        memcpy(TxBuffer, pkt_login->GetHead(), sizeof(*(pkt_login->GetHead())));
        memcpy(TxBuffer + sizeof(*(pkt_login->GetHead())), &(pkt_login->GetBody()->User), sizeof(pkt_login->GetBody()->User));
        memcpy(TxBuffer + sizeof(*(pkt_login->GetHead())) + sizeof(pkt_login->GetBody()->User), pkt_login->GetBody()->Data, pkt_login->GetHead()->Length);


        (**logginginfo).logPacketSend(&pkt_login);


        int sendSize = send(ConnectionSocket, TxBuffer, totalSize, 0);

        if (sendSize < 0) {
            std::cout << "Failed to Send the Data" << std::endl;

            return -1;
        }

        delete[] TxBuffer;
        TxBuffer = nullptr;

        delete[] pkt_login->GetBody()->Data;
        pkt_login->GetBody()->Data = nullptr;

        sqldb.closeDatabase(&stmt);
        stmt = nullptr;

        return 0;
    }

    else {
        std::cout << "User exists in Table 'UsersWithoutProfile'" << std::endl;

        if (sqlite3_close(sqldb.getDB()) != SQLITE_OK) {
            std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        }
    }

    return 1;
}



int LoginCheck_UserExistsIn_UsersWithoutProfile(SOCKET& ConnectionSocket, RequestLogger** logginginfo, char** tableName, LogIn** log, UserCredentials* cred) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);


    sqlite3_stmt* stmt;

    std::ostringstream oss;

    std::string query_str;


    if (strcmp(*tableName, "UsersWithoutProfile") == 0) {
        oss << "SELECT * FROM " << *tableName << " WHERE (username = '" << (**log).username << "') " << "AND " << "(password = '" << (**log).password << "');";

        query_str = oss.str();

        const char* sqlgetUserCredentials = query_str.c_str();

        // Prepare the SQL statement
        int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlgetUserCredentials, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

            return -1;
        }

        Packet* pkt_login = CreatePacket();


        // Execute the SQL statement
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {

            pkt_login->GetBody()->User = sqlite3_column_int(stmt, 0);

            memset((*cred).username, 0, sizeof((*cred).username));
            memcpy((*cred).username, sqlite3_column_text(stmt, 1), sqlite3_column_bytes(stmt, 1));

            memset((*cred).password, 0, sizeof((*cred).password));
            memcpy((*cred).password, sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));

            memset((*cred).email, 0, sizeof((*cred).email));
            memcpy((*cred).email, sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3));
        }

        else if (rc != SQLITE_DONE) {
            std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

            return -1;
        }



        char source[20] = "127.0.0.1";
        int source_size = sizeof(source);

        char destination[20] = "127.0.0.1";
        int destination_size = sizeof(destination);

        char Route[40] = "LOGIN_USERFOUNDWITHOUTIMAGE";
        int Route_size = sizeof(Route);

        bool Authorization = true;

        int length = sizeof((*cred).username) + sizeof((*cred).password) + sizeof((*cred).email);


        memcpy(pkt_login->GetHead()->Source, source, source_size);
        memcpy(pkt_login->GetHead()->Destination, destination, destination_size);
        memcpy(pkt_login->GetHead()->Route, Route, Route_size);

        pkt_login->GetHead()->Authorization = Authorization;

        pkt_login->GetHead()->Length = length;


        pkt_login->GetBody()->Data = new char[pkt_login->GetHead()->Length];

        memset(pkt_login->GetBody()->Data, 0, pkt_login->GetHead()->Length);

        memcpy(pkt_login->GetBody()->Data, (*cred).username, sizeof((*cred).username));
        memcpy(pkt_login->GetBody()->Data + sizeof((*cred).username), (*cred).password, sizeof((*cred).password));
        memcpy(pkt_login->GetBody()->Data + sizeof((*cred).username) + sizeof((*cred).password), (*cred).email, sizeof((*cred).email));



        int totalSize = sizeof(*(pkt_login->GetHead())) + sizeof(pkt_login->GetBody()->User) + pkt_login->GetHead()->Length;

        char* TxBuffer = new char[totalSize];
        memset(TxBuffer, 0, totalSize);


        memcpy(TxBuffer, pkt_login->GetHead(), sizeof(*(pkt_login->GetHead())));
        memcpy(TxBuffer + sizeof(*(pkt_login->GetHead())), &(pkt_login->GetBody()->User), sizeof(pkt_login->GetBody()->User));
        memcpy(TxBuffer + sizeof(*(pkt_login->GetHead())) + sizeof(pkt_login->GetBody()->User), pkt_login->GetBody()->Data, pkt_login->GetHead()->Length);


        (**logginginfo).logPacketSend(&pkt_login);


        int sendSize = send(ConnectionSocket, TxBuffer, totalSize, 0);

        if (sendSize < 0) {
            std::cout << "Failed to Send the Data" << std::endl;

            return -1;
        }

        delete[] pkt_login->GetBody()->Data;
        pkt_login->GetBody()->Data = nullptr;


        delete[] TxBuffer;
        TxBuffer = nullptr;


        sqldb.closeDatabase(&stmt);
        stmt = nullptr;


        return 0;
    }

    else {
        std::cout << "User doesn't exist in 'UsersWithoutProfile' Table" << std::endl;

        if (sqlite3_close(sqldb.getDB()) != SQLITE_OK) {
            std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        }
    }

    return 1;
}



int CountAndCheckListingsForPreLogin(SOCKET& ConnectionSocket, RequestLogger** logginginfo, Packet** dataTransferPkt) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    sqlite3_stmt* stmt;

    int numOfRows;

    std::ostringstream oss;

    oss << "SELECT COUNT(*) FROM listings;";

    std::string query_str = oss.str();

    const char* sqlCountListingRows = query_str.c_str();



    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlCountListingRows, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

        return -1;
    }



    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        numOfRows = sqlite3_column_int(stmt, 0);
        std::cout << "Number of rows in the table: " << numOfRows << std::endl;
    }



    std::string source = "127.0.0.1";
    std::string destination = "127.0.0.1";
    std::string route = "PRELOGIN_LISTINGS_COUNT";
    bool auth = false;
    unsigned int length = numOfRows;


    memcpy((*dataTransferPkt)->GetHead()->Source, source.c_str(), source.length());
    memcpy((*dataTransferPkt)->GetHead()->Destination, destination.c_str(), destination.length());
    memcpy((*dataTransferPkt)->GetHead()->Route, route.c_str(), route.length());

    (*dataTransferPkt)->GetHead()->Authorization = auth;
    (*dataTransferPkt)->GetHead()->Length = length;

    char* TxBuffer = new char[sizeof(*((*dataTransferPkt)->GetHead()))];

    memset(TxBuffer, 0, sizeof(*((*dataTransferPkt)->GetHead())));


    memcpy(TxBuffer, (*dataTransferPkt)->GetHead(), sizeof(*((*dataTransferPkt)->GetHead())));

    (**logginginfo).logPacketSend(dataTransferPkt);


    int sendSize = send(ConnectionSocket, TxBuffer, sizeof(*((*dataTransferPkt)->GetHead())), 0);

    if (sendSize < 0) {
        std::cout << "Corrupted Data" << std::endl;

        return -1;
    }


    sqldb.closeDatabase(&stmt);
    stmt = nullptr;

    return 0;
}



int SendPreListingIfAnyToClient(SOCKET& ConnectionSocket, RequestLogger** logginginfo, Packet** dataTransferPkt) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    sqlite3_stmt* stmt;

    int numOfRows;

    std::ostringstream oss;

    std::string query_str;


    if ((*dataTransferPkt)->GetHead()->Length > 0) {

        oss << "SELECT * FROM listings;";

        query_str = oss.str();

        const char* sqlSelectRows = query_str.c_str();

        int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlSelectRows, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

            return -1;
        }


        // Step through each row
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Listing list;

            memset(list.Title, 0, sizeof(list.Title));
            memset(list.Location, 0, sizeof(list.Location));
            memset(list.Condition, 0, sizeof(list.Condition));
            memset(list.EstimatedWorth, 0, sizeof(list.EstimatedWorth));
            memset(list.Delivery, 0, sizeof(list.Delivery));
            memset(list.LookingFor, 0, sizeof(list.LookingFor));

            // Retrieve data from the current row
            int id = sqlite3_column_int(stmt, 0);
            int idSize = sqlite3_column_bytes(stmt, 0);

            const unsigned char* title = sqlite3_column_text(stmt, 1);
            int titleSize = sqlite3_column_bytes(stmt, 1);

            const unsigned char* location = sqlite3_column_text(stmt, 2);
            int locationSize = sqlite3_column_bytes(stmt, 2);

            const unsigned char* condition = sqlite3_column_text(stmt, 3);
            int conditionSize = sqlite3_column_bytes(stmt, 3);

            const unsigned char* estimated_worth = sqlite3_column_text(stmt, 4);
            int worthSize = sqlite3_column_bytes(stmt, 4);

            const unsigned char* delivery = sqlite3_column_text(stmt, 5);
            int deliverySize = sqlite3_column_bytes(stmt, 5);

            const unsigned char* looking_for = sqlite3_column_text(stmt, 6);
            int looking_for_size = sqlite3_column_bytes(stmt, 6);

            // Retrieve BLOB data
            const void* blobArray = sqlite3_column_blob(stmt, 7);

            unsigned int postImageSize = sqlite3_column_bytes(stmt, 7);


            memcpy(&(list.Title), title, titleSize);
            memcpy(&(list.Location), location, locationSize);
            memcpy(&(list.Condition), condition, conditionSize);
            memcpy(&(list.EstimatedWorth), estimated_worth, worthSize);
            memcpy(&(list.Delivery), delivery, deliverySize);
            memcpy(&(list.LookingFor), looking_for, looking_for_size);

            list.ImageStructArray = new char[postImageSize];

            memset(list.ImageStructArray, 0, postImageSize);

            memcpy(list.ImageStructArray, (char*)blobArray, postImageSize);




            int TotalSize = (6 * sizeof(list.Title)) + postImageSize + sizeof(unsigned int);

            char* TxBuffer = new char[TotalSize];

            memset(TxBuffer, 0, TotalSize);

            unsigned int sizeOfImage = postImageSize;


            memcpy(TxBuffer, &(list.Title), sizeof(list.Title));

            memcpy(TxBuffer + sizeof(list.Title), &(list.Location), sizeof(list.Location));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location), &(list.Condition), sizeof(list.Condition));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition), &(list.EstimatedWorth), sizeof(list.EstimatedWorth));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth), &(list.Delivery), sizeof(list.Delivery));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery), &(list.LookingFor), sizeof(list.LookingFor));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor), &sizeOfImage, sizeof(sizeOfImage));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor) + sizeof(sizeOfImage), list.ImageStructArray, sizeOfImage);


            (**logginginfo).logListingSend(&list);


            int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);

            if (sendSize < 0) {
                std::cout << "Corrupted Data" << std::endl;

                return -1;
            }

            delete[] list.ImageStructArray;
            list.ImageStructArray = nullptr;

            delete[] TxBuffer;
            TxBuffer = nullptr;



            TxBuffer = new char[100];

            memset(TxBuffer, 0, 100);

            int recvSize = recv(ConnectionSocket, TxBuffer, 100, 0);

            if (recvSize < 0) {
                std::cout << "Receiving Data Corrupted" << std::endl;

                return -1;
            }


            std::string messageReceived(TxBuffer);

            (**logginginfo).logResponse(messageReceived.c_str());

            std::cout << "Message Sent By Client: " << messageReceived << std::endl;


            delete[] TxBuffer;
            TxBuffer = nullptr;
        }

        sqldb.closeDatabase(&stmt);
        stmt = nullptr;

        return 0;
    }

    else {
        std::cout << "No Posts to Show" << std::endl;
    }

    if (sqlite3_close(sqldb.getDB()) != SQLITE_OK) {
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
    }

    return 1;
}


int SendCountOfListingsForPostLogin(SOCKET& ConnectionSocket, RequestLogger** logginginfo, Packet*** Pkt, Packet** dataTransferPacket) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    sqlite3_stmt* stmt;

    int numOfRows;

    std::ostringstream oss;
    oss << "SELECT COUNT(*) FROM listings WHERE id != '" << (**Pkt)->GetBody()->User << "';";
    std::string query_str = oss.str();

    const char* sqlCountListingRows = query_str.c_str();



    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlCountListingRows, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

        return -1;
    }



    // Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        numOfRows = sqlite3_column_int(stmt, 0);
        std::cout << "Number of rows in the table: " << numOfRows << std::endl;
    }




    std::string source = "127.0.0.1";
    std::string destination = "127.0.0.1";
    std::string route = "POSTLOGIN_LISTINGS_COUNT";
    bool auth = true;
    unsigned int length = numOfRows;


    memcpy((*dataTransferPacket)->GetHead()->Source, source.c_str(), source.length());
    memcpy((*dataTransferPacket)->GetHead()->Destination, destination.c_str(), destination.length());
    memcpy((*dataTransferPacket)->GetHead()->Route, route.c_str(), route.length());

    (*dataTransferPacket)->GetHead()->Authorization = auth;
    (*dataTransferPacket)->GetHead()->Length = length;

    char* TxBuffer = new char[sizeof(*((*dataTransferPacket)->GetHead()))];

    memset(TxBuffer, 0, sizeof(*((*dataTransferPacket)->GetHead())));


    memcpy(TxBuffer, (*dataTransferPacket)->GetHead(), sizeof(*((*dataTransferPacket)->GetHead())));

    (**logginginfo).logPacketSend(dataTransferPacket);


    int sendSize = send(ConnectionSocket, TxBuffer, sizeof(*((*dataTransferPacket)->GetHead())), 0);

    if (sendSize < 0) {
        std::cout << "Corrupted Data" << std::endl;

        return -1;
    }

    delete[] TxBuffer;
    TxBuffer = nullptr;


    sqldb.closeDatabase(&stmt);
    stmt = nullptr;


    return 0;
}



int SendPostListingIfAnyToClient(SOCKET& ConnectionSocket, RequestLogger** logginginfo, Packet*** Pkt, Packet** dataTransferPkt) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    sqlite3_stmt* stmt;

    std::ostringstream oss;

    std::string query_str;


    if ((*dataTransferPkt)->GetHead()->Length > 0) {

        oss << "SELECT * FROM listings WHERE id != '" << (**Pkt)->GetBody()->User << "';";
        query_str.clear();

        query_str = oss.str();

        const char* sqlSelectRows = query_str.c_str();


        int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlSelectRows, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

            return -1;
        }




        // Step through each row
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Listing list;

            memset(list.Title, 0, sizeof(list.Title));
            memset(list.Location, 0, sizeof(list.Location));
            memset(list.Condition, 0, sizeof(list.Condition));
            memset(list.EstimatedWorth, 0, sizeof(list.EstimatedWorth));
            memset(list.Delivery, 0, sizeof(list.Delivery));
            memset(list.LookingFor, 0, sizeof(list.LookingFor));

            // Retrieve data from the current row
            int id = sqlite3_column_int(stmt, 0);
            int idSize = sqlite3_column_bytes(stmt, 0);

            const unsigned char* title = sqlite3_column_text(stmt, 1);
            int titleSize = sqlite3_column_bytes(stmt, 1);

            const unsigned char* location = sqlite3_column_text(stmt, 2);
            int locationSize = sqlite3_column_bytes(stmt, 2);

            const unsigned char* condition = sqlite3_column_text(stmt, 3);
            int conditionSize = sqlite3_column_bytes(stmt, 3);

            const unsigned char* estimated_worth = sqlite3_column_text(stmt, 4);
            int worthSize = sqlite3_column_bytes(stmt, 4);

            const unsigned char* delivery = sqlite3_column_text(stmt, 5);
            int deliverySize = sqlite3_column_bytes(stmt, 5);

            const unsigned char* looking_for = sqlite3_column_text(stmt, 6);
            int looking_for_size = sqlite3_column_bytes(stmt, 6);

            // Retrieve BLOB data
            const void* blobArray = sqlite3_column_blob(stmt, 7);

            unsigned int postImageSize = sqlite3_column_bytes(stmt, 7);


            memcpy(&(list.Title), title, titleSize);
            memcpy(&(list.Location), location, locationSize);
            memcpy(&(list.Condition), condition, conditionSize);
            memcpy(&(list.EstimatedWorth), estimated_worth, worthSize);
            memcpy(&(list.Delivery), delivery, deliverySize);
            memcpy(&(list.LookingFor), looking_for, looking_for_size);

            list.ImageStructArray = new char[postImageSize];

            memset(list.ImageStructArray, 0, postImageSize);

            memcpy(list.ImageStructArray, (char*)blobArray, postImageSize);




            int TotalSize = (6 * sizeof(list.Title)) + postImageSize + sizeof(unsigned int);

            char* TxBuffer = new char[TotalSize];

            memset(TxBuffer, 0, TotalSize);

            unsigned int sizeOfImage = postImageSize;


            memcpy(TxBuffer, &(list.Title), sizeof(list.Title));

            memcpy(TxBuffer + sizeof(list.Title), &(list.Location), sizeof(list.Location));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location), &(list.Condition), sizeof(list.Condition));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition), &(list.EstimatedWorth), sizeof(list.EstimatedWorth));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth), &(list.Delivery), sizeof(list.Delivery));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery), &(list.LookingFor), sizeof(list.LookingFor));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor), &sizeOfImage, sizeof(sizeOfImage));

            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor) + sizeof(sizeOfImage), list.ImageStructArray, sizeOfImage);

            (**logginginfo).logListingSend(&list);

            int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);

            if (sendSize < 0) {
                std::cout << "Corrupted Data" << std::endl;

                return -1;
            }

            delete[] list.ImageStructArray;
            list.ImageStructArray = nullptr;


            delete[] TxBuffer;
            TxBuffer = nullptr;



            TxBuffer = new char[100];

            memset(TxBuffer, 0, 100);

            int recvSize = recv(ConnectionSocket, TxBuffer, 100, 0);

            if (recvSize < 0) {
                std::cout << "Received Data Corrupted" << std::endl;

                return -1;
            }


            std::string messageReceived(TxBuffer);

            (**logginginfo).logResponse(messageReceived.c_str());

            std::cout << "Message Sent By Client: " << messageReceived << std::endl;


            delete[] TxBuffer;
            TxBuffer = nullptr;
        }

        sqldb.closeDatabase(&stmt);
        stmt = nullptr;

        return 0;
    }

    else {
        std::cout << "No Listings to show" << std::endl;
    }

    if (sqlite3_close(sqldb.getDB()) != SQLITE_OK) {
        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
    }

    return 1;
}


bool userFound;

int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    userFound = true; // Set the flag to true if any rows are returned

    return 0;
}


int CheckSignUpUserExists(SOCKET& ConnectionSocket, RequestLogger** logginginfo, SignUpCheck** check) {
    std::string dbPath = "database.db";

    SQLiteDatabase sqldb(dbPath);

    userFound = false;

    std::ostringstream oss;

    std::string query;

    oss << "SELECT username, email FROM UsersWithProfile WHERE username = '" << (**check).username << "';";

    std::string withProfile_query_str = oss.str();

    int (*callback_ptr)(void*, int, char**, char**);

    callback_ptr = callback;

    char* errMsg = 0;


    int rc = sqlite3_exec(sqldb.getDB(), withProfile_query_str.c_str(), &callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);

        return -1;
    }


    if (!userFound) {
        oss.clear();

        oss << "SELECT username, email FROM UsersWithoutProfile WHERE username = '" << (**check).username << "';";

        std::string withoutProfile_query_str = oss.str();

        rc = sqlite3_exec(sqldb.getDB(), withoutProfile_query_str.c_str(), &callback, 0, &errMsg);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);

            return -1;
        }
    }

    /*Close the database connection*/
    //sqlite3_close(sqldb.getDB());

    Packet* pkt = CreatePacket();

    char source[20] = "127.0.0.1";
    int source_size = sizeof(source);

    char destination[20] = "127.0.0.1";
    int destination_size = sizeof(destination);

    char Route[40] = "LOGIN";
    int Route_size = sizeof(Route);

    bool Authorization = userFound;

    int length = 0;

    SetHeaderInformation(pkt, source, source_size, destination, destination_size, Route, Route_size, Authorization, length);

    


    


    int TotalSize = 0;

    char* TxBuffer = SerializeUserCheckingData(pkt, TotalSize);

    (**logginginfo).logPacketSend(&pkt);

    int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);

    if (sendSize < 0) {
        std::cout << "Sending User Check Failed!!" << std::endl;

        return -1;
    }

    else {
        std::cout << "Response for User Check Successfully sent!! Wohoooooo" << std::endl;
    }



    if (!Authorization) {
        oss.str("");
        oss.clear();
        query.clear();

        int clientID;

        oss << "SELECT MAX(id) FROM UsersWithProfile;";

        query = oss.str();

        sqlite3_stmt* stmt;


        int rc = sqlite3_prepare_v2(sqldb.getDB(), query.c_str(), -1, &stmt, NULL);

        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

            return -1;
        }


        // Step through each row
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            clientID = sqlite3_column_int(stmt, 0);

            clientID++;
        }


        sqlite3_finalize(stmt);
        stmt = nullptr;


        char* TxBuffer = new char[sizeof(clientID)];


        memset(TxBuffer, 0, sizeof(clientID));


        memcpy(TxBuffer, &clientID, sizeof(clientID));


        int sendSize = send(ConnectionSocket, TxBuffer, sizeof(clientID), 0);

        if (sendSize < 0) {
            std::cout << "Error in Sending" << std::endl;

            return -1;
        }

        /*Close the database connection*/
        sqldb.closeDatabase(&stmt);
    }

    else {
        /*Close the database connection*/
        sqlite3_close(sqldb.getDB());
    }



    DestroyPacket(pkt);

    delete[] TxBuffer;
    TxBuffer = nullptr;


    return 0;
}



int RouteCheck(SOCKET& ConnectionSocket, int clientID, RequestLogger* logginginfo, Packet** Pkt, Listing* ptrToList, SignUp* signup, LogIn* log, SignUpCheck* check) {

    int result_posting_listing;

    int result_generatingClientID;

    int result_delete_userRequested_post;

    int result_GetCountOfUsersPost;

    int result_SendUsersPostOneAtATime;

    int result_GetUserUploadedProfilePhoto;

    int result_RegisterUsersWithoutProfilePhoto;

    int result_LoginCheckUserExists;

    int result_LoginCheck_UserDoesntExist;

    int result_LoginCheck_UserExistsIn_UsersWithProfile;

    int result_LoginCheck_UserExistsIn_UsersWithoutProfile;

    int result_CountAndCheckListingsForPreLogin;

    int result_SendPreListingIfAnyToClient;

    int result_SendCountOfListingsForPostLogin;

    int result_SendPostListingIfAnyToClient;

    int result_CheckSignUpUserExists;


    if (strcmp((*Pkt)->GetHead()->Route, "POST") == 0) {
        result_posting_listing = PostReceivedListing(&Pkt, &ptrToList);
    }


    else if (strcmp((*Pkt)->GetHead()->Route, "STARTUP_GETID") == 0) {
        result_generatingClientID = GenerateStartupIDForClient(ConnectionSocket, &logginginfo, clientID);
    }


    else if (strcmp((*Pkt)->GetHead()->Route, "DELETE_POST") == 0) {
        result_delete_userRequested_post = DeleteUserRequestedPost(&Pkt, &ptrToList);
    }


    else if (strcmp((*Pkt)->GetHead()->Route, "MYPOSTS_COUNT") == 0) {

        Packet* dataTransferPkt = CreatePacket();

        result_GetCountOfUsersPost = GetCountOfUsersPost(ConnectionSocket, &Pkt, &logginginfo, &dataTransferPkt);

        result_SendUsersPostOneAtATime = SendUsersPostOneAtATime(ConnectionSocket, &logginginfo, &Pkt, &dataTransferPkt);

        DestroyPacket(dataTransferPkt);
    }


    else if (strcmp((*Pkt)->GetHead()->Route, "SIGNUP_IMAGEUPLOADED") == 0) {
        result_GetUserUploadedProfilePhoto = GetUserUploadedProfilePhoto(ConnectionSocket, &logginginfo, &Pkt, &signup);
    }


    else if (strcmp((*Pkt)->GetHead()->Route, "SIGNUP_IMAGENOTUPLOADED") == 0) {
        result_RegisterUsersWithoutProfilePhoto = RegisterUsersWithoutProfilePhoto(&Pkt, &signup);
    }


    else if (strcmp((*Pkt)->GetHead()->Route, "LOGIN") == 0) {
        char* tableName = nullptr;

        UserCredentials cred;

        result_LoginCheckUserExists = LoginCheckUserExists(&log, &tableName);

        result_LoginCheck_UserDoesntExist = LoginCheck_UserDoesntExist(ConnectionSocket, &logginginfo, &tableName);

        result_LoginCheck_UserExistsIn_UsersWithProfile = LoginCheck_UserExistsIn_UsersWithProfile(ConnectionSocket, &logginginfo, &tableName, &log, &cred);

        result_LoginCheck_UserExistsIn_UsersWithoutProfile = LoginCheck_UserExistsIn_UsersWithoutProfile(ConnectionSocket, &logginginfo, &tableName, &log, &cred);
    }


    else if (strcmp((*Pkt)->GetHead()->Route, "PRELOGIN_LISTINGS_COUNT") == 0) {
        Packet* dataTransferPkt = CreatePacket();

        result_CountAndCheckListingsForPreLogin = CountAndCheckListingsForPreLogin(ConnectionSocket, &logginginfo, &dataTransferPkt);

        result_SendPreListingIfAnyToClient = SendPreListingIfAnyToClient(ConnectionSocket, &logginginfo, &dataTransferPkt);

        DestroyPacket(dataTransferPkt);
    }

    
    else if (strcmp((*Pkt)->GetHead()->Route, "POSTLOGIN_LISTINGS_COUNT") == 0) {
        Packet* dataTransferPacket = CreatePacket();

        result_SendCountOfListingsForPostLogin = SendCountOfListingsForPostLogin(ConnectionSocket, &logginginfo, &Pkt, &dataTransferPacket);

        result_SendPostListingIfAnyToClient = SendPostListingIfAnyToClient(ConnectionSocket, &logginginfo, &Pkt, &dataTransferPacket);

        DestroyPacket(dataTransferPacket);
    }


    else if (strcmp((*Pkt)->GetHead()->Route, "SIGNUP_USERCHECK") == 0) {
        result_CheckSignUpUserExists = CheckSignUpUserExists(ConnectionSocket, &logginginfo, &check);
    }


    else {
        std::cout << "Invalid Route" << std::endl;

        return -1;
    }


    std::cout << "Success!!" << std::endl;

    return 0;
}











int CloseSocket(SOCKET ClientSocket, closesocketFunc closeFunc, WSACleanupFunc wsacleanupfunc) {

    // Cleaning up the socket
if (closeFunc(ClientSocket) == SOCKET_ERROR)
{
    std::cerr << "Error closing the socket" << std::endl;
    return 0;
}

std::cout << "Client Socket Closed" << std::endl;


// Cleaning up the winsock library
if (wsacleanupfunc() == SOCKET_ERROR)
{
    std::cerr << "Error cleaning WSACleanup" << std::endl;
    return 2;
}

std::cout << "WSA Closed" << std::endl;

return 1;
}

int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc) {

    int sendSize = sendFunc(ClientSocket, TxBuffer, totalSize, 0);

    if (sendSize < 0) {
        std::cout << "Sending Failed" << std::endl;

        CloseSocket(ClientSocket);

        return 0;
    }
    else {
        std::cout << "Message Successfully sent: " << TxBuffer << std::endl;
    }

    return 1;
    // returning 1 upon successful completion.
}


SOCKET setupConnection2(WSAStartupFunc wsaStartup, socketFunc socketfunc, connectFunc connectfunc, WSACleanupFunc wsacleanupfunc) {
    // starting up and configuring the Winsock dynamically linked library

    WSADATA wsaData;

    if (wsaStartup(MAKEWORD(2, 2), &wsaData) != 0)
        // providing the address of the object (&wsaData) into the library's startup function
    {
        std::cout << "ERROR: Failed to start WSA" << std::endl;

        return 0;
    }

    else {
        std::cout << "WSA Started" << std::endl;
    }

    // Create a Socket
    // This is to make the call to the server

    SOCKET ServerSocket;
    ServerSocket = socketfunc(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // here we are using TCP protocol

    if (ServerSocket == INVALID_SOCKET)
    {
        wsacleanupfunc();

        std::cout << "ERROR: Failed to create ServerSocket" << std::endl;

        return 0;
    }

    else {
        std::cout << "Client Socket Created" << std::endl;
    }

    // Connect
    // starting up the three way handshake process
    // The server should have binded and configured a socket for Listening by now.

    sockaddr_in SvrAddr;
    // This object contains necessary information needed by the library to setup the TCP/IP headers correctly.

    SvrAddr.sin_family = AF_INET;
    // The family which specifies the protocol that will be used.

    SvrAddr.sin_port = htons(27000);
    // The client is using the port 27500.

    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // The IPV4 address the server is located on. Since different network we are gonna send the request to the NAT using localhost.

    if ((connectfunc(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR)
    {
        CloseSocket(ServerSocket);

        std::cout << "ERROR: Connection attempted Failed" << std::endl;

        return 0;
    }

    else {
        std::cout << "Connection Established" << std::endl;
    }


    return ServerSocket;
}
////////////////////////////////////////////////////////////////

std::vector<std::thread> threadPool;
std::vector<std::atomic<bool>> cleanupFlags(20);
std::mutex threadMutex;
std::condition_variable cv;


// ******** for unique id ********
std::atomic<unsigned int> nextClientID{ 0 };
std::unordered_map<int, SOCKET> clientIDToConnections;



SOCKET setupConnection(int (*funcptr)(SOCKET)) {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "ERROR: Failed to start WSA" << std::endl;

        return -1;
    }

    SOCKET ServerSocket;
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (ServerSocket == INVALID_SOCKET) {
        WSACleanup();

        std::cout << "ERROR: Failed to create ServerSocket" << std::endl;

        return -1;
    }

    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = INADDR_ANY;
    SvrAddr.sin_port = htons(27000);
    if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
    {
        std::cout << "ERROR: Failed to bind ServerSocket" << std::endl;

        closesocket(ServerSocket);

        WSACleanup();

        return -1;
    }


    if (listen(ServerSocket, 1) == -1) {
        std::cout << "ERROR: listen failed to configure ServerSocket" << std::endl;

        closesocket(ServerSocket);

        WSACleanup();

        return -1;
    }

    int i = 0;

    while (1) {
        SOCKET* ConnectionSocket = new SOCKET();
        if ((*ConnectionSocket = accept(ServerSocket, NULL, NULL)) == -1) {
            continue;
        }


        // printing out a message if the process is succesful
        std::cout << "Server Socket Successfully Binded" << endl;

        std::thread clientThread(funcptr, *ConnectionSocket);

        {
            std::lock_guard<std::mutex> lock(threadMutex);
            threadPool.push_back(std::move(clientThread));

            cleanupFlags[i].store(false);

            i++;
        }


        delete ConnectionSocket;
        ConnectionSocket = nullptr;
    }

    return ServerSocket;
}


void cleanupThreads() {
    while (1) {
        std::unique_lock<std::mutex> lock(threadMutex);

        cv.wait(lock, [&] {
            for (const auto& flag : cleanupFlags) {
                if (flag.load()) return true; // Check if flag is set
            }

        return false;
            });

        // Find and mark threads for cleanup
        for (size_t i = 0; i < cleanupFlags.size(); ++i) {
            if (cleanupFlags[i].load()) {
                if (threadPool[i].joinable()) {
                    threadPool[i].join();
                }

                // Reset the flag
                cleanupFlags[i].store(false);
            }
        }
    }
}





int threadedFunc(SOCKET ConnectionSocket) {

    RequestLogger logginginfo("server_log_file.txt");
    unsigned int clientID = ++nextClientID;
    clientIDToConnections[clientID] = ConnectionSocket;

    char* RxBuffer = new char[2500000];   //declaring a receive buffer with size 128

    while (1) {

        memset(RxBuffer, 0, 2500000);


        // receive the client message by passing the ServerSocket, address and size of the receive buffer, the 0 flag, client address structure, and the size of the client address structure
        //Note: buffer is a pointer to the data (it is an array which means it acts as a pointer to the first element of the array)


        int receive_result = recv(ConnectionSocket, RxBuffer, 2500000, 0);


        // if the return value of the 'recvfrom' function is -1, close the server socket and end the program execution
        if (receive_result <= 0)
        {
            // Simulate client disconnection
            // Wait for 5 seconds
            std::this_thread::sleep_for(std::chrono::seconds(5));

            closesocket(ConnectionSocket);
            /*WSACleanup();*/


            {
                std::lock_guard<std::mutex> lock(threadMutex);

                for (size_t i = 0; i < threadPool.size(); ++i) {
                    if (threadPool[i].get_id() == std::this_thread::get_id()) {
                        cleanupFlags[i] = true;

                        break;
                    }
                }
            }


            cv.notify_one();


            std::cout << "ERROR: did not receive anything from client" << endl;  // if recvfrom returns -1 it means the process was unsuccessfull
            return 0;
        }

        std::cout << "Message Received from Client: " << RxBuffer << endl;

        Packet* Pkt = CreatePacket();

        LogIn log;

        SignUp signup;

        SignUpCheck check;

        Listing list;

        Deserialization(Pkt, RxBuffer, log, signup, check, list);
        logginginfo.logPacketRecv(&Pkt);


        char* errMsg = 0;


        RouteCheck(ConnectionSocket, clientID, &logginginfo, &Pkt, &list, &signup, &log, &check);

        DestroyPacket(Pkt);


        //if (strcmp(Pkt->GetHead()->Route, "POST") == 0) {
        //    /*std::string dbPath = "database.db";

        //    SQLiteDatabase sqldb(dbPath);

        //    const char* sqlCreateTableListing = "CREATE TABLE IF NOT EXISTS listings ("
        //        "id INTEGER NOT NULL,"
        //        "title TEXT NOT NULL,"
        //        "location TEXT NOT NULL,"
        //        "condition TEXT NOT NULL,"
        //        "estimated_worth TEXT NOT NULL,"
        //        "delivery TEXT NOT NULL,"
        //        "looking_for TEXT NOT NULL,"
        //        "listing_picture BLOB NOT NULL,"
        //        "postID INTEGER PRIMARY KEY);";

        //    bool query_exe_result = sqldb.executeQuery(sqlCreateTableListing);

        //    if (!query_exe_result) {
        //        return -1;
        //    }

        //    sqlite3_stmt* stmt = nullptr;

        //    int ListingPostInsertionReturn = sqldb.ListingPostInsert(&stmt, Pkt, list);

        //    if (ListingPostInsertionReturn == -1) {
        //        return ListingPostInsertionReturn;
        //    }

        //    sqldb.closeDatabase(&stmt);
        //    stmt = nullptr;*/


        //}

        //else if (strcmp(Pkt->GetHead()->Route, "STARTUP_GETID") == 0) {
        //    std::string source = "127.0.0.1";
        //    std::string destination = "127.0.0.1";
        //    std::string route = "STARTUP_GETID";
        //    bool auth = false;
        //    unsigned int DataLength = 0;

        //    Packet* pkt = CreatePacket();

        //    memcpy(pkt->GetHead()->Source, source.c_str(), source.length());
        //    memcpy(pkt->GetHead()->Destination, destination.c_str(), destination.length());
        //    memcpy(pkt->GetHead()->Route, route.c_str(), route.length());

        //    pkt->GetHead()->Authorization = auth;
        //    pkt->GetHead()->Length = DataLength;


        //    pkt->GetBody()->User = clientID;

        //    int size = sizeof(pkt->GetBody()->User);


        //    char* TxBuffer = new char[sizeof(*(pkt->GetHead())) + sizeof(pkt->GetBody()->User)];

        //    memset(TxBuffer, 0, sizeof(*(pkt->GetHead())) + sizeof(pkt->GetBody()->User));


        //    memcpy(TxBuffer, pkt->GetHead(), sizeof(*(pkt->GetHead())));

        //    memcpy(TxBuffer + sizeof(*(pkt->GetHead())), &clientID, sizeof(clientID));

        //    logginginfo.logPacketSend(&pkt);
        //    int sendSize = send(ConnectionSocket, TxBuffer, sizeof(*(pkt->GetHead())) + sizeof(pkt->GetBody()->User), 0);
        //}

        //else if (strcmp(Pkt->GetHead()->Route, "DELETE_POST") == 0) {
        //    std::string dbPath = "database.db";

        //    SQLiteDatabase sqldb(dbPath);

        //    sqlite3_stmt* stmt = nullptr;

        //    std::ostringstream oss;

        //    std::cout << Pkt->GetBody()->User << std::endl;

        //    oss << "SELECT postID FROM listings WHERE " << "(id = " << (int)Pkt->GetBody()->User << ") " << "AND " << "(title = '" << list.Title << "') " << "AND " << "(location = '" << list.Location << "') " << "AND " << "(condition = '" << list.Condition << "') " << "AND " << "(estimated_worth = '" << list.EstimatedWorth << "') " << "AND " << "(delivery = '" << list.Delivery << "') " << "AND " << "(looking_for = '" << list.LookingFor << "') " << ";";



        //    std::string query_str = oss.str();

        //    const char* sqlGetPostID = query_str.c_str();



        //    // Prepare the SQL statement
        //    int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlGetPostID, -1, &stmt, nullptr);
        //    if (rc != SQLITE_OK) {
        //        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //    }

        //    /*sqlite3_bind_int(stmt, 1, (int)(Pkt->GetBody()->User));
        //    sqlite3_bind_text(stmt, 2, list.Title, -1, SQLITE_TRANSIENT);
        //    sqlite3_bind_text(stmt, 3, list.Location, -1, SQLITE_TRANSIENT);
        //    sqlite3_bind_text(stmt, 4, list.Condition, -1, SQLITE_TRANSIENT);
        //    sqlite3_bind_text(stmt, 2, list.Title, -1, SQLITE_TRANSIENT);
        //    sqlite3_bind_text(stmt, 3, list.Location, -1, SQLITE_TRANSIENT);
        //    sqlite3_bind_text(stmt, 4, list.Condition, -1, SQLITE_TRANSIENT);*/

        //    // Bind the blob parameter
        //    /*sqlite3_bind_blob(stmt, 1, list.ImageStructArray, Pkt->GetHead()->Length - (sizeof(list.Title) + sizeof(list.EstimatedWorth) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.Delivery) + sizeof(list.LookingFor)), SQLITE_STATIC);
        //    std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;*/

        //    // Execute the SQL statement
        //    rc = sqlite3_step(stmt);
        //    if (rc == SQLITE_ROW) {
        //        std::cout << "Row Returned by Select statement!!" << std::endl;
        //        std::cout << "Deleting...." << std::endl;
        //    }
        //    /*while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        //        std::cout << "" << std::endl;
        //    }
        //    std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;*/

        //    int postID = sqlite3_column_int(stmt, 0);

        //    /*rc = sqlite3_step(stmt);
        //    if (rc == SQLITE_DONE) {
        //        std::cout << "Post Successfully Deleted" << std::endl;
        //    }*/
        //    // std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;

        //    sqlite3_finalize(stmt);

        //    oss.clear();
        //    oss.str("");

        //    query_str.clear();

        //    oss << "DELETE FROM listings WHERE " << "postID = '" << postID << "';";

        //    query_str = oss.str();

        //    const char* sqlDeletePost = query_str.c_str();


        //    rc = sqlite3_prepare_v2(sqldb.getDB(), sqlDeletePost, -1, &stmt, nullptr);
        //    if (rc != SQLITE_OK) {
        //        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //    }

        //    rc = sqlite3_step(stmt);
        //    if (rc == SQLITE_DONE) {
        //        std::cout << "Post Successfully Deleted" << std::endl;
        //    }
        //    else {
        //        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //    }

        //    sqldb.closeDatabase(&stmt);
        //    stmt = nullptr;
        //}



        //else if (strcmp(Pkt->GetHead()->Route, "MYPOSTS_COUNT") == 0) {
        //    std::string dbPath = "database.db";

        //    SQLiteDatabase sqldb(dbPath);

        //    sqlite3_stmt* stmt;

        //    int numOfRows;

        //    std::ostringstream oss;
        //    oss << "SELECT COUNT(*) FROM listings WHERE id = '" << Pkt->GetBody()->User << "';";
        //    std::string query_str = oss.str();

        //    const char* sqlCountListingRows = query_str.c_str();



        //    // Prepare the SQL statement
        //    int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlCountListingRows, -1, &stmt, NULL);
        //    if (rc != SQLITE_OK) {
        //        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //    }



        //    // Execute the SQL statement
        //    rc = sqlite3_step(stmt);
        //    if (rc == SQLITE_ROW) {
        //        numOfRows = sqlite3_column_int(stmt, 0);
        //        std::cout << "Number of rows in the table: " << numOfRows << std::endl;
        //    }

        //    sqldb.closeDatabase(&stmt);
        //    stmt = nullptr;



        //    std::string source = "127.0.0.1";
        //    std::string destination = "127.0.0.1";
        //    std::string route = "MYPOSTS_COUNT";
        //    bool auth = true;
        //    unsigned int length = numOfRows;


        //    Packet* pkt = CreatePacket();

        //    memcpy(pkt->GetHead()->Source, source.c_str(), source.length());
        //    memcpy(pkt->GetHead()->Destination, destination.c_str(), destination.length());
        //    memcpy(pkt->GetHead()->Route, route.c_str(), route.length());

        //    pkt->GetHead()->Authorization = auth;
        //    pkt->GetHead()->Length = length;

        //    char* TxBuffer = new char[sizeof(*(pkt->GetHead()))];

        //    memset(TxBuffer, 0, sizeof(*(pkt->GetHead())));


        //    memcpy(TxBuffer, pkt->GetHead(), sizeof(*(pkt->GetHead())));

        //    logginginfo.logPacketSend(&pkt);
        //    send(ConnectionSocket, TxBuffer, sizeof(*(pkt->GetHead())), 0);



        //    if (pkt->GetHead()->Length > 0) {
        //        oss.clear();
        //        oss.str("");

        //        oss << "SELECT * FROM listings WHERE id = '" << Pkt->GetBody()->User << "';";
        //        query_str.clear();

        //        query_str = oss.str();

        //        const char* sqlSelectRows = query_str.c_str();

        //        SQLiteDatabase sqldb2(dbPath);

        //        sqlite3_stmt* stmt2;

        //        rc = sqlite3_prepare_v2(sqldb2.getDB(), sqlSelectRows, -1, &stmt2, NULL);
        //        if (rc != SQLITE_OK) {
        //            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb2.getDB()) << std::endl;
        //        }




        //        // Step through each row
        //        while (sqlite3_step(stmt2) == SQLITE_ROW) {
        //            Listing list;

        //            memset(list.Title, 0, sizeof(list.Title));
        //            memset(list.Location, 0, sizeof(list.Location));
        //            memset(list.Condition, 0, sizeof(list.Condition));
        //            memset(list.EstimatedWorth, 0, sizeof(list.EstimatedWorth));
        //            memset(list.Delivery, 0, sizeof(list.Delivery));
        //            memset(list.LookingFor, 0, sizeof(list.LookingFor));

        //            // Retrieve data from the current row
        //            int id = sqlite3_column_int(stmt2, 0);
        //            int idSize = sqlite3_column_bytes(stmt2, 0);

        //            const unsigned char* title = sqlite3_column_text(stmt2, 1);
        //            int titleSize = sqlite3_column_bytes(stmt2, 1);

        //            const unsigned char* location = sqlite3_column_text(stmt2, 2);
        //            int locationSize = sqlite3_column_bytes(stmt2, 2);

        //            const unsigned char* condition = sqlite3_column_text(stmt2, 3);
        //            int conditionSize = sqlite3_column_bytes(stmt2, 3);

        //            const unsigned char* estimated_worth = sqlite3_column_text(stmt2, 4);
        //            int worthSize = sqlite3_column_bytes(stmt2, 4);

        //            const unsigned char* delivery = sqlite3_column_text(stmt2, 5);
        //            int deliverySize = sqlite3_column_bytes(stmt2, 5);

        //            const unsigned char* looking_for = sqlite3_column_text(stmt2, 6);
        //            int looking_for_size = sqlite3_column_bytes(stmt2, 6);

        //            // Retrieve BLOB data
        //            const void* blobArray = sqlite3_column_blob(stmt2, 7);

        //            unsigned int postImageSize = sqlite3_column_bytes(stmt2, 7);


        //            memcpy(&(list.Title), title, titleSize);
        //            memcpy(&(list.Location), location, locationSize);
        //            memcpy(&(list.Condition), condition, conditionSize);
        //            memcpy(&(list.EstimatedWorth), estimated_worth, worthSize);
        //            memcpy(&(list.Delivery), delivery, deliverySize);
        //            memcpy(&(list.LookingFor), looking_for, looking_for_size);

        //            list.ImageStructArray = new char[postImageSize];

        //            memset(list.ImageStructArray, 0, postImageSize);

        //            memcpy(list.ImageStructArray, (char*)blobArray, postImageSize);




        //            int TotalSize = (6 * sizeof(list.Title)) + postImageSize + sizeof(unsigned int);

        //            char* TxBuffer = new char[TotalSize];

        //            memset(TxBuffer, 0, TotalSize);

        //            unsigned int sizeOfImage = postImageSize;


        //            memcpy(TxBuffer, &(list.Title), sizeof(list.Title));

        //            memcpy(TxBuffer + sizeof(list.Title), &(list.Location), sizeof(list.Location));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location), &(list.Condition), sizeof(list.Condition));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition), &(list.EstimatedWorth), sizeof(list.EstimatedWorth));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth), &(list.Delivery), sizeof(list.Delivery));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery), &(list.LookingFor), sizeof(list.LookingFor));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor), &sizeOfImage, sizeof(sizeOfImage));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor) + sizeof(sizeOfImage), list.ImageStructArray, sizeOfImage);

        //            logginginfo.logListingSend(&list);
        //            int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);

        //            delete[] TxBuffer;
        //            TxBuffer = nullptr;


        //            TxBuffer = new char[100];

        //            memset(TxBuffer, 0, 100);

        //            int recvSize = recv(ConnectionSocket, TxBuffer, 100, 0);


        //            std::string messageReceived(TxBuffer);

        //            std::cout << "Message Sent By Client: " << messageReceived << std::endl;
        //            logginginfo.logResponse(messageReceived.c_str());

        //            delete[] TxBuffer;
        //            TxBuffer = nullptr;
        //        }

        //        sqldb2.closeDatabase(&stmt2);
        //        stmt2 = nullptr;
        //    }
        //}

        //else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_IMAGEUPLOADED") == 0) {
        //    std::string dbPath = "database.db";

        //    SQLiteDatabase sqldb(dbPath);

        //    // SQL command to create table
        //    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithProfile ("
        //        "id INTEGER NOT NULL,"
        //        "username TEXT NOT NULL,"
        //        "password TEXT NOT NULL,"
        //        "email TEXT PRIMARY KEY,"
        //        "profile_picture BLOB NOT NULL);";


        //    bool query_exe_result = sqldb.executeQuery(sqlCreateTable);

        //    if (!query_exe_result) {
        //        return -1;
        //    }



        //    sqlite3_stmt* stmt = nullptr;

        //    int SignUpdataInsertionReturn = sqldb.SignUpWithImageDataInsert(&stmt, Pkt, signup);

        //    if (SignUpdataInsertionReturn == -1) {
        //        return SignUpdataInsertionReturn;
        //    }



        //    // Retrieve BLOB data
        //    char* imageArray = nullptr;
        //    int imageSize = 0;

        //    sqldb.FetchImage(&stmt, signup.email, &imageArray, imageSize);



        //    logginginfo.logImageSend(&imageArray, imageSize);
        //    int sendSize = send(ConnectionSocket, imageArray, imageSize, 0);

        //    if (sendSize < 0) {
        //        std::cout << "Sending Image Failed!!" << std::endl;

        //        return -1;
        //    }

        //    else {
        //        std::cout << "Image Successfully sent!! Wohoooooo" << std::endl;
        //    }

        //    delete[] imageArray;

        //    /*Finalize the statement and close the database connection*/
        //    sqldb.closeDatabase(&stmt);
        //    stmt = nullptr;
        //}

        //else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_IMAGENOTUPLOADED") == 0) {
        //    std::string dbPath = "database.db";

        //    SQLiteDatabase sqldb(dbPath);

        //    // SQL command to create table
        //    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithoutProfile ("
        //        "id INTEGER NOT NULL,"
        //        "username TEXT NOT NULL,"
        //        "password TEXT NOT NULL,"
        //        "email TEXT PRIMARY KEY,"
        //        "profile_picture_SubstituteData TEXT NOT NULL);";


        //    bool query_exe_result = sqldb.executeQuery(sqlCreateTable);

        //    if (!query_exe_result) {
        //        return -1;
        //    }



        //    sqlite3_stmt* stmt = nullptr;

        //    int SignUpdataInsertionReturn = sqldb.SignUpWithoutImageDataInsert(&stmt, Pkt, signup);

        //    if (SignUpdataInsertionReturn == -1) {
        //        return SignUpdataInsertionReturn;
        //    }

        //    sqldb.closeDatabase(&stmt);
        //    stmt = nullptr;
        //}

        //else if (strcmp(Pkt->GetHead()->Route, "LOGIN") == 0) {
        //    std::string dbPath = "database.db";

        //    SQLiteDatabase sqldb(dbPath);


        //    sqlite3_stmt* stmt;

        //    std::ostringstream oss;
        //    oss << "SELECT 'UsersWithProfile' "
        //        "FROM UsersWithProfile "
        //        "WHERE (username = '" << log.username << "')" << " AND" << " (password = '" << log.password << "') "
        //        "UNION ALL "
        //        "SELECT 'UsersWithoutProfile' "
        //        "FROM UsersWithoutProfile "
        //        "WHERE (username = '" << log.username << "')" << " AND" << " (password = '" << log.password << "');";

        //    std::string query_str = oss.str();

        //    const char* sqlTableWhereUserFound = query_str.c_str();



        //    // Prepare the SQL statement
        //    int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlTableWhereUserFound, -1, &stmt, NULL);
        //    if (rc != SQLITE_OK) {
        //        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //    }

        //    // Execute the SQL statement
        //    rc = sqlite3_step(stmt);
        //    if (rc == SQLITE_DONE) {
        //        std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //    }

        //    const char* tableName = (char*)sqlite3_column_text(stmt, 0);


        //    UserCredentials cred;

        //    if (!tableName) {
        //        Packet* pkt_login = CreatePacket();

        //        char source[20] = "127.0.0.1";
        //        int source_size = sizeof(source);

        //        char destination[20] = "127.0.0.1";
        //        int destination_size = sizeof(destination);

        //        char Route[40] = "LOGIN_USERNOTFOUND";
        //        int Route_size = sizeof(Route);

        //        bool Authorization = false;

        //        int bodyLength = 0;


        //        memcpy(pkt_login->GetHead()->Source, source, source_size);
        //        memcpy(pkt_login->GetHead()->Destination, destination, destination_size);
        //        memcpy(pkt_login->GetHead()->Route, Route, Route_size);

        //        pkt_login->GetHead()->Authorization = Authorization;

        //        pkt_login->GetHead()->Length = bodyLength;


        //        int totalSize = sizeof(*(pkt_login->GetHead())) + pkt_login->GetHead()->Length;

        //        char* TxBuffer = new char[totalSize];
        //        memset(TxBuffer, 0, totalSize);


        //        memcpy(TxBuffer, pkt_login->GetHead(), sizeof(*(pkt_login->GetHead())));

        //        logginginfo.logPacketSend(&pkt_login);
        //        int sendSize = send(ConnectionSocket, TxBuffer, totalSize, 0);

        //        if (sendSize < 0) {
        //            std::cout << "Failed to Send the Data" << std::endl;
        //        }

        //        delete[] TxBuffer;
        //        TxBuffer = nullptr;
        //    }

        //    else if (strcmp(tableName, "UsersWithProfile") == 0) {
        //        oss.str("");
        //        oss.clear();

        //        query_str.clear();

        //        oss << "SELECT * FROM " << tableName << " WHERE (username = '" << log.username << "') " << "AND " << "(password = '" << log.password << "');";

        //        query_str = oss.str();

        //        const char* sqlgetUserCredentials = query_str.c_str();

        //        sqlite3_finalize(stmt);

        //        // Prepare the SQL statement
        //        int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlgetUserCredentials, -1, &stmt, NULL);
        //        if (rc != SQLITE_OK) {
        //            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //        }

        //        Packet* pkt_login = CreatePacket();

        //        int imageSize = 0;

        //        // Execute the SQL statement
        //        rc = sqlite3_step(stmt);
        //        if (rc == SQLITE_ROW) {

        //            pkt_login->GetBody()->User = sqlite3_column_int(stmt, 0);

        //            memset(cred.username, 0, sizeof(cred.username));
        //            memcpy(cred.username, sqlite3_column_text(stmt, 1), sqlite3_column_bytes(stmt, 1));

        //            memset(cred.password, 0, sizeof(cred.password));
        //            memcpy(cred.password, sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));

        //            memset(cred.email, 0, sizeof(cred.email));
        //            memcpy(cred.email, sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3));

        //            /*Retrieve BLOB data*/
        //            const void* blobArray = sqlite3_column_blob(stmt, 4);

        //            imageSize = sqlite3_column_bytes(stmt, 4);


        //            cred.imageStructArray = new char[imageSize];

        //            memset(cred.imageStructArray, 0, imageSize);

        //            memcpy(cred.imageStructArray, (char*)blobArray, imageSize);

        //            blobArray = nullptr;
        //        }

        //        else if (rc != SQLITE_DONE) {
        //            std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //        }


        //        char source[20] = "127.0.0.1";
        //        int source_size = sizeof(source);

        //        char destination[20] = "127.0.0.1";
        //        int destination_size = sizeof(destination);

        //        char Route[40] = "LOGIN_USERFOUNDWITHIMAGE";
        //        int Route_size = sizeof(Route);

        //        bool Authorization = true;

        //        int length = sizeof(cred.username) + sizeof(cred.password) + sizeof(cred.email) + imageSize;


        //        memcpy(pkt_login->GetHead()->Source, source, source_size);
        //        memcpy(pkt_login->GetHead()->Destination, destination, destination_size);
        //        memcpy(pkt_login->GetHead()->Route, Route, Route_size);

        //        pkt_login->GetHead()->Authorization = Authorization;

        //        pkt_login->GetHead()->Length = length;


        //        pkt_login->GetBody()->Data = new char[pkt_login->GetHead()->Length];

        //        memset(pkt_login->GetBody()->Data, 0, pkt_login->GetHead()->Length);

        //        memcpy(pkt_login->GetBody()->Data, cred.username, sizeof(cred.username));
        //        memcpy(pkt_login->GetBody()->Data + sizeof(cred.username), cred.password, sizeof(cred.password));
        //        memcpy(pkt_login->GetBody()->Data + sizeof(cred.username) + sizeof(cred.password), cred.email, sizeof(cred.email));

        //        memcpy(pkt_login->GetBody()->Data + sizeof(cred.username) + sizeof(cred.password) + sizeof(cred.email), cred.imageStructArray, imageSize);


        //        delete[] cred.imageStructArray;
        //        cred.imageStructArray = nullptr;



        //        int totalSize = sizeof(*(pkt_login->GetHead())) + sizeof(pkt_login->GetBody()->User) + pkt_login->GetHead()->Length;

        //        char* TxBuffer = new char[totalSize];
        //        memset(TxBuffer, 0, totalSize);


        //        memcpy(TxBuffer, pkt_login->GetHead(), sizeof(*(pkt_login->GetHead())));
        //        memcpy(TxBuffer + sizeof(*(pkt_login->GetHead())), &(pkt_login->GetBody()->User), sizeof(pkt_login->GetBody()->User));
        //        memcpy(TxBuffer + sizeof(*(pkt_login->GetHead())) + sizeof(pkt_login->GetBody()->User), pkt_login->GetBody()->Data, pkt_login->GetHead()->Length);
        //       
        //        
        //        logginginfo.logPacketSend(&pkt_login);
        //        
        //        
        //        int sendSize = send(ConnectionSocket, TxBuffer, totalSize, 0);

        //        if (sendSize < 0) {
        //            std::cout << "Failed to Send the Data" << std::endl;
        //        }

        //        delete[] TxBuffer;
        //        TxBuffer = nullptr;

        //        delete[] pkt_login->GetBody()->Data;
        //        pkt_login->GetBody()->Data = nullptr;

        //    }

        //    else if (strcmp(tableName, "UsersWithoutProfile") == 0) {
        //        oss.str("");
        //        oss.clear();

        //        query_str.clear();

        //        oss << "SELECT * FROM " << tableName << " WHERE (username = '" << log.username << "') " << "AND " << "(password = '" << log.password << "');";

        //        query_str = oss.str();

        //        const char* sqlgetUserCredentials = query_str.c_str();

        //        sqlite3_finalize(stmt);

        //        // Prepare the SQL statement
        //        int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlgetUserCredentials, -1, &stmt, NULL);
        //        if (rc != SQLITE_OK) {
        //            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //        }

        //        Packet* pkt_login = CreatePacket();


        //        // Execute the SQL statement
        //        rc = sqlite3_step(stmt);
        //        if (rc == SQLITE_ROW) {

        //            pkt_login->GetBody()->User = sqlite3_column_int(stmt, 0);

        //            memset(cred.username, 0, sizeof(cred.username));
        //            memcpy(cred.username, sqlite3_column_text(stmt, 1), sqlite3_column_bytes(stmt, 1));

        //            memset(cred.password, 0, sizeof(cred.password));
        //            memcpy(cred.password, sqlite3_column_text(stmt, 2), sqlite3_column_bytes(stmt, 2));

        //            memset(cred.email, 0, sizeof(cred.email));
        //            memcpy(cred.email, sqlite3_column_text(stmt, 3), sqlite3_column_bytes(stmt, 3));
        //        }

        //        else if (rc != SQLITE_DONE) {
        //            std::cerr << "Error executing SQL statement: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //        }



        //        char source[20] = "127.0.0.1";
        //        int source_size = sizeof(source);

        //        char destination[20] = "127.0.0.1";
        //        int destination_size = sizeof(destination);

        //        char Route[40] = "LOGIN_USERFOUNDWITHOUTIMAGE";
        //        int Route_size = sizeof(Route);

        //        bool Authorization = true;

        //        int length = sizeof(cred.username) + sizeof(cred.password) + sizeof(cred.email);


        //        memcpy(pkt_login->GetHead()->Source, source, source_size);
        //        memcpy(pkt_login->GetHead()->Destination, destination, destination_size);
        //        memcpy(pkt_login->GetHead()->Route, Route, Route_size);

        //        pkt_login->GetHead()->Authorization = Authorization;

        //        pkt_login->GetHead()->Length = length;


        //        pkt_login->GetBody()->Data = new char[pkt_login->GetHead()->Length];

        //        memset(pkt_login->GetBody()->Data, 0, pkt_login->GetHead()->Length);

        //        memcpy(pkt_login->GetBody()->Data, cred.username, sizeof(cred.username));
        //        memcpy(pkt_login->GetBody()->Data + sizeof(cred.username), cred.password, sizeof(cred.password));
        //        memcpy(pkt_login->GetBody()->Data + sizeof(cred.username) + sizeof(cred.password), cred.email, sizeof(cred.email));



        //        int totalSize = sizeof(*(pkt_login->GetHead())) + sizeof(pkt_login->GetBody()->User) + pkt_login->GetHead()->Length;

        //        char* TxBuffer = new char[totalSize];
        //        memset(TxBuffer, 0, totalSize);


        //        memcpy(TxBuffer, pkt_login->GetHead(), sizeof(*(pkt_login->GetHead())));
        //        memcpy(TxBuffer + sizeof(*(pkt_login->GetHead())), &(pkt_login->GetBody()->User), sizeof(pkt_login->GetBody()->User));
        //        memcpy(TxBuffer + sizeof(*(pkt_login->GetHead())) + sizeof(pkt_login->GetBody()->User), pkt_login->GetBody()->Data, pkt_login->GetHead()->Length);
        //        
        //        
        //        logginginfo.logPacketSend(&pkt_login);
        //        
        //        
        //        int sendSize = send(ConnectionSocket, TxBuffer, totalSize, 0);

        //        if (sendSize < 0) {
        //            std::cout << "Failed to Send the Data" << std::endl;
        //        }

        //        delete[] TxBuffer;
        //        TxBuffer = nullptr;
        //    }

        //    sqldb.closeDatabase(&stmt);
        //    stmt = nullptr;
        //}

        //else if (strcmp(Pkt->GetHead()->Route, "PRELOGIN_LISTINGS_COUNT") == 0) {
        //    std::string dbPath = "database.db";

        //    SQLiteDatabase sqldb(dbPath);

        //    sqlite3_stmt* stmt;

        //    int numOfRows;

        //    std::ostringstream oss;
        //    oss << "SELECT COUNT(*) FROM listings;";
        //    std::string query_str = oss.str();

        //    const char* sqlCountListingRows = query_str.c_str();



        //    // Prepare the SQL statement
        //    int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlCountListingRows, -1, &stmt, NULL);
        //    if (rc != SQLITE_OK) {
        //        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //    }



        //    // Execute the SQL statement
        //    rc = sqlite3_step(stmt);
        //    if (rc == SQLITE_ROW) {
        //        numOfRows = sqlite3_column_int(stmt, 0);
        //        std::cout << "Number of rows in the table: " << numOfRows << std::endl;
        //    }

        //    sqldb.closeDatabase(&stmt);
        //    stmt = nullptr;



        //    std::string source = "127.0.0.1";
        //    std::string destination = "127.0.0.1";
        //    std::string route = "PRELOGIN_LISTINGS_COUNT";
        //    bool auth = false;
        //    unsigned int length = numOfRows;


        //    Packet* pkt = CreatePacket();

        //    memcpy(pkt->GetHead()->Source, source.c_str(), source.length());
        //    memcpy(pkt->GetHead()->Destination, destination.c_str(), destination.length());
        //    memcpy(pkt->GetHead()->Route, route.c_str(), route.length());

        //    pkt->GetHead()->Authorization = auth;
        //    pkt->GetHead()->Length = length;

        //    char* TxBuffer = new char[sizeof(*(pkt->GetHead()))];

        //    memset(TxBuffer, 0, sizeof(*(pkt->GetHead())));


        //    memcpy(TxBuffer, pkt->GetHead(), sizeof(*(pkt->GetHead())));

        //    logginginfo.logPacketSend(&pkt);
        //   
        //    
        //    send(ConnectionSocket, TxBuffer, sizeof(*(pkt->GetHead())), 0);



        //    if (pkt->GetHead()->Length > 0) {
        //        oss.clear();
        //        oss.str("");

        //        oss << "SELECT * FROM listings;";
        //        query_str.clear();

        //        query_str = oss.str();

        //        const char* sqlSelectRows = query_str.c_str();

        //        SQLiteDatabase sqldb2(dbPath);

        //        sqlite3_stmt* stmt2;

        //        rc = sqlite3_prepare_v2(sqldb2.getDB(), sqlSelectRows, -1, &stmt2, NULL);
        //        if (rc != SQLITE_OK) {
        //            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb2.getDB()) << std::endl;
        //        }




        //        // Step through each row
        //        while (sqlite3_step(stmt2) == SQLITE_ROW) {
        //            Listing list;

        //            memset(list.Title, 0, sizeof(list.Title));
        //            memset(list.Location, 0, sizeof(list.Location));
        //            memset(list.Condition, 0, sizeof(list.Condition));
        //            memset(list.EstimatedWorth, 0, sizeof(list.EstimatedWorth));
        //            memset(list.Delivery, 0, sizeof(list.Delivery));
        //            memset(list.LookingFor, 0, sizeof(list.LookingFor));

        //            // Retrieve data from the current row
        //            int id = sqlite3_column_int(stmt2, 0);
        //            int idSize = sqlite3_column_bytes(stmt2, 0);

        //            const unsigned char* title = sqlite3_column_text(stmt2, 1);
        //            int titleSize = sqlite3_column_bytes(stmt2, 1);

        //            const unsigned char* location = sqlite3_column_text(stmt2, 2);
        //            int locationSize = sqlite3_column_bytes(stmt2, 2);

        //            const unsigned char* condition = sqlite3_column_text(stmt2, 3);
        //            int conditionSize = sqlite3_column_bytes(stmt2, 3);

        //            const unsigned char* estimated_worth = sqlite3_column_text(stmt2, 4);
        //            int worthSize = sqlite3_column_bytes(stmt2, 4);

        //            const unsigned char* delivery = sqlite3_column_text(stmt2, 5);
        //            int deliverySize = sqlite3_column_bytes(stmt2, 5);

        //            const unsigned char* looking_for = sqlite3_column_text(stmt2, 6);
        //            int looking_for_size = sqlite3_column_bytes(stmt2, 6);

        //            // Retrieve BLOB data
        //            const void* blobArray = sqlite3_column_blob(stmt2, 7);

        //            unsigned int postImageSize = sqlite3_column_bytes(stmt2, 7);


        //            memcpy(&(list.Title), title, titleSize);
        //            memcpy(&(list.Location), location, locationSize);
        //            memcpy(&(list.Condition), condition, conditionSize);
        //            memcpy(&(list.EstimatedWorth), estimated_worth, worthSize);
        //            memcpy(&(list.Delivery), delivery, deliverySize);
        //            memcpy(&(list.LookingFor), looking_for, looking_for_size);

        //            list.ImageStructArray = new char[postImageSize];

        //            memset(list.ImageStructArray, 0, postImageSize);

        //            memcpy(list.ImageStructArray, (char*)blobArray, postImageSize);




        //            int TotalSize = (6 * sizeof(list.Title)) + postImageSize + sizeof(unsigned int);

        //            char* TxBuffer = new char[TotalSize];

        //            memset(TxBuffer, 0, TotalSize);

        //            unsigned int sizeOfImage = postImageSize;


        //            memcpy(TxBuffer, &(list.Title), sizeof(list.Title));

        //            memcpy(TxBuffer + sizeof(list.Title), &(list.Location), sizeof(list.Location));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location), &(list.Condition), sizeof(list.Condition));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition), &(list.EstimatedWorth), sizeof(list.EstimatedWorth));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth), &(list.Delivery), sizeof(list.Delivery));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery), &(list.LookingFor), sizeof(list.LookingFor));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor), &sizeOfImage, sizeof(sizeOfImage));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor) + sizeof(sizeOfImage), list.ImageStructArray, sizeOfImage);
        //            
        //            
        //            
        //            logginginfo.logListingSend(&list);
        //            
        //            
        //            
        //            int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);

        //            delete[] TxBuffer;
        //            TxBuffer = nullptr;



        //            TxBuffer = new char[100];

        //            memset(TxBuffer, 0, 100);

        //            int recvSize = recv(ConnectionSocket, TxBuffer, 100, 0);
        //            

        //            std::string messageReceived(TxBuffer);

        //            logginginfo.logResponse(messageReceived.c_str());

        //            std::cout << "Message Sent By Client: " << messageReceived << std::endl;


        //            delete[] TxBuffer;
        //            TxBuffer = nullptr;
        //        }

        //        sqldb2.closeDatabase(&stmt2);
        //        stmt2 = nullptr;
        //    }
        //}


        //else if (strcmp(Pkt->GetHead()->Route, "POSTLOGIN_LISTINGS_COUNT") == 0) {
        //    std::string dbPath = "database.db";

        //    SQLiteDatabase sqldb(dbPath);

        //    sqlite3_stmt* stmt;

        //    int numOfRows;

        //    std::ostringstream oss;
        //    oss << "SELECT COUNT(*) FROM listings WHERE id != '" << Pkt->GetBody()->User << "';";
        //    std::string query_str = oss.str();

        //    const char* sqlCountListingRows = query_str.c_str();



        //    // Prepare the SQL statement
        //    int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlCountListingRows, -1, &stmt, NULL);
        //    if (rc != SQLITE_OK) {
        //        std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
        //    }



        //    // Execute the SQL statement
        //    rc = sqlite3_step(stmt);
        //    if (rc == SQLITE_ROW) {
        //        numOfRows = sqlite3_column_int(stmt, 0);
        //        std::cout << "Number of rows in the table: " << numOfRows << std::endl;
        //    }

        //    sqldb.closeDatabase(&stmt);
        //    stmt = nullptr;



        //    std::string source = "127.0.0.1";
        //    std::string destination = "127.0.0.1";
        //    std::string route = "POSTLOGIN_LISTINGS_COUNT";
        //    bool auth = true;
        //    unsigned int length = numOfRows;


        //    Packet* pkt = CreatePacket();

        //    memcpy(pkt->GetHead()->Source, source.c_str(), source.length());
        //    memcpy(pkt->GetHead()->Destination, destination.c_str(), destination.length());
        //    memcpy(pkt->GetHead()->Route, route.c_str(), route.length());

        //    pkt->GetHead()->Authorization = auth;
        //    pkt->GetHead()->Length = length;

        //    char* TxBuffer = new char[sizeof(*(pkt->GetHead()))];

        //    memset(TxBuffer, 0, sizeof(*(pkt->GetHead())));


        //    memcpy(TxBuffer, pkt->GetHead(), sizeof(*(pkt->GetHead())));

        //    logginginfo.logPacketSend(&pkt);


        //    send(ConnectionSocket, TxBuffer, sizeof(*(pkt->GetHead())), 0);



        //    if (pkt->GetHead()->Length > 0) {
        //        oss.clear();
        //        oss.str("");

        //        oss << "SELECT * FROM listings WHERE id != '" << Pkt->GetBody()->User << "';";
        //        query_str.clear();

        //        query_str = oss.str();

        //        const char* sqlSelectRows = query_str.c_str();

        //        SQLiteDatabase sqldb2(dbPath);

        //        sqlite3_stmt* stmt2;

        //        rc = sqlite3_prepare_v2(sqldb2.getDB(), sqlSelectRows, -1, &stmt2, NULL);
        //        if (rc != SQLITE_OK) {
        //            std::cerr << "SQL error: " << sqlite3_errmsg(sqldb2.getDB()) << std::endl;
        //        }




        //        // Step through each row
        //        while (sqlite3_step(stmt2) == SQLITE_ROW) {
        //            Listing list;

        //            memset(list.Title, 0, sizeof(list.Title));
        //            memset(list.Location, 0, sizeof(list.Location));
        //            memset(list.Condition, 0, sizeof(list.Condition));
        //            memset(list.EstimatedWorth, 0, sizeof(list.EstimatedWorth));
        //            memset(list.Delivery, 0, sizeof(list.Delivery));
        //            memset(list.LookingFor, 0, sizeof(list.LookingFor));

        //            // Retrieve data from the current row
        //            int id = sqlite3_column_int(stmt2, 0);
        //            int idSize = sqlite3_column_bytes(stmt2, 0);

        //            const unsigned char* title = sqlite3_column_text(stmt2, 1);
        //            int titleSize = sqlite3_column_bytes(stmt2, 1);

        //            const unsigned char* location = sqlite3_column_text(stmt2, 2);
        //            int locationSize = sqlite3_column_bytes(stmt2, 2);

        //            const unsigned char* condition = sqlite3_column_text(stmt2, 3);
        //            int conditionSize = sqlite3_column_bytes(stmt2, 3);

        //            const unsigned char* estimated_worth = sqlite3_column_text(stmt2, 4);
        //            int worthSize = sqlite3_column_bytes(stmt2, 4);

        //            const unsigned char* delivery = sqlite3_column_text(stmt2, 5);
        //            int deliverySize = sqlite3_column_bytes(stmt2, 5);

        //            const unsigned char* looking_for = sqlite3_column_text(stmt2, 6);
        //            int looking_for_size = sqlite3_column_bytes(stmt2, 6);

        //            // Retrieve BLOB data
        //            const void* blobArray = sqlite3_column_blob(stmt2, 7);

        //            unsigned int postImageSize = sqlite3_column_bytes(stmt2, 7);


        //            memcpy(&(list.Title), title, titleSize);
        //            memcpy(&(list.Location), location, locationSize);
        //            memcpy(&(list.Condition), condition, conditionSize);
        //            memcpy(&(list.EstimatedWorth), estimated_worth, worthSize);
        //            memcpy(&(list.Delivery), delivery, deliverySize);
        //            memcpy(&(list.LookingFor), looking_for, looking_for_size);

        //            list.ImageStructArray = new char[postImageSize];

        //            memset(list.ImageStructArray, 0, postImageSize);

        //            memcpy(list.ImageStructArray, (char*)blobArray, postImageSize);




        //            int TotalSize = (6 * sizeof(list.Title)) + postImageSize + sizeof(unsigned int);

        //            char* TxBuffer = new char[TotalSize];

        //            memset(TxBuffer, 0, TotalSize);

        //            unsigned int sizeOfImage = postImageSize;


        //            memcpy(TxBuffer, &(list.Title), sizeof(list.Title));

        //            memcpy(TxBuffer + sizeof(list.Title), &(list.Location), sizeof(list.Location));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location), &(list.Condition), sizeof(list.Condition));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition), &(list.EstimatedWorth), sizeof(list.EstimatedWorth));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth), &(list.Delivery), sizeof(list.Delivery));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery), &(list.LookingFor), sizeof(list.LookingFor));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor), &sizeOfImage, sizeof(sizeOfImage));

        //            memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor) + sizeof(sizeOfImage), list.ImageStructArray, sizeOfImage);

        //            logginginfo.logListingSend(&list);

        //            int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);

        //            delete[] TxBuffer;
        //            TxBuffer = nullptr;



        //            TxBuffer = new char[100];

        //            memset(TxBuffer, 0, 100);

        //            int recvSize = recv(ConnectionSocket, TxBuffer, 100, 0);
        //            

        //            std::string messageReceived(TxBuffer);

        //            logginginfo.logResponse(messageReceived.c_str());

        //            std::cout << "Message Sent By Client: " << messageReceived << std::endl;


        //            delete[] TxBuffer;
        //            TxBuffer = nullptr;
        //        }

        //        sqldb2.closeDatabase(&stmt2);
        //        stmt2 = nullptr;
        //    }
        //}


        //else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_USERCHECK") == 0) {
        //    std::string dbPath = "database.db";

        //    SQLiteDatabase sqldb(dbPath);

        //    userFound = false;

        //    std::ostringstream oss;
        //    oss << "SELECT username, email FROM UsersWithProfile WHERE username = '" << check.username << "';";
        //    std::string withProfile_query_str = oss.str();

        //    int (*callback_ptr)(void*, int, char**, char**);

        //    callback_ptr = callback;

        //    char* errMsg = 0;


        //    int rc = sqlite3_exec(sqldb.getDB(), withProfile_query_str.c_str(), &callback, 0, &errMsg);
        //    if (rc != SQLITE_OK) {
        //        std::cerr << "SQL error: " << errMsg << std::endl;
        //        sqlite3_free(errMsg);
        //    }


        //    if (!userFound) {
        //        oss.clear();

        //        oss << "SELECT username, email FROM UsersWithoutProfile WHERE username = '" << check.username << "';";

        //        std::string withoutProfile_query_str = oss.str();

        //        rc = sqlite3_exec(sqldb.getDB(), withoutProfile_query_str.c_str(), &callback, 0, &errMsg);

        //        if (rc != SQLITE_OK) {
        //            std::cerr << "SQL error: " << errMsg << std::endl;
        //            sqlite3_free(errMsg);
        //        }
        //    }

        //    /*Close the database connection*/
        //    sqlite3_close(sqldb.getDB());

        //    Packet* pkt = CreatePacket();

        //    char source[20] = "127.0.0.1";
        //    int source_size = sizeof(source);

        //    char destination[20] = "127.0.0.1";
        //    int destination_size = sizeof(destination);

        //    char Route[40] = "LOGIN";
        //    int Route_size = sizeof(Route);

        //    bool Authorization = userFound;

        //    int length = 0;

        //    SetHeaderInformation(pkt, source, source_size, destination, destination_size, Route, Route_size, Authorization, length);

        //    int TotalSize = 0;

        //    char* TxBuffer = SerializeUserCheckingData(pkt, TotalSize);
        //    logginginfo.logPacketSend(&pkt);
        //    int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);

        //    if (sendSize < 0) {
        //        std::cout << "Sending User Check Failed!!" << std::endl;

        //        return -1;
        //    }

        //    else {
        //        std::cout << "Response for User Check Successfully sent!! Wohoooooo" << std::endl;
        //    }

        //    delete[] TxBuffer;
        //    TxBuffer = nullptr;
        //}

        

    }

    return 0;
}



int main()
{
    // Start the server in a separate thread
    std::thread serverThread(setupConnection, threadedFunc);


    // Start the Cleanup thread
    std::thread cleanupThread(cleanupThreads);


    // Wait for the server thread to finish (never returns)
    serverThread.join();

    // Wait for the cleanup thread to finish (never returns)
    cleanupThread.join();


    WSACleanup();
    std::cout << "Winsock library resources cleaned up and released" << endl;

    return 0;
}
