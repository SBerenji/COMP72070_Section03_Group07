//#include "TCPServer.h"

//int main(void) {
//	TCPServer server = TCPServer(27000);
//	server.start();
//	return 1;
//}
#include <windows.networking.sockets.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include "SQLiteDatabase.h"
#include "file_utils.h"
#include <random>
//#include <opencv2.4/opencv2/opencv.hpp>
//#include <vector>
#pragma comment(lib, "ws2_32.lib") // Link to the Winsock library

#include "Packet.h"

using namespace std;

SOCKET setupConnection() {
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

    return ServerSocket;
}


//int sendData(SOCKET ConnectionSocket, char* TxBuffer, int size) {
//
//}


bool userFound;

int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    userFound = true; // Set the flag to true if any rows are returned

    return 0;
}


int main()
{
    SOCKET ServerSocket = setupConnection();

    while (1) {
        SOCKET ConnectionSocket;
        if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == -1) {
            closesocket(ServerSocket);

            WSACleanup();

            return -1;
        }

        // printing out a message if the process is succesful
        std::cout << "Server Socket Successfully Binded" << endl;


        while (1) {
            char RxBuffer[190000];   //declaring a receive buffer with size 128


            // receive the client message by passing the ServerSocket, address and size of the receive buffer, the 0 flag, client address structure, and the size of the client address structure
            //Note: buffer is a pointer to the data (it is an array which means it acts as a pointer to the first element of the array)

           /* recv(ServerSocket, RxBuffer, sizeof(RxBuffer), 0);*/

            int receive_result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);


            // if the return value of the 'recvfrom' function is -1, close the server socket and end the program execution
            if (receive_result < 0)
            {
                closesocket(ConnectionSocket);
                closesocket(ServerSocket);
                WSACleanup();
                std::cout << "ERROR: did not receive anything from client" << endl;  // if recvfrom returns -1 it means the process was unsuccessfull
                return 0;
            }

            std::cout << "Message Received from Client: " << RxBuffer << endl;

            Packet* Pkt = CreatePacket();

            LogIn log;
            Listing listing;
            SignUp signup;

            SignUpCheck check;

            Deserialization(Pkt, RxBuffer, log, signup, check);

            if (strcmp(Pkt->GetHead()->Route, "SIGNUP_USERCHECK") != 0) {
                Display(Pkt, std::cout, log, signup);

                char* imageBuff = Pkt->GetBody()->Data + (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email));
            }

            

            /*   SQLiteDatabase db("database.db");
               if (!db.isOpen()) {
                   std:cerr << "Failed to open db" << std::endl;
                   return 1;
               }*/
               //int id = int(Pkt->GetBody()->User);
            //std::random_device rd;
            //std::mt19937 gen(rd()); // Mersenne Twister engine seeded with rd()

            //// Define the range for the random integer
            //int min_value = 1;
            //int max_value = 100;

            //// Generate a random integer
            //std::uniform_int_distribution<int> distribution(min_value, max_value);
            //int id = distribution(gen);
            //std::string profile_picture = signup.ImageStructArray;

           /* const char* sql = "DROP TABLE IF EXISTS users;"
                "CREATE TABLE users ("
                "id INTEGER PRIMARY KEY,"
                "username TEXT NOT NULL,"
                "password TEXT NOT NULL,"
                "email TEXT NOT NULL,"
                "profile_picture BLOB"
                ");";*/

                //if (!db.executeQuery(sql)) {
                //    std::cerr << "Query error: " << std::endl; // Log the query error
                //}
                //else {
                //    std::cout << "table created" << std::endl;
                //}


                /*std::string query = "INSERT INTO users (id, username, password, email, profile_picture) VALUES ("
                    + std::to_string(id) + ", "
                    + "'" + signup.username + "', "
                    + "'" + signup.password + "', "
                    + "'" + signup.email + "', "
                    + "'" + *(signup.ImageStructArray) + "')";*/
            std::string dbPath = "database.db";

            SQLiteDatabase sqldb(dbPath);

            //std::ifstream file(dbPath);
            //bool fileExists = file.good();
            //file.close();

            //// If the file does not exist, create it
            //if (!fileExists) {
            //    std::ofstream createFile(dbPath);
            //    createFile.close();
            //}

            //// Open the database
            //sqlite3* db;

            //int rc = sqlite3_open(dbPath.c_str(), &db);
            //if (rc != SQLITE_OK) {
            //    std::cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << std::endl;
            //}


            char* errMsg = 0;

            if (strcmp(Pkt->GetHead()->Route, "POST") == 0) {
                const char* sqlCreateTableListing = "CREATE TABLE IF NOT EXISTS listings ("
                    "id INTEGER PRIMARY KEY,"
                    "title TEXT NOT NULL,"
                    "estimated_worth TEXT NOT NULL,"
                    "location TEXT NOT NULL,"
                    "condition TEXT NOT NULL,"
                    "delivery TEXT NOT NULL,"
                    "looking_for TEXT NOT NULL,"
                    "listing_picture BLOB NOT NULL,"
                    "listing_date DATETIME NOT NULL"
                    ");";

                bool query_exe_result = sqldb.executeQuery(sqlCreateTableListing);

                if (!query_exe_result) {
                    return -1;
                }

                sqlite3_stmt* stmt = nullptr;

                int ListingPostInsertionReturn = sqldb.ListingPostInsert(stmt, Pkt, listing);

                if (ListingPostInsertionReturn == -1) {
                    return ListingPostInsertionReturn;
                }

                char* ListingImageArray = nullptr;
                int listingImageSize = 0;

                sqldb.FetchImage(stmt, (int)(Pkt->GetBody()->User), &ListingImageArray, listingImageSize);


                int sendSize = send(ConnectionSocket, ListingImageArray, listingImageSize, 0);

                if (sendSize < 0) {
                    std::cout << "Sending Image Failed!!" << std::endl;

                    return -1;
                }

                else {
                    std::cout << "Image Successfully sent!! Wohoooooo" << std::endl;
                }

                delete ListingImageArray;

                sqldb.closeDatabase(stmt);
            }



            if (strcmp(Pkt->GetHead()->Route, "SIGNUP_IMAGEUPLOADED") == 0) {
                // SQL command to create table
                const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithProfile ("
                    "id INTEGER PRIMARY KEY,"
                    "username TEXT NOT NULL,"
                    "password TEXT NOT NULL,"
                    "email TEXT NOT NULL,"
                    "profile_picture BLOB NOT NULL);";


                bool query_exe_result = sqldb.executeQuery(sqlCreateTable);

                if (!query_exe_result) {
                    return -1;
                }



                sqlite3_stmt* stmt = nullptr;

                int SignUpdataInsertionReturn = sqldb.SignUpWithImageDataInsert(stmt, Pkt, signup);

                if (SignUpdataInsertionReturn == -1) {
                    return SignUpdataInsertionReturn;
                }



                // Retrieve BLOB data
                char* imageArray = nullptr;
                int imageSize = 0;

                sqldb.FetchImage(stmt, (int)(Pkt->GetBody()->User), &imageArray, imageSize);




                int sendSize = send(ConnectionSocket, imageArray, imageSize, 0);

                /*Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email))*/

                if (sendSize < 0) {
                    std::cout << "Sending Image Failed!!" << std::endl;

                    return -1;
                }

                else {
                    std::cout << "Image Successfully sent!! Wohoooooo" << std::endl;
                }

                delete imageArray;

                // Finalize the statement and close the database connection
                sqldb.closeDatabase(stmt);
            }

            else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_IMAGENOTUPLOADED") == 0) {
                // SQL command to create table
                const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithoutProfile ("
                    "id INTEGER PRIMARY KEY,"
                    "username TEXT NOT NULL,"
                    "password TEXT NOT NULL,"
                    "email TEXT NOT NULL,"
                    "profile_picture_SubstituteData TEXT NOT NULL);";


                bool query_exe_result = sqldb.executeQuery(sqlCreateTable);

                if (!query_exe_result) {
                    return -1;
                }



                sqlite3_stmt* stmt = nullptr;

                int SignUpdataInsertionReturn = sqldb.SignUpWithoutImageDataInsert(stmt, Pkt, signup);

                if (SignUpdataInsertionReturn == -1) {
                    return SignUpdataInsertionReturn;
                }


                sqldb.closeDatabase(stmt);
            }

            else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_USERCHECK") == 0) {
                userFound = false;

                std::ostringstream oss;
                oss << "SELECT username, email FROM UsersWithProfile WHERE username = '" << check.username << "';";
                std::string withProfile_query_str = oss.str();

                int (*callback_ptr)(void*, int, char**, char**);

                callback_ptr = callback;

                char* errMsg = 0;


                int rc = sqlite3_exec(sqldb.getDB(), withProfile_query_str.c_str(), &callback, 0, &errMsg);
                if (rc != SQLITE_OK) {
                    std::cerr << "SQL error: " << errMsg << std::endl;
                    sqlite3_free(errMsg);
                }


                if (!userFound) {
                    oss.clear();

                    oss << "SELECT username, email FROM UsersWithoutProfile WHERE username = '" << check.username << "';";

                    std::string withoutProfile_query_str = oss.str();

                    rc = sqlite3_exec(sqldb.getDB(), withoutProfile_query_str.c_str(), &callback, 0, &errMsg);

                    if (rc != SQLITE_OK) {
                        std::cerr << "SQL error: " << errMsg << std::endl;
                        sqlite3_free(errMsg);
                    }
                }

                Packet pkt;

                char source[20] = "127.0.0.1";
                int source_size = sizeof(source);

                char destination[20] = "127.0.0.1";
                int destination_size = sizeof(destination);

                char Route[40] = "LOGIN";
                int Route_size = sizeof(Route);

                bool Authorization = userFound;

                int length = 0;

                SetHeaderInformation(&pkt, source, source_size, destination, destination_size, Route, Route_size, Authorization, length);

                int TotalSize = 0;

                char* TxBuffer = SerializeUserCheckingData(&pkt, TotalSize);

                int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);

                /*Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email))*/

                if (sendSize < 0) {
                    std::cout << "Sending User Check Failed!!" << std::endl;

                    return -1;
                }

                else {
                    std::cout << "Response for User Check Successfully sent!! Wohoooooo" << std::endl;
                }

                delete[] TxBuffer;
                TxBuffer = nullptr;
            }


            

            /*rc = sqlite3_exec(db, sqlCreateTable, 0, 0, &errMsg);


            if (rc != SQLITE_OK) {
                std::cerr << "SQL error: " << errMsg << std::endl;
                sqlite3_free(errMsg);
                sqlite3_close(db);

                return -1;
            }
            else {
                std::cout << "Table created successfully." << std::endl;
            }*/

            

            /*sqlite3_stmt* stmt;
            const char* sql = "INSERT INTO users (id, username, password, email, profile_picture) VALUES (?, ?, ?, ?, ?)";
            sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            sqlite3_bind_int(stmt, 1, id);
            sqlite3_bind_text(stmt, 2, signup.username, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, signup.password, -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, signup.email, -1, SQLITE_TRANSIENT);
            sqlite3_bind_blob(stmt, 5, signup.ImageStructArray, Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email)), SQLITE_STATIC);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return 1;
            }

            std::cout << "Data inserted successfully!" << std::endl;*/



            // Construct the query as a std::string
            //std::ostringstream oss;
            //oss << "SELECT profile_picture FROM users WHERE id = " << id;
            //std::string query_str = oss.str();



            //// Convert the std::string query to a const char*
            //const char* query = query_str.c_str();

            ////memcpy(query, ("SELECT profile_picture FROM users WHERE id = " + std::to_string(id)).c_str(), ("SELECT profile_picture FROM users WHERE id = " + std::to_string(id)).length());


            //// Prepare the SQL statement
            //rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
            //if (rc != SQLITE_OK) {
            //    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            //    sqlite3_close(db);
            //    return 1;
            //}


            //// Execute the query
            //rc = sqlite3_step(stmt);
            //if (rc != SQLITE_ROW) {
            //    std::cerr << "No data found" << std::endl;
            //    sqlite3_finalize(stmt);
            //    sqlite3_close(db);
            //    return 1;
            //}


            

            //if (!db.executeQuery(sql)) {
            //    std::cerr << "Query error: "<< std::endl; // Log the query error
            //}
            //else {
            //    std::cout << "Saved to db" << std::endl;
            //}

            /*char retrieveUser[200000];

            int size = Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email));

            memcpy(retrieveUser, (char*)("SELECT profile_picture FROM users WHERE id = " + std::to_string(id)).c_str(), size);*/





            /*BufferToImage::GetImageFromByteArray(imageBuff);*/

            //// Assuming 'receivedData' is the byte array received from the client
            //std::vector<unsigned char> receivedData; // Assuming receivedData contains the byte array

            //// Convert byte array to OpenCV Mat object
            //cv::Mat decodedImage = byteArrayToMat(receivedData);

            //// Display or further process the decoded image
            //cv::imshow("Decoded Image", decodedImage);
            //cv::waitKey(0);
        }

        return 0;
    }


    //char TxBuffer[] = "Hello Client";
    //// sending the success message to the client address saved from recvfrom()
    //// by passng the ServerSocket, address and size of the transmit buffer, flag 0, client address structure and the size of the structure
    //int send_result = sendto(ServerSocket, TxBuffer, sizeof(TxBuffer), 0, (struct sockaddr*)&CltAddr, sizeof(CltAddr));

    //// if the return value of the 'sendto' function is -1, close the server socket and end the program
    //if (send_result == -1)
    //{
    //    closesocket(ServerSocket);
    //    WSACleanup();
    //    // display an error message
    //    cout << "Sending to client failed" << endl;

    //    return 0;
    //}

    //// printing out a message if the process is succesful
    //cout << "Sending Back to the Client: " << TxBuffer << endl;


    // Cleaning up the ClientSocket and Winsock library
    closesocket(ServerSocket);
    std::cout << "Server socket closed" << endl;

    WSACleanup();
    std::cout << "Winsock library resources cleaned up and released" << endl;




    return 0;
}

