#include "pch.h"
#include "gtest/gtest.h"

#include "../SwapifyServer/SQLiteDatabase.h"
#include "../SwapifyServer/SQLiteDatabase.cpp"
#include "../SwapifyServer/file_utils.h"
#include "../SwapifyServer/file_utils.cpp"
#include "../SwapifyServer/Packet.h"
#include "../SwapifyServer/Packet.cpp"
#include "../SwapifyServer/User.h"
#include "../SwapifyServer/UsersRoute.h"
#include "../SwapifyServer/UserRoutes.cpp"

#include <iostream>
#include <sstream>
#include <string>
#include <functional>

std::string capture_stderr(std::function<void()> func) //saves the standard error returned by the function to the stringstream object and returns it as a string
{ 
   // std::streambuf* originalErr = std::cerr.rdbuf(); //takes the current buffer and saves it to originalErr
    std::stringstream newErr; //new stringstream object called newErr
    std::cerr.rdbuf(newErr.rdbuf()); 

    func(); //execute the function passed as a parameter
   // std::cerr.rdbuf(originalErr); //originalErr is placed back in the buffer

    return newErr.str(); //return newErr object as a string
}

Packet* dummyPacketPostInfo(Listing& list) {
    Packet* Pkt = new Packet();

    Pkt->GetBody()->Data = nullptr;


    memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
    memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
    memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


    SetHeaderInformation(Pkt, "Source", strlen("Source"), "Destination", strlen("Destination"), "POST", strlen("POST"), true, 0);
    strcpy_s(list.Title, "Title");
    strcpy_s(list.Location, "Location");
    strcpy_s(list.Condition, "Condition");
    strcpy_s(list.EstimatedWorth, "EstimatedWorth");
    strcpy_s(list.Delivery, "Delivery");
    strcpy_s(list.LookingFor, "LookingFor");
    char imageData[] = "Your image data here";
    int imageDataSize = strlen(imageData);
    list.ImageStructArray = new char[imageDataSize + 1];
    strcpy_s(list.ImageStructArray, imageDataSize + 1, imageData);



    SetBody(Pkt, 'L', reinterpret_cast<char*>(&list), sizeof(list));

    return Pkt;
}


Packet* dummyPacketPostInfoImage(Listing& list) {
    Packet* Pkt = new Packet();

    Pkt->GetBody()->Data = nullptr;


    memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
    memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
    memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


    SetHeaderInformation(Pkt, "127.0.0.1", strlen("127.0.0.1"), "127.0.0.1", strlen("127.0.0.1"), "POST", strlen("POST"), true, 0);
    strcpy_s(list.Title, "Title");
    strcpy_s(list.Location, "Location");
    strcpy_s(list.Condition, "Condition");
    strcpy_s(list.EstimatedWorth, "EstimatedWorth");
    strcpy_s(list.Delivery, "Delivery");
    strcpy_s(list.LookingFor, "LookingFor");
    char imageData[] = "eruru";
    int imageDataSize = strlen(imageData);
    list.ImageStructArray = new char[imageDataSize + 1];
    strcpy_s(list.ImageStructArray, imageDataSize + 1, imageData);



    SetBody(Pkt, 'L', reinterpret_cast<char*>(&list), sizeof(list));

    return Pkt;
}

Packet* dummyPacketPostNoInfo(Listing& list) {
    Packet* Pkt = new Packet();

    Pkt->GetBody()->Data = nullptr;


    memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
    memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
    memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


    SetHeaderInformation(Pkt, "", strlen(""), "", strlen(""), "", strlen(""), true, 0);
    strcpy_s(list.Title, "");
    strcpy_s(list.Location, "");
    strcpy_s(list.Condition, "");
    strcpy_s(list.EstimatedWorth, "");
    strcpy_s(list.Delivery, "");
    strcpy_s(list.LookingFor, "");
    char imageData[] = "";
    int imageDataSize = strlen(imageData);
    list.ImageStructArray = new char[imageDataSize + 1];
    strcpy_s(list.ImageStructArray, imageDataSize + 1, imageData);



    SetBody(Pkt, 'L', reinterpret_cast<char*>(&list), sizeof(list));

    return Pkt;
}


Packet* dummyPacketSignUp(SignUp& signup)
{
    Packet* Pkt = new Packet();

    Pkt->GetBody()->Data = nullptr;


    memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
    memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
    memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


    SetHeaderInformation(Pkt, "Source", strlen("Source"), "Destination", strlen("Destination"), "POST", strlen("POST"), true, 0);


    strcpy_s(signup.username, "Username");
    strcpy_s(signup.password, "Password");
    strcpy_s(signup.email, "Email");
    char imageData[] = "Your image data here";
    int imageDataSize = strlen(imageData);
    signup.ImageStructArray = new char[imageDataSize + 1];
    strcpy_s(signup.ImageStructArray, imageDataSize + 1, imageData);


    SetBody(Pkt, 'S', reinterpret_cast<char*>(&signup), sizeof(signup));

    return Pkt;
}

