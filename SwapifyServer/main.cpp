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
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <unordered_map>

#pragma comment(lib, "ws2_32.lib") // Link to the Winsock library

#include "Packet.h"

using namespace std;


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

    /**threadObjArray = new std::thread*[20];

    memset(*threadObjArray, 0, 20);

    int offset = 0;

    std::thread** temp = *threadObjArray;*/

    int i = 0;

    while (1) {
        SOCKET* ConnectionSocket = new SOCKET();
        if ((*ConnectionSocket = accept(ServerSocket, NULL, NULL)) == -1) {
            continue;

            /*closesocket(ServerSocket);

            WSACleanup();

            return -1;*/
        }


        // printing out a message if the process is succesful
        std::cout << "Server Socket Successfully Binded" << endl;


        // Lambda function to capture the parameters and call the desired function
        //auto threadFuncWithParams = [&](std::thread& threadObj) {
        //    threadedFunc(ConnectionSocket); // Pass the parameters to the function
        //    };

         // Create a copy of ConnectionSocket to pass to the thread
        /*SOCKET* connectionCopy = new SOCKET(ConnectionSocket);*/

        ///*std::thread* thread1 = new std::thread(funcptr, *ConnectionSocket);

        //*temp = thread1;*/

        //memcpy(**threadObjArray + offset, thread1, sizeof(thread1));

        /*(*thread1).join();*/

        /*(*threadObjArray + offset) = new std::thread(threadFuncWithParams, std::ref(*threadObjArray[count]));*/

        ///*offset++;

        //temp = temp + offset;*/

        std::thread clientThread(funcptr, *ConnectionSocket);

        {
            std::lock_guard<std::mutex> lock(threadMutex);
            threadPool.push_back(std::move(clientThread));

            //while (i <= cleanupFlags.size()) {
            //    cleanupFlags.push_back(std::atomic<bool>(false)); // Add new elements with initial value false // Add new elements with initial value false
            //}

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


        //// Find and mark threads for cleanup
        //std::vector<size_t> threadsToRemove;
        //for (size_t i = 0; i < cleanupFlags.size(); ++i) {
        //    if (cleanupFlags[i]) {
        //        threadsToRemove.push_back(i);
        //    }
        //}


        //// Find and Join threads marked for cleanup
        //for (auto it = threadsToRemove.rbegin(); it != threadsToRemove.rend(); ++it) {
        //    if (threadPool[*it].joinable()) {
        //        threadPool[*it].join();
        //    }

        //    cleanupFlags.erase(cleanupFlags.begin() + *it);

        //    threadPool.erase(threadPool.begin() + *it);

        //    //break;
        //}

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


//int sendData(SOCKET ConnectionSocket, char* TxBuffer, int size) {
//
//}


bool userFound;

int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    userFound = true; // Set the flag to true if any rows are returned

    return 0;
}


int threadedFunc(SOCKET ConnectionSocket) {
    /*while (1) {
        SOCKET ConnectionSocket;
        if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == -1) {
            closesocket(ServerSocket);

            WSACleanup();

            return -1;
        }*/

    // printing out a message if the process is succesful
    /*std::cout << "Server Socket Successfully Binded" << endl;*/

    unsigned int clientID = ++nextClientID;
    clientIDToConnections[clientID] = ConnectionSocket;

    while (1) {
        char RxBuffer[400000];   //declaring a receive buffer with size 128

        memset(RxBuffer, 0, 400000);


        // receive the client message by passing the ServerSocket, address and size of the receive buffer, the 0 flag, client address structure, and the size of the client address structure
        //Note: buffer is a pointer to the data (it is an array which means it acts as a pointer to the first element of the array)

        /* recv(ServerSocket, RxBuffer, sizeof(RxBuffer), 0);*/

        int receive_result = recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);


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



        char* errMsg = 0;

        if (strcmp(Pkt->GetHead()->Route, "POST") == 0) {
            std::string dbPath = "database.db";

            SQLiteDatabase sqldb(dbPath);

            const char* sqlCreateTableListing = "CREATE TABLE IF NOT EXISTS listings ("
                "id INTEGER NOT NULL,"
                "title TEXT PRIMARY KEY,"
                "location TEXT NOT NULL,"
                "condition TEXT NOT NULL,"
                "estimated_worth TEXT NOT NULL,"
                "delivery TEXT NOT NULL,"
                "looking_for TEXT NOT NULL,"
                "listing_picture BLOB NOT NULL);";

            bool query_exe_result = sqldb.executeQuery(sqlCreateTableListing);

            if (!query_exe_result) {
                return -1;
            }

            sqlite3_stmt* stmt = nullptr;

            int ListingPostInsertionReturn = sqldb.ListingPostInsert(&stmt, Pkt, list);

            if (ListingPostInsertionReturn == -1) {
                return ListingPostInsertionReturn;
            }

            sqldb.closeDatabase(&stmt);
        }

        else if (strcmp(Pkt->GetHead()->Route, "STARTUP_GETID") == 0) {
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


            int sendSize = send(ConnectionSocket, TxBuffer, sizeof(*(pkt->GetHead())) + sizeof(pkt->GetBody()->User), 0);
        }

        else if (strcmp(Pkt->GetHead()->Route, "DELETE_POST") == 0) {
            std::string dbPath = "database.db";

            SQLiteDatabase sqldb(dbPath);

            sqlite3_stmt* stmt;

            std::ostringstream oss;

            oss << "DELETE FROM listings WHERE title = '" << list.Title << "';";

            std::string query_str = oss.str();

            const char* sqldeletepost = query_str.c_str();



            // Prepare the SQL statement
            int rc = sqlite3_prepare_v2(sqldb.getDB(), sqldeletepost, -1, &stmt, NULL);
            if (rc != SQLITE_OK) {
                std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
            }


            // Execute the SQL statement
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_DONE) {
                std::cout << "Post Successfully Deleted" << std::endl;
            }

            sqldb.closeDatabase(&stmt);
            stmt = nullptr;
        }



        else if (strcmp(Pkt->GetHead()->Route, "MYPOSTS_COUNT") == 0) {
            std::string dbPath = "database.db";

            SQLiteDatabase sqldb(dbPath);

            sqlite3_stmt* stmt;

            int numOfRows;

            std::ostringstream oss;
            oss << "SELECT COUNT(*) FROM listings WHERE id = '" << Pkt->GetBody()->User << "';";
            std::string query_str = oss.str();

            const char* sqlCountListingRows = query_str.c_str();



            // Prepare the SQL statement
            int rc = sqlite3_prepare_v2(sqldb.getDB(), sqlCountListingRows, -1, &stmt, NULL);
            if (rc != SQLITE_OK) {
                std::cerr << "SQL error: " << sqlite3_errmsg(sqldb.getDB()) << std::endl;
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


            Packet* pkt = CreatePacket();

            memcpy(pkt->GetHead()->Source, source.c_str(), source.length());
            memcpy(pkt->GetHead()->Destination, destination.c_str(), destination.length());
            memcpy(pkt->GetHead()->Route, route.c_str(), route.length());

            pkt->GetHead()->Authorization = auth;
            pkt->GetHead()->Length = length;

            char* TxBuffer = new char[sizeof(*(pkt->GetHead()))];

            memset(TxBuffer, 0, sizeof(*(pkt->GetHead())));


            memcpy(TxBuffer, pkt->GetHead(), sizeof(*(pkt->GetHead())));


            send(ConnectionSocket, TxBuffer, sizeof(*(pkt->GetHead())), 0);





            oss.clear();
            oss.str("");

            oss << "SELECT * FROM listings WHERE id = '" << Pkt->GetBody()->User << "';";
            query_str.clear();

            query_str = oss.str();

            const char* sqlSelectRows = query_str.c_str();

            SQLiteDatabase sqldb2(dbPath);

            rc = sqlite3_prepare_v2(sqldb2.getDB(), sqlSelectRows, -1, &stmt, NULL);
            if (rc != SQLITE_OK) {
                std::cerr << "SQL error: " << sqlite3_errmsg(sqldb2.getDB()) << std::endl;
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


                int sendSize = send(ConnectionSocket, TxBuffer, TotalSize, 0);

                delete[] TxBuffer;
                TxBuffer = nullptr;
            }
        }

        else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_IMAGEUPLOADED") == 0) {
            std::string dbPath = "database.db";

            SQLiteDatabase sqldb(dbPath);

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

            int SignUpdataInsertionReturn = sqldb.SignUpWithImageDataInsert(&stmt, Pkt, signup);

            if (SignUpdataInsertionReturn == -1) {
                return SignUpdataInsertionReturn;
            }



            // Retrieve BLOB data
            char* imageArray = nullptr;
            int imageSize = 0;

            sqldb.FetchImage(&stmt, signup.email, &imageArray, imageSize);




            int sendSize = send(ConnectionSocket, imageArray, imageSize, 0);

            /*Pkt->GetHead()->Length - (sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email))*/

            if (sendSize < 0) {
                std::cout << "Sending Image Failed!!" << std::endl;

                return -1;
            }

            else {
                std::cout << "Image Successfully sent!! Wohoooooo" << std::endl;
            }

            delete[] imageArray;

            /*sqlite3_finalize(stmt);*/

            /*Finalize the statement and close the database connection*/
            sqldb.closeDatabase(&stmt);
        }

        else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_IMAGENOTUPLOADED") == 0) {
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

            int SignUpdataInsertionReturn = sqldb.SignUpWithoutImageDataInsert(&stmt, Pkt, signup);

            if (SignUpdataInsertionReturn == -1) {
                return SignUpdataInsertionReturn;
            }

            /*sqlite3_finalize(stmt);*/
            sqldb.closeDatabase(&stmt);
        }

        else if (strcmp(Pkt->GetHead()->Route, "SIGNUP_USERCHECK") == 0) {
            std::string dbPath = "database.db";

            SQLiteDatabase sqldb(dbPath);

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

            /*Close the database connection*/
            sqlite3_close(sqldb.getDB());

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

        DestroyPacket(Pkt);

    }

    return 0;
}



