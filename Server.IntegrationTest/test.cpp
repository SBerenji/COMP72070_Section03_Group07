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

std::string capture_stderr(std::function<void()> func) {
    // Save the current stderr buffer
    std::streambuf* oldStderr = std::cerr.rdbuf();

    // Create a stringstream to redirect stderr
    std::stringstream capturedStderr;
    std::cerr.rdbuf(capturedStderr.rdbuf());

    // Execute the provided function
    func();

    // Restore the original stderr buffer
    std::cerr.rdbuf(oldStderr);

    // Return the captured stderr content as a string
    return capturedStderr.str();
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

TEST(IntegrationTest1, ExecuteQueryTest) {
    //Arrange
    std::string dbPath = "database1.db";
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


TEST(IntegrationTest1, ExecuteQueryTest_NotWork) {
    //Arrange
    std::string dbPath = "database1.db";
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

TEST(IntegrationTest2, ListingPostInsert) {
    //Arrange
    Listing list;
    Packet* Pkt = dummyPacketPostInfo(list);
    std::string dbPath = "database135.db";
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

TEST(IntegrationTest2, ListingPostInsert_failed) {
    //Arrange
    Listing list;
    Packet* Pkt = dummyPacketPostInfo(list);;
    std::string dbPath = "database101.db";
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


TEST(IntegrationTest3, SignUpWithImageDataInsert) {
    //Arrange
    SignUp signup;
    Packet* packet = dummyPacketSignUp(signup);
    std::string dbPath = "database333.db";
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


/// <summary>
/// ///////////////////////////////////fix this
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST(IntegrationTest3, SignUpWithImageDataInsert_failed__345___) 
{
    //Arrange
    SignUp signup;
    Packet* packet = dummyPacketSignUp(signup);
    std::string dbPath = "database12345w2r4srg54.db";
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
    char* expectedErrorMessage = "Failed to execute statement: UNIQUE constraint failed: UsersWithProfile.email\n";
    std::string actualErrorMessage = capture_stderr([&] {
        sqldb.SignUpWithImageDataInsert(&stmt, packet, signup);
        });

    EXPECT_TRUE(actualErrorMessage.find(expectedErrorMessage) != std::string::npos);


   
}

TEST(IntegrationTest4, SignUpWithoutImageDataInsert) {
    //Arrange
    SignUp signup;
    Packet* packet = dummyPacketSignUp(signup);
    std::string dbPath = "database444.db";
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


TEST(IntegrationTest4, SignUpWithoutImageDataInsert_failed) {
    //Arrange
    SignUp signup;
    Packet* packet = dummyPacketSignUp(signup);
    std::string dbPath = "database444.db";
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

    EXPECT_FALSE(actualErrorMessage.find(expectedErrorMessage) != std::string::npos);

}


TEST(IntegrationTest5, FetchImage) {
    //Arrange
    Listing list;

    Packet* Pkt = dummyPacketPostInfo(list);

    std::string dbPath = "database556643456.db";
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

    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithProfile ("
        "id INTEGER NOT NULL,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "email TEXT PRIMARY KEY,"
        "profile_picture BLOB NOT NULL);";


    bool query_exe_result2 = sqldb.executeQuery(sqlCreateTable);

    sqlite3_stmt* stmt = nullptr;

    int ListingPostInsertionReturn = sqldb.ListingPostInsert(&stmt, Pkt, list);



    char* ListingImageArray = nullptr;
    int listingImageSize = 0;
    int fetch = sqldb.FetchImage(&stmt, list.Title, &ListingImageArray, listingImageSize);

    EXPECT_EQ(0, fetch);
}


/// <summary>
/// ////////////////////////////////////////////////////////////
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST(IntegrationTest5, FetchImage_notfound) {
    //Arrange
    Listing list;

    Packet* Pkt = dummyPacketPostInfo(list);

    std::string dbPath = "database556643456.db";
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

    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS UsersWithProfile ("
        "id INTEGER NOT NULL,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL,"
        "email TEXT PRIMARY KEY,"
        "profile_picture BLOB NOT NULL);";


    bool query_exe_result2 = sqldb.executeQuery(sqlCreateTable);

    sqlite3_stmt* stmt = nullptr;
    
    int ListingPostInsertionReturn = sqldb.ListingPostInsert(&stmt, Pkt, list);
    
   

    char* ListingImageArray = nullptr;
    int listingImageSize = 0;
    int fetch = sqldb.FetchImage(&stmt, list.Title, &ListingImageArray, listingImageSize);
   
    EXPECT_EQ(-1, fetch);
}



TEST(IntegrationTest5, getDB)
{
    //Arrange			
    std::string dbPath = "database444.db";
    SQLiteDatabase sqldb(dbPath);
    sqlite3* sq = nullptr;


    //Act
    sq = sqldb.getDB();


    //Assert
    EXPECT_TRUE(sq != nullptr);
}


TEST(IntegrationTest6, isOpen)
{
    //Arrange			
    std::string dbPath = "database444.db";
    SQLiteDatabase sqldb(dbPath);
    //sqlite3* sq = nullptr;


    //Act
    bool isopen = sqldb.isOpen();


    //Assert
    EXPECT_TRUE(isopen);
}


TEST(IntegrationTest7, isOpen_False)
{
    //Arrange			
    std::string dbPath = "database444.db";
    SQLiteDatabase sqldb(dbPath);
    sqlite3_stmt* stmt = nullptr;


    //Act
    sqldb.closeDatabase(&stmt);
    bool isopen = sqldb.isOpen();


    //Assert
    EXPECT_FALSE(isopen);
}


//TEST_METHOD(TestMethod37_SQLiteDatabase_SignUpWithoutImageDataInsert)
//    {
//    	//Arrange 
//    	Packet* packet = dummyPacket();
//    	SignUp signup = dummySignin();
//    	//std::string dbPath = "isOpenDB.db";
//    	SQLiteDatabase sqldb(":memory:");
//    	sqlite3_stmt* stmt = nullptr;
//    	int SignUpdataInsertionReturn = sqldb.SignUpWithImageDataInsert(&stmt, packet, signup);
//    	char* imageArray = nullptr;
//    	int imageSize = 0;
//
//
//    	//Act
//    	int actual = sqldb.FetchImage(&stmt, signup.email, &imageArray, imageSize);
//
//
//    	//Assert
//    	Assert::AreEqual(0, actual);
//    }




int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