Packet* dummyPacketSignUpNoImage(SignUp& signup)
{
    Packet* Pkt = new Packet();

    Pkt->GetBody()->Data = nullptr;


    memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
    memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
    memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


    SetHeaderInformation(Pkt, "Source", strlen("Source"), "Destination", strlen("Destination"), "POST", strlen("POST"), true, 0);


    strcpy_s(signup.username, "Username");
    strcpy_s(signup.password, "Password");
    strcpy_s(signup.email, "Email");
    char imageData[] = "";
    int imageDataSize = strlen(imageData);
    signup.ImageStructArray = new char[imageDataSize + 1];
    strcpy_s(signup.ImageStructArray, imageDataSize + 1, imageData);


    SetBody(Pkt, 'S', reinterpret_cast<char*>(&signup), sizeof(signup));

    return Pkt;
}


TEST(TESTSVR02, ExecuteQueryTest) {
    //Arrange
    std::string dbPath = "databasetestser11.db";
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

    //Act
    bool query_exe_result = sqldb.executeQuery(sqlCreateTableListing);


    //Assert
    EXPECT_TRUE(query_exe_result);


    sqlite3_stmt* stmt = nullptr;
    sqldb.closeDatabase(&stmt);
}


TEST(TESTSVR02, ExecuteQueryTest_Unsuccessful) {
    //Arrange
    std::string dbPath = "databasetestser21.db";
    SQLiteDatabase sqldb(dbPath);
    const char* sqlCreateTableListing = "TABLE ("
        "id INTEGER NOT NULL,"
        "title TEXT;";

    //Act
    bool query_exe_result = sqldb.executeQuery(sqlCreateTableListing);


    //Assert
    EXPECT_FALSE(query_exe_result);


    sqlite3_stmt* stmt = nullptr;
    sqldb.closeDatabase(&stmt);
}

TEST(TESTSVR09, ListingPostInsert) {
    //Arrange
    Listing list;
    Packet* Pkt = dummyPacketPostInfo(list);
    std::string dbPath = "databasetestser31.db";
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
    sqlite3_stmt* stmt = nullptr;


    //Act
    int ListingPostInsertionReturn = sqldb.ListingPostInsert(&stmt, Pkt, list);


    //Assert
    EXPECT_EQ(0, ListingPostInsertionReturn);


}

TEST(TESTSVR09, ListingPostInsert_Unsuccessful) {
    //Arrange
    Listing list;
    Packet* Pkt = dummyPacketPostInfo(list);;
    std::string dbPath = "databasetestser41.db";
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
    sqlite3_stmt* stmt = nullptr;


    //Act
    int ListingPostInsertionReturn1 = sqldb.ListingPostInsert(&stmt, Pkt, list);
    int ListingPostInsertionReturn2 = sqldb.ListingPostInsert(&stmt, Pkt, list);

    //Assert
    EXPECT_EQ(-1, ListingPostInsertionReturn2);


}