int main()
{
    //std::thread** threadObjArray = nullptr;

    //SOCKET ServerSocket = setupConnection(threadedFunc);


    // Start the server in a separate thread
    std::thread serverThread(setupConnection, threadedFunc);


    // Start the Cleanup thread
    std::thread cleanupThread(cleanupThreads);

     
    // Wait for the server thread to finish (never returns)
    serverThread.join();

    // Wait for the cleanup thread to finish (never returns)
    cleanupThread.join();
    

    /*for (int offset = 0; offset < 20; offset++) {
        if (!(threadObjArray)[offset]) {
            break;
        }

        else if (std::thread::id() != (*(threadObjArray[offset])).get_id()) {
            (*(threadObjArray[offset])).join();
        }

    }

    for (int i = 0; i < 20; i++) {
        if (!threadObjArray[i]) {
            break;
        }

        else {
            delete threadObjArray[i];
        }
    }

    delete[] threadObjArray;*/

    /*int (*funcptr)(SOCKET&) = threadedFunc;

    std::thread thread1(funcptr, std::ref(ServerSocket));

    std::thread thread2(funcptr, std::ref(ServerSocket));


    thread1.join();
    thread2.join();*/


    // Cleaning up the ClientSocket and Winsock library
    /*closesocket(ServerSocket);
    std::cout << "Server socket closed" << endl;*/

    WSACleanup();
    std::cout << "Winsock library resources cleaned up and released" << endl;




    return 0;
}

