//#include "TCPServer.h"

//int main(void) {
//	TCPServer server = TCPServer(27000);
//	server.start();
//	return 1;
//}
//#include <windows.networking.sockets.h>
//#include <cstring>
//#include <iostream>
//#include <fstream>
//#include <sstream>
#include "Packet.h"
//#include "SQLiteDatabase.h"
//#include "file_utils.h"
//#include <random>
//#include <opencv2.4/opencv2/opencv.hpp>
//#include <vector>
//#pragma comment(lib, "ws2_32.lib") // Link to the Winsock library



//extern "C" __declspec(dllexport) int setupConnection();
extern "C" __declspec(dllexport) SOCKET setupConnection2(WSAStartupFunc wsaStartup = WSAStartup, socketFunc socketfunc = socket, connectFunc connectfunc = connect, WSACleanupFunc wsacleanupfunc = WSACleanup);
extern "C" __declspec(dllexport) int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc = send);
extern "C" __declspec(dllexport) int CloseSocket(SOCKET ClientSocket, closesocketFunc closeFunc = closesocket, WSACleanupFunc wsacleanupfunc = WSACleanup);


//SOCKET setupConnection() {
//    WSADATA wsaData;
//
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//        std::cout << "ERROR: Failed to start WSA" << std::endl;
//
//        return -1;
//    }
//
//    SOCKET ServerSocket;
//    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//
//    if (ServerSocket == INVALID_SOCKET) {
//        WSACleanup();
//
//        std::cout << "ERROR: Failed to create ServerSocket" << std::endl;
//
//        return -1;
//    }
//
//    sockaddr_in SvrAddr;
//    SvrAddr.sin_family = AF_INET;
//    SvrAddr.sin_addr.s_addr = INADDR_ANY;
//    SvrAddr.sin_port = htons(27000);
//    if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
//    {
//        std::cout << "ERROR: Failed to bind ServerSocket" << std::endl;
//
//        closesocket(ServerSocket);
//
//        WSACleanup();
//
//        return -1;
//    }
//
//
//    if (listen(ServerSocket, 1) == -1) {
//        std::cout << "ERROR: listen failed to configure ServerSocket" << std::endl;
//
//        closesocket(ServerSocket);
//
//        WSACleanup();
//
//        return -1;
//    }
//
//    return ServerSocket;
//}