TEST(TESTSVR01, SignUpWithImageDataInsert) {
    //Arrange
    SignUp signup;
    Packet* packet = dummyPacketSignUp(signup);
    std::string dbPath = "databasetestser51.db";
    SQLiteDatabase sqldb(dbPath);
    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithProfile ("
        "id INTEGER NOT NULL,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "email TEXT PRIMARY KEY,"
        "profile_picture BLOB NOT NULL);";
    bool query_exe_result = sqldb.executeQuery(sqlCreateTable);
    sqlite3_stmt* stmt = nullptr;


    //Act
    int SignUpdataInsertionReturn = sqldb.SignUpWithImageDataInsert(&stmt, packet, signup);


    //Assert
    EXPECT_EQ(0, SignUpdataInsertionReturn);


}



TEST(TESTSVR01, SignUpWithImageDataInsert_Unsuccessful)
{
    //Arrange
    SignUp signup;
    Packet* packet = dummyPacketSignUp(signup);
    std::string dbPath = "databasetestser61.db";
    SQLiteDatabase sqldb(dbPath);
    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithProfile ("
        "id INTEGER NOT NULL,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "email TEXT PRIMARY KEY,"
        "profile_picture BLOB NOT NULL);";
    bool query_exe_result = sqldb.executeQuery(sqlCreateTable);
    sqlite3_stmt* stmt = nullptr;
    sqldb.SignUpWithImageDataInsert(&stmt, packet, signup);

    //Act
    char* expectedErrorMessage = "Failed to execute statement: UNIQUE constraint failed: UsersWithProfile.email\n";
    std::string actualErrorMessage = capture_stderr([&] {
        sqldb.SignUpWithImageDataInsert(&stmt, packet, signup);
        });

    EXPECT_EQ(actualErrorMessage, expectedErrorMessage);
}

TEST(TESTSVR03, SignUpWithoutImageDataInsert) {
    //Arrange
    SignUp signup;
    Packet* packet = dummyPacketSignUp(signup);
    std::string dbPath = "databasetestser71.db";
    SQLiteDatabase sqldb(dbPath);
    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithoutProfile ("
        "id INTEGER NOT NULL,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "email TEXT PRIMARY KEY,"
        "profile_picture_SubstituteData TEXT NOT NULL);";

    bool query_exe_result = sqldb.executeQuery(sqlCreateTable);
    sqlite3_stmt* stmt = nullptr;


    //Act
    int SignUpdataInsertionReturn = sqldb.SignUpWithoutImageDataInsert(&stmt, packet, signup);


    //Assert
    EXPECT_EQ(0, SignUpdataInsertionReturn);


}


TEST(TESTSVR03, SignUpWithoutImageDataInsert_Unsuccessful) {
    //Arrange
    SignUp signup;
    Packet* packet = dummyPacketSignUp(signup);
    std::string dbPath = "databasetestser181.db";
    SQLiteDatabase sqldb(dbPath);
    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithoutProfile ("
        "id INTEGER NOT NULL,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "email TEXT PRIMARY KEY,"
        "profile_picture_SubstituteData TEXT NOT NULL);";

    bool query_exe_result = sqldb.executeQuery(sqlCreateTable);
    sqlite3_stmt* stmt = nullptr;


    //Act
    int SignUpdataInsertionReturn1 = sqldb.SignUpWithoutImageDataInsert(&stmt, packet, signup);


    //Assert
    char* expectedErrorMessage = "Failed to execute statement: UNIQUE constraint failed: UsersWithProfile.email\n";
    std::string actualErrorMessage = capture_stderr([&] {
        sqldb.SignUpWithoutImageDataInsert(&stmt, packet, signup);
        });

    EXPECT_TRUE(actualErrorMessage != expectedErrorMessage);

}




/// <summary>
/// ////////////////////////////////////////////////////////////
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST(TESTSVR30, FetchImage_Successful) {
    //Arrange
    SignUp signup;

    Packet* Pkt = dummyPacketSignUp(signup);

    std::string dbPath = "databasetestser91.db";

    SQLiteDatabase sqldb(dbPath);

    // SQL command to create table
    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithProfile ("
        "id INTEGER NOT NULL,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "email TEXT PRIMARY KEY,"
        "profile_picture BLOB NOT NULL);";


    bool query_exe_result = sqldb.executeQuery(sqlCreateTable);



    sqlite3_stmt* stmt = nullptr;

    int SignUpdataInsertionReturn = sqldb.SignUpWithImageDataInsert(&stmt, Pkt, signup);



    // Retrieve BLOB data
    char* imageArray = nullptr;
    int imageSize = 0;

    int fetch = sqldb.FetchImage(&stmt, signup.email, &imageArray, imageSize);



   
    EXPECT_EQ(0, fetch);
}

TEST(TESTSVR30, FetchImage_UnSuccessful) {
    //Arrange
    SignUp signup;

    Packet* Pkt = CreatePacket();

    std::string dbPath = "databasetestser101.db";

    SQLiteDatabase sqldb(dbPath);

    sqlite3_stmt* stmt = nullptr;

    // Retrieve BLOB data
    char* imageArray = nullptr;
    int imageSize = 0;


    //Act
    int fetch = sqldb.FetchImage(&stmt, signup.email, &imageArray, imageSize);



    //Assert
    EXPECT_EQ(-1, fetch);
}



TEST(TESTSVR30, getDB)
{
    //Arrange			
    std::string dbPath = "databasetestser111.db";
    SQLiteDatabase sqldb(dbPath);
    sqlite3* sq = nullptr;


    //Act
    sq = sqldb.getDB();


    //Assert
    EXPECT_TRUE(sq != nullptr);
}


TEST(TESTSVR29, isOpen)
{
    //Arrange			
    std::string dbPath = "databasetestser121.db";
    SQLiteDatabase sqldb(dbPath);
    //sqlite3* sq = nullptr;


    //Act
    bool isopen = sqldb.isOpen();


    //Assert
    EXPECT_TRUE(isopen);
}





TEST(TESTSVR20, closeDB_Unsuccessful)
{
    //Arrange			
    std::string dbPath = "databasetestser141.db";
    SQLiteDatabase sqldb(dbPath);
    sqlite3_stmt* stmt = nullptr;

    sqldb.closeDatabase(&stmt);
    //Act
    char* expectedErrorMessage = "Error executing SQL statement: bad parameter or other API misuse\n";
    std::string actualErrorMessage = capture_stderr([&] {
        sqldb.closeDatabase(&stmt);
        });


    //Assert
    EXPECT_EQ(expectedErrorMessage, actualErrorMessage);
}




TEST(TESTSVR36, SQLiteDatabase_Unsuccessful)
{
    //Arrange			
    std::string dbPath = "";
    SQLiteDatabase sqldb(dbPath);

    //Act
    char* expectedErrorMessage = "";
    std::string actualErrorMessage = capture_stderr([&] {
        SQLiteDatabase sqldb(dbPath);
        });


    //Assert
    EXPECT_EQ(expectedErrorMessage, actualErrorMessage);
}




int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