///// CHANGED THE CLOSESOCKET FUNCTION FOR TESTING PURPOSES
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

    SOCKET ClientSocket;
    ClientSocket = socketfunc(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // here we are using TCP protocol

    if (ClientSocket == INVALID_SOCKET)
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

    if ((connectfunc(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR)
    {
        CloseSocket(ClientSocket);

        std::cout << "ERROR: Connection attempted Failed" << std::endl;

        return 0;
    }

    else {
        std::cout << "Connection Established" << std::endl;
    }


    return ClientSocket;
}



//int sendData(SOCKET ConnectionSocket, char* TxBuffer, int size) {
//
//}
//
//
//bool userFound;
//
//int callback(void* NotUsed, int argc, char** argv, char** azColName) {
//    userFound = true; // Set the flag to true if any rows are returned
//
//    return 0;
//}

//
int main()
{

//    SOCKET ServerSocket = setupConnection();
//
//    while (1) {
//        SOCKET ConnectionSocket;
//        if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == -1) {
//            closesocket(ServerSocket);
//
//            WSACleanup();
//
//            return -1;
//        }
//
//        // printing out a message if the process is succesful
//        std::cout << "Server Socket Successfully Binded" << endl;
//
//
//        while (1) {
//            char RxBuffer[400000];   //declaring a receive buffer with size 128
//
//
//            // receive the client message by passing the ServerSocket, address and size of the receive buffer, the 0 flag, client address structure, and the size of the client address structure
//            //Note: buffer is a pointer to the data (it is an array which means it acts as a pointer to the first element of the array)
//
//           /* recv(ServerSocket, RxBuffer, sizeof(RxBuffer), 0);*/
//
//            int receive_result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
//
//
//            // if the return value of the 'recvfrom' function is -1, close the server socket and end the program execution
//            if (receive_result < 0)
//            {
//                closesocket(ConnectionSocket);
//                closesocket(ServerSocket);
//                WSACleanup();
//                std::cout << "ERROR: did not receive anything from client" << endl;  // if recvfrom returns -1 it means the process was unsuccessfull
//                return 0;
//            }
//
//            std::cout << "Message Received from Client: " << RxBuffer << endl;
//
//            Packet* Pkt = CreatePacket();
//
//            LogIn log;
//
//            SignUp signup;
//
//            SignUpCheck check;
//
//            Listing list;
//
//            Deserialization(Pkt, RxBuffer, log, signup, check, list);
//
//            //if ((strcmp(Pkt->GetHead()->Route, "SIGNUP_USERCHECK") != 0) || (strcmp(Pkt->GetHead()->Route, "MYPOSTS_COUNT") != 0)) {
//            //    Display(Pkt, std::cout, log, signup, list);
//
//            //    /*char* imageBuff = Pkt->GetBody()->Data + (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email));*/
//            //}
//
//
//
//            /*   SQLiteDatabase db("database.db");
//               if (!db.isOpen()) {
//                   std:cerr << "Failed to open db" << std::endl;
//                   return 1;
//               }*/
//               //int id = int(Pkt->GetBody()->User);
//            //std::random_device rd;
//            //std::mt19937 gen(rd()); // Mersenne Twister engine seeded with rd()
//
//            //// Define the range for the random integer
//            //int min_value = 1;
//            //int max_value = 100;
//
//            //// Generate a random integer
//            //std::uniform_int_distribution<int> distribution(min_value, max_value);
//            //int id = distribution(gen);
//            //std::string profile_picture = signup.ImageStructArray;
//
//           /* const char* sql = "DROP TABLE IF EXISTS users;"
//                "CREATE TABLE users ("
//                "id INTEGER PRIMARY KEY,"
//                "username TEXT NOT NULL,"
//                "password TEXT NOT NULL,"
//                "email TEXT NOT NULL,"
//                "profile_picture BLOB"
//                ");";*/
//
//                //if (!db.executeQuery(sql)) {
//                //    std::cerr << "Query error: " << std::endl; // Log the query error
//                //}
//                //else {
//                //    std::cout << "table created" << std::endl;
//                //}
//
//
//                /*std::string query = "INSERT INTO users (id, username, password, email, profile_picture) VALUES ("
//                    + std::to_string(id) + ", "
//                    + "'" + signup.username + "', "
//                    + "'" + signup.password + "', "
//                    + "'" + signup.email + "', "
//                    + "'" + *(signup.ImageStructArray) + "')";*/
//                    /*std::string dbPath = "database.db";
//
//                    SQLiteDatabase sqldb(dbPath);*/
//
//                    //std::ifstream file(dbPath);
//                    //bool fileExists = file.good();
//                    //file.close();
//
//                    //// If the file does not exist, create it
//                    //if (!fileExists) {
//                    //    std::ofstream createFile(dbPath);
//                    //    createFile.close();
//                    //}
//
//                    //// Open the database
//                    //sqlite3* db;
//
//                    //int rc = sqlite3_open(dbPath.c_str(), &db);
//                    //if (rc != SQLITE_OK) {
//                    //    std::cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << std::endl;
//                    //}
//
//
//            char* errMsg = 0;
//
//            if (strcmp(Pkt->GetHead()->Route, "POST") == 0) {
//                std::string dbPath = "database.db";
//
//                SQLiteDatabase sqldb(dbPath);
//
//                const char* sqlCreateTableListing = "CREATE TABLE IF NOT EXISTS listings ("
//                    "id INTEGER NOT NULL,"
//                    "title TEXT PRIMARY KEY,"
//                    "location TEXT NOT NULL,"
//                    "condition TEXT NOT NULL,"
//                    "estimated_worth TEXT NOT NULL,"
//                    "delivery TEXT NOT NULL,"
//                    "looking_for TEXT NOT NULL,"
//                    "listing_picture BLOB NOT NULL);";
//
//                bool query_exe_result = sqldb.executeQuery(sqlCreateTableListing);
//
//                if (!query_exe_result) {
//                    return -1;
//                }
//
//                sqlite3_stmt* stmt = nullptr;
//
//                int ListingPostInsertionReturn = sqldb.ListingPostInsert(&stmt, Pkt, list);
//
//                if (ListingPostInsertionReturn == -1) {
//                    return ListingPostInsertionReturn;
//                }
//
//                /*char* ListingImageArray = nullptr;
//                int listingImageSize = 0;
//
//                sqldb.FetchImage(&stmt, list.Title, &ListingImageArray, listingImageSize);
//
//
//                int sendSize = send(ConnectionSocket, ListingImageArray, listingImageSize, 0);
//
//                if (sendSize < 0) {
//                    std::cout << "Sending Image Failed!!" << std::endl;
//
//                    return -1;
//                }
//
//                else {
//                    std::cout << "Image Successfully sent!! Wohoooooo" << std::endl;
//                }
//
//                delete ListingImageArray;*/
//
//                sqldb.closeDatabase(&stmt);
//            }
//
//            else if (strcmp(Pkt->GetHead()->Route, "MYPOSTS_COUNT") == 0) {
//                std::string dbPath = "database.db";
//
//                SQLiteDatabase sqldb(dbPath);
//
//                sqlite3_stmt* stmt;
//
//                int numOfRows;
//
//                const char* sqlCountListingRows = "SELECT COUNT(*) FROM listings;";
//
//
//
//                // Prepare the SQL statement
//                int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlCountListingRows, -1, &stmt, NULL);
//                if (rc != SQLITE_OK) {
//                    std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
//                }
//
//
//
//                // Execute the SQL statement
//                rc = sqlite3_step(stmt);
//                if (rc == SQLITE_ROW) {
//                    numOfRows = sqlite3_column_int(stmt, 0);
//                    std::cout << "Number of rows in the table: " << numOfRows << std::endl;
//                }
//
//                sqldb.closeDatabase(&stmt);
//                stmt = nullptr;
//
//
//
//                std::string source = "127.0.0.1";
//                std::string destination = "127.0.0.1";
//                std::string route = "MYPOSTS_COUNT";
//                bool auth = true;
//                unsigned int length = numOfRows;
//
//
//                Packet* pkt = CreatePacket();
//
//                memcpy(pkt->GetHead()->Source, source.c_str(), source.length());
//                memcpy(pkt->GetHead()->Destination, destination.c_str(), destination.length());
//                memcpy(pkt->GetHead()->Route, route.c_str(), route.length());
//
//                pkt->GetHead()->Authorization = auth;
//                pkt->GetHead()->Length = length;
//
//                char* TxBuffer = new char[sizeof(*(pkt->GetHead()))];
//
//                memset(TxBuffer, 0, sizeof(*(pkt->GetHead())));
//
//
//                memcpy(TxBuffer, pkt->GetHead(), sizeof(*(pkt->GetHead())));
//
//
//                send(ConnectionSocket, TxBuffer, sizeof(*(pkt->GetHead())), 0);
//
//
//
//
//
//                const char* sqlSelectRows = "SELECT * FROM listings;";
//
//                SQLiteDatabase sqldb2(dbPath);
//
//                rc = sqlite3_prepare_v2(sqldb2.getDB(), sqlSelectRows, -1, &stmt, NULL);
//                if (rc != SQLITE_OK) {
//                    std::cerr << "SQL error: " << sqlite3_errmsg(sqldb2.getDB()) << std::endl;
//                }
//
//
//
//
//                // Step through each row
//                while (sqlite3_step(stmt) == SQLITE_ROW) {
//                    Listing list;
//
//                    memset(list.Title, 0, sizeof(list.Title));
//                    memset(list.Location, 0, sizeof(list.Location));
//                    memset(list.Condition, 0, sizeof(list.Condition));
//                    memset(list.EstimatedWorth, 0, sizeof(list.EstimatedWorth));
//                    memset(list.Delivery, 0, sizeof(list.Delivery));
//                    memset(list.LookingFor, 0, sizeof(list.LookingFor));
//
//                    // Retrieve data from the current row
//                    int id = sqlite3_column_int(stmt, 0);
//                    int idSize = sqlite3_column_bytes(stmt, 0);
//
//                    const unsigned char* title = sqlite3_column_text(stmt, 1);
//                    int titleSize = sqlite3_column_bytes(stmt, 1);
//
//                    const unsigned char* location = sqlite3_column_text(stmt, 2);
//                    int locationSize = sqlite3_column_bytes(stmt, 2);
//
//                    const unsigned char* condition = sqlite3_column_text(stmt, 3);
//                    int conditionSize = sqlite3_column_bytes(stmt, 3);
//
//                    const unsigned char* estimated_worth = sqlite3_column_text(stmt, 4);
//                    int worthSize = sqlite3_column_bytes(stmt, 4);
//
//                    const unsigned char* delivery = sqlite3_column_text(stmt, 5);
//                    int deliverySize = sqlite3_column_bytes(stmt, 5);
//
//                    const unsigned char* looking_for = sqlite3_column_text(stmt, 6);
//                    int looking_for_size = sqlite3_column_bytes(stmt, 6);
//
//                    // Retrieve BLOB data
//                    const void* blobArray = sqlite3_column_blob(stmt, 7);
//
//                    unsigned int postImageSize = sqlite3_column_bytes(stmt, 7);
//
//
//                    memcpy(&(list.Title), title, titleSize);
//                    memcpy(&(list.Location), location, locationSize);
//                    memcpy(&(list.Condition), condition, conditionSize);
//                    memcpy(&(list.EstimatedWorth), estimated_worth, worthSize);
//                    memcpy(&(list.Delivery), delivery, deliverySize);
//                    memcpy(&(list.LookingFor), looking_for, looking_for_size);
//
//                    list.ImageStructArray = new char[postImageSize];
//
//                    memset(list.ImageStructArray, 0, postImageSize);
//
//                    memcpy(list.ImageStructArray, (char*)blobArray, postImageSize);
//
//
//
//
//                    int TotalSize = (6 * sizeof(list.Title)) + postImageSize + sizeof(unsigned int);
//
//                    char* TxBuffer = new char[TotalSize];
//
//                    memset(TxBuffer, 0, TotalSize);
//
//                    unsigned int sizeOfImage = postImageSize;
//
//
//                    memcpy(TxBuffer, &(list.Title), sizeof(list.Title));
//
//                    memcpy(TxBuffer + sizeof(list.Title), &(list.Location), sizeof(list.Location));
//
//                    memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location), &(list.Condition), sizeof(list.Condition));
//
//                    memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition), &(list.EstimatedWorth), sizeof(list.EstimatedWorth));
//
//                    memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth), &(list.Delivery), sizeof(list.Delivery));
//
//                    memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery), &(list.LookingFor), sizeof(list.LookingFor));
//
//                    memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor), &sizeOfImage, sizeof(sizeOfImage));
//
//                    memcpy(TxBuffer + sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor) + sizeof(sizeOfImage), list.ImageStructArray, sizeOfImage);
//
//
//                    int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);
//
//                    delete[] TxBuffer;
//                    TxBuffer = nullptr;
//                }
//            }
//
//            else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_IMAGEUPLOADED") == 0) {
//                std::string dbPath = "database.db";
//
//                SQLiteDatabase sqldb(dbPath);
//
//                // SQL command to create table
//                const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithProfile ("
//                    "id INTEGER NOT NULL,"
//                    "username TEXT NOT NULL,"
//                    "password TEXT NOT NULL,"
//                    "email TEXT PRIMARY KEY,"
//                    "profile_picture BLOB NOT NULL);";
//
//
//                bool query_exe_result = sqldb.executeQuery(sqlCreateTable);
//
//                if (!query_exe_result) {
//                    return -1;
//                }
//
//
//
//                sqlite3_stmt* stmt = nullptr;
//
//                int SignUpdataInsertionReturn = sqldb.SignUpWithImageDataInsert(&stmt, Pkt, signup);
//
//                if (SignUpdataInsertionReturn == -1) {
//                    return SignUpdataInsertionReturn;
//                }
//
//
//
//                // Retrieve BLOB data
//                char* imageArray = nullptr;
//                int imageSize = 0;
//
//                sqldb.FetchImage(&stmt, signup.email, &imageArray, imageSize);
//
//
//
//
//                int sendSize = send(ConnectionSocket, imageArray, imageSize, 0);
//
//                /*Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email))*/
//
//                if (sendSize < 0) {
//                    std::cout << "Sending Image Failed!!" << std::endl;
//
//                    return -1;
//                }
//
//                else {
//                    std::cout << "Image Successfully sent!! Wohoooooo" << std::endl;
//                }
//
//                delete[] imageArray;
//
//                /*sqlite3_finalize(stmt);*/
//
//                /*Finalize the statement and close the database connection*/
//                sqldb.closeDatabase(&stmt);
//            }
//
//            else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_IMAGENOTUPLOADED") == 0) {
//                std::string dbPath = "database.db";
//
//                SQLiteDatabase sqldb(dbPath);
//
//                // SQL command to create table
//                const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithoutProfile ("
//                    "id INTEGER NOT NULL,"
//                    "username TEXT NOT NULL,"
//                    "password TEXT NOT NULL,"
//                    "email TEXT PRIMARY KEY,"
//                    "profile_picture_SubstituteData TEXT NOT NULL);";
//
//
//                bool query_exe_result = sqldb.executeQuery(sqlCreateTable);
//
//                if (!query_exe_result) {
//                    return -1;
//                }
//
//
//
//                sqlite3_stmt* stmt = nullptr;
//
//                int SignUpdataInsertionReturn = sqldb.SignUpWithoutImageDataInsert(&stmt, Pkt, signup);
//
//                if (SignUpdataInsertionReturn == -1) {
//                    return SignUpdataInsertionReturn;
//                }
//
//                /*sqlite3_finalize(stmt);*/
//                sqldb.closeDatabase(&stmt);
//            }
//
//            else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_USERCHECK") == 0) {
//                std::string dbPath = "database.db";
//
//                SQLiteDatabase sqldb(dbPath);
//
//                userFound = false;
//
//                std::ostringstream oss;
//                oss << "SELECT username, email FROM UsersWithProfile WHERE username = '" << check.username << "';";
//                std::string withProfile_query_str = oss.str();
//
//                int (*callback_ptr)(void*, int, char**, char**);
//
//                callback_ptr = callback;
//
//                char* errMsg = 0;
//
//
//                int rc = sqlite3_exec(sqldb.getDB(), withProfile_query_str.c_str(), &callback, 0, &errMsg);
//                if (rc != SQLITE_OK) {
//                    std::cerr << "SQL error: " << errMsg << std::endl;
//                    sqlite3_free(errMsg);
//                }
//
//
//                if (!userFound) {
//                    oss.clear();
//
//                    oss << "SELECT username, email FROM UsersWithoutProfile WHERE username = '" << check.username << "';";
//
//                    std::string withoutProfile_query_str = oss.str();
//
//                    rc = sqlite3_exec(sqldb.getDB(), withoutProfile_query_str.c_str(), &callback, 0, &errMsg);
//
//                    if (rc != SQLITE_OK) {
//                        std::cerr << "SQL error: " << errMsg << std::endl;
//                        sqlite3_free(errMsg);
//                    }
//                }
//
//                /*Close the database connection*/
//                sqlite3_close(sqldb.getDB());
//
//                Packet pkt;
//
//                char source[20] = "127.0.0.1";
//                int source_size = sizeof(source);
//
//                char destination[20] = "127.0.0.1";
//                int destination_size = sizeof(destination);
//
//                char Route[40] = "LOGIN";
//                int Route_size = sizeof(Route);
//
//                bool Authorization = userFound;
//
//                int length = 0;
//
//                SetHeaderInformation(&pkt, source, source_size, destination, destination_size, Route, Route_size, Authorization, length);
//
//                int TotalSize = 0;
//
//                char* TxBuffer = SerializeUserCheckingData(&pkt, TotalSize);
//
//                int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);
//
//                /*Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email))*/
//
//                if (sendSize < 0) {
//                    std::cout << "Sending User Check Failed!!" << std::endl;
//
//                    return -1;
//                }
//
//                else {
//                    std::cout << "Response for User Check Successfully sent!! Wohoooooo" << std::endl;
//                }
//
//                delete[] TxBuffer;
//                TxBuffer = nullptr;
//            }
//
//            DestroyPacket(Pkt);
//
//
//
//
//            /*rc = sqlite3_exec(db, sqlCreateTable, 0, 0, &errMsg);
//
//
//            if (rc != SQLITE_OK) {
//                std::cerr << "SQL error: " << errMsg << std::endl;
//                sqlite3_free(errMsg);
//                sqlite3_close(db);
//
//                return -1;
//            }
//            else {
//                std::cout << "Table created successfully." << std::endl;
//            }*/
//
//
//
//            /*sqlite3_stmt* stmt;
//            const char* sql = "INSERT INTO users (id, username, password, email, profile_picture) VALUES (?, ?, ?, ?, ?)";
//            sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
//            sqlite3_bind_int(stmt, 1, id);
//            sqlite3_bind_text(stmt, 2, signup.username, -1, SQLITE_TRANSIENT);
//            sqlite3_bind_text(stmt, 3, signup.password, -1, SQLITE_TRANSIENT);
//            sqlite3_bind_text(stmt, 4, signup.email, -1, SQLITE_TRANSIENT);
//            sqlite3_bind_blob(stmt, 5, signup.ImageStructArray, Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email)), SQLITE_STATIC);
//
//            rc = sqlite3_step(stmt);
//            if (rc != SQLITE_DONE) {
//                std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
//                sqlite3_finalize(stmt);
//                sqlite3_close(db);
//                return 1;
//            }
//
//            std::cout << "Data inserted successfully!" << std::endl;*/
//
//
//
//            // Construct the query as a std::string
//            //std::ostringstream oss;
//            //oss << "SELECT profile_picture FROM users WHERE id = " << id;
//            //std::string query_str = oss.str();
//
//
//
//            //// Convert the std::string query to a const char*
//            //const char* query = query_str.c_str();
//
//            ////memcpy(query, ("SELECT profile_picture FROM users WHERE id = " + std::to_string(id)).c_str(), ("SELECT profile_picture FROM users WHERE id = " + std::to_string(id)).length());
//
//
//            //// Prepare the SQL statement
//            //rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
//            //if (rc != SQLITE_OK) {
//            //    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
//            //    sqlite3_close(db);
//            //    return 1;
//            //}
//
//
//            //// Execute the query
//            //rc = sqlite3_step(stmt);
//            //if (rc != SQLITE_ROW) {
//            //    std::cerr << "No data found" << std::endl;
//            //    sqlite3_finalize(stmt);
//            //    sqlite3_close(db);
//            //    return 1;
//            //}
//
//
//
//
//            //if (!db.executeQuery(sql)) {
//            //    std::cerr << "Query error: "<< std::endl; // Log the query error
//            //}
//            //else {
//            //    std::cout << "Saved to db" << std::endl;
//            //}
//
//            /*char retrieveUser[200000];
//
//            int size = Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email));
//
//            memcpy(retrieveUser, (char*)("SELECT profile_picture FROM users WHERE id = " + std::to_string(id)).c_str(), size);*/
//
//
//
//
//
//            /*BufferToImage::GetImageFromByteArray(imageBuff);*/
//
//            //// Assuming 'receivedData' is the byte array received from the client
//            //std::vector<unsigned char> receivedData; // Assuming receivedData contains the byte array
//
//            //// Convert byte array to OpenCV Mat object
//            //cv::Mat decodedImage = byteArrayToMat(receivedData);
//
//            //// Display or further process the decoded image
//            //cv::imshow("Decoded Image", decodedImage);
//            //cv::waitKey(0);
//
//        }
//
//        return 0;
//    }
//
//
//    //char TxBuffer[] = "Hello Client";
//    //// sending the success message to the client address saved from recvfrom()
//    //// by passng the ServerSocket, address and size of the transmit buffer, flag 0, client address structure and the size of the structure
//    //int send_result = sendto(ServerSocket, TxBuffer, sizeof(TxBuffer), 0, (struct sockaddr*)&CltAddr, sizeof(CltAddr));
//
//    //// if the return value of the 'sendto' function is -1, close the server socket and end the program
//    //if (send_result == -1)
//    //{
//    //    closesocket(ServerSocket);
//    //    WSACleanup();
//    //    // display an error message
//    //    cout << "Sending to client failed" << endl;
//
//    //    return 0;
//    //}
//
//    //// printing out a message if the process is succesful
//    //cout << "Sending Back to the Client: " << TxBuffer << endl;
//
//
//    // Cleaning up the ClientSocket and Winsock library
//    closesocket(ServerSocket);
//    std::cout << "Server socket closed" << endl;
//
//    WSACleanup();
//    std::cout << "Winsock library resources cleaned up and released" << endl;
//
//
//
//
   return 0;
}