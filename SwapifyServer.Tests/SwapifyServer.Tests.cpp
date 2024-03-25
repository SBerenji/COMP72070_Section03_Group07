#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include "CppUnitTest.h"

#include "../SwapifyServer/file_utils.h"
#include "../SwapifyServer/Packet.h"
#include "../SwapifyServer/SQLiteDatabase.h"
#include "../SwapifyServer/User.h"
#include "../SwapifyServer/UsersRoute.h"
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


Packet* dummyPacket() //creates a filled packet to be tested
{
	Packet* actual = CreatePacket(); //creates a new packet


	strcpy_s(actual->GetHead()->Source, "Client"); //sets source
	strcpy_s(actual->GetHead()->Destination, "Server"); //sets destination
	strcpy_s(actual->GetHead()->Route, "Route1"); //sets route
	actual->GetHead()->Authorization = true; //sets authorization
	actual->GetHead()->Length = 12; //sets legnth
	int DataSize = 5; 
	char* Data = "data"; 
	actual->GetBody()->User = 2; //sets user
	actual->GetBody()->Data = new char[DataSize]; //assigns memory for data
	actual->GetBody()->Data[DataSize - 1] = '\0'; //sets last item in data to null
	memcpy(actual->GetBody()->Data, Data, DataSize); //sets data 
	actual->GetTail()->Checksum = 2; //sets checksum
	
	return actual;
}


Packet* dummyPacketLogIn() //creates a filled packet to be tested
{
	Packet* actual = CreatePacket(); //creates a new packet


	strcpy_s(actual->GetHead()->Source, "Client"); //sets source
	strcpy_s(actual->GetHead()->Destination, "Server"); //sets destination
	strcpy_s(actual->GetHead()->Route, "LOGIN"); //sets route
	actual->GetHead()->Authorization = true; //sets authorization
	actual->GetHead()->Length = 12; //sets legnth
	int DataSize = 5;
	char* Data = "username..password..";
	actual->GetBody()->User = 2; //sets user
	actual->GetBody()->Data = new char[DataSize]; //assigns memory for data
	actual->GetBody()->Data[DataSize - 1] = '\0'; //sets last item in data to null
	memcpy(actual->GetBody()->Data, Data, DataSize); //sets data 
	actual->GetTail()->Checksum = 2; //sets checksum

	return actual;
}


Packet* dummyPacketSignUp() //creates a filled packet to be tested
{
	Packet* actual = CreatePacket(); //creates a new packet


	strcpy_s(actual->GetHead()->Source, "Client"); //sets source
	strcpy_s(actual->GetHead()->Destination, "Server"); //sets destination
	strcpy_s(actual->GetHead()->Route, "SIGNUP_IMAGEUPLOADED"); //sets route
	actual->GetHead()->Authorization = true; //sets authorization
	actual->GetHead()->Length = 12; //sets legnth
	int DataSize = 5;
	char* Data = "username..password..email...................................1";
	actual->GetBody()->User = 2; //sets user
	actual->GetBody()->Data = new char[DataSize]; //assigns memory for data
	actual->GetBody()->Data[DataSize - 1] = '\0'; //sets last item in data to null
	memcpy(actual->GetBody()->Data, Data, DataSize); //sets data 
	actual->GetTail()->Checksum = 2; //sets checksum

	return actual;
}



Packet* dummyPacketSignUpUserCheck() //creates a filled packet to be tested
{
	Packet* actual = CreatePacket(); //creates a new packet


	strcpy_s(actual->GetHead()->Source, "Client"); //sets source
	strcpy_s(actual->GetHead()->Destination, "Server"); //sets destination
	strcpy_s(actual->GetHead()->Route, "SIGNUP_USERCHECK"); //sets route
	actual->GetHead()->Authorization = true; //sets authorization
	actual->GetHead()->Length = 12; //sets legnth
	int DataSize = 5;
	char* Data = "username..email...................................";
	actual->GetBody()->User = 2; //sets user
	actual->GetBody()->Data = new char[DataSize]; //assigns memory for data
	actual->GetBody()->Data[DataSize - 1] = '\0'; //sets last item in data to null
	memcpy(actual->GetBody()->Data, Data, DataSize); //sets data 
	actual->GetTail()->Checksum = 2; //sets checksum

	return actual;
}


Listing dummyListing()
{
	Listing list;
	strcpy_s(list.Title, "Title");
	strcpy_s(list.Location, "Location");
	strcpy_s(list.Condition, "Condition");
	strcpy_s(list.EstimatedWorth, "EstimatedWorth");
	strcpy_s(list.Delivery, "Delivery");
	strcpy_s(list.LookingFor, "LookingFor");
	int imageSize = 20;

	return list;
}

Packet* dummyPacketPost() //creates a filled packet to be tested
{
	Packet* actual = CreatePacket(); //creates a new packet


	strcpy_s(actual->GetHead()->Source, "Client"); //sets source
	strcpy_s(actual->GetHead()->Destination, "Server"); //sets destination
	strcpy_s(actual->GetHead()->Route, "POST"); //sets route
	actual->GetHead()->Authorization = true; //sets authorization
	actual->GetHead()->Length = 12; //sets legnth
	int imageSize = 20;
	Listing list = dummyListing();
	int DataSize = sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor) + imageSize;
	char* Data = new char[DataSize];
	memset(Data, 0, (sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor)) + imageSize);
	memcpy(actual->GetBody()->Data, Data, DataSize); //sets data 
	actual->GetTail()->Checksum = 2; //sets checksum

	return actual;
}



Packet* dummyPacketNoBody() //creates a filled packet to be tested
{
	Packet* actual = CreatePacket(); //creates a new packet


	strcpy_s(actual->GetHead()->Source, "Client"); //sets source
	strcpy_s(actual->GetHead()->Destination, "Server"); //sets destination
	strcpy_s(actual->GetHead()->Route, "Route1"); //sets route
	actual->GetHead()->Authorization = true; //sets authorization
	actual->GetHead()->Length = 0; //sets legnth
	int DataSize = 0;
	char* Data = "";
	actual->GetBody()->User = 2; //sets user
	actual->GetBody()->Data = new char[DataSize]; //assigns memory for data
	memcpy(actual->GetBody()->Data, Data, DataSize); //sets data 
	actual->GetTail()->Checksum = 2; //sets checksum

	return actual;
}

//Packet* dummyPacketSignUp() //creates dummy signup packet
//{
//	Packet* actual = CreatePacket();
//
//
//	strcpy_s(actual->GetHead()->Source, "Client"); //sets source
//	strcpy_s(actual->GetHead()->Destination, "Server"); //sets destination
//	strcpy_s(actual->GetHead()->Route, "SIGNUP"); //set route to sign up 
//	actual->GetHead()->Authorization = true; //sets authorization
//	actual->GetHead()->Length = 12; //sets length
//	int DataSize = 5; 
//	char* Data = "data"; 
//	actual->GetBody()->User = 2; //sets user 
//	actual->GetBody()->Data = new char[DataSize]; //assigns memory for user
//	actual->GetBody()->Data[DataSize - 1] = '\0'; //sets lat item in data to null
//	memcpy(actual->GetBody()->Data, Data, DataSize); //sets data
//	actual->GetTail()->Checksum = 2; //sets checksum
//
//	return actual;
//}

LogIn dummyLogin() //dummy log in function
{
	LogIn log;
	strcpy_s(log.username, "username"); //sets username
	strcpy_s(log.password, "password"); //sets password
	return log;
}

SignUp dummySignin() //dummy sign in function
{
	SignUp signin;
	strcpy_s(signin.username, "username"); //sets username
	strcpy_s(signin.password, "password"); //sets password
	strcpy_s(signin.email, "email"); //sets email
	char* imagestructarray = "isa"; 
	signin.ImageStructArray = new char[sizeof(imagestructarray)];
	memcpy(signin.ImageStructArray, imagestructarray, 3); //sets imagestruct array

	return signin;
}

class MockClientSocket {
public:
	MockClientSocket() {}
};

//std::vector<char*> readFromFile(const std::string& filename) {
//	std::vector<char*> result; // Store the converted values
//	std::string inputStr = "";
//
//	std::ifstream file(filename);
//	if (file.is_open()) {
//		while (std::getline(file, inputStr)) {
//			std::istringstream issLine(inputStr);
//			std::string title;
//			std::getline(issLine, title, ':');
//
//			std::string value;
//			std::getline(issLine, value);
//
//			// Allocate memory for the C-style string
//			char* newValue = new char[value.size() + 1];
//			strncpy(newValue, value.c_str(), value.size() + 1);
//
//			// Store the result
//			result.push_back(newValue);
//		}
//		file.close();
//	}
//	else {
//		std::cerr << "Error opening file: " << filename << std::endl;
//	}
//
//	return result;
//}


class MockSQLiteDatabase : public SQLiteDatabase {
public:
	MockSQLiteDatabase(const std::string & dbPath) : SQLiteDatabase(dbPath) {}

	bool executeQuery(const char* sqlQuery) {
		return true;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////////


extern "C" int CloseSocket(SOCKET ClientSocket, closesocketFunc closeFunc = closesocket, WSACleanupFunc wsacleanupfunc = WSACleanup);
extern "C" int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc = send);
extern "C" SOCKET setupConnection2(WSAStartupFunc wsaStartup = WSAStartup, socketFunc socketfunc = socket, connectFunc connectfunc = connect, WSACleanupFunc wsacleanup = WSACleanup);


//  *****  Creating mock functions for the sendData() function   ******

// The actual send function returns the total size sent upon successful sending
int MockSendSuccess(SOCKET ClientSocket, const char* TxBuffer, int totalSize, int flag) {
	return totalSize; // Return the number of bytes sent
}

// This is the Mock function for sendData in order to simulate UNsucessful sending
// The actual send function returns -1 upon UNsuccessful sending
int MockSendFailure(SOCKET ClientSocket, const char* TxBuffer, int totalSize, int flag) {
	return -1;
}






//  *****  Creating mock functions for the setupConnection2() function   ******

// WSAStartup success:
int MockWSAStartupSuccess(WORD wVersionRequested, LPWSADATA lpWSAData) {
	return 0; // simulate success
}


// WSAStartup failure:
int MockWSAStartupFailure(WORD wVersionRequested, LPWSADATA lpWSAData) {
	return 1; // simulate failure
}



// Socket success:
SOCKET MockSocketSuccess(int, int, int)
{
	return 1;  //simulate success (anything but -1)
}


// Socket failure:
SOCKET MockSocketFailure(int, int, int)
{
	return INVALID_SOCKET;  // Simulate failure (must return INVALID_SOCKET)
}


// connect success:
int MockConnectSuccess(SOCKET, const struct sockaddr*, int)
{
	return 1; //simulate success (anything but -1)
}


// connect failure:
int MockConnectFailure(SOCKET, const struct sockaddr*, int)
{
	return SOCKET_ERROR; //simulate failure (must be -1)
}


//****    Mock funciton for the CloseSocket function    *****
int MockCloseSocketSuccess(SOCKET)
{
	return 0;   // This function will return 0 upon success
}


int MockCloseSocketFailure(SOCKET)
{
	return SOCKET_ERROR;   // This function will return 0 upon success
}


int MockWSACleanupSuccess()
{
	return 0;   // This function will return 0 upon success
}


int MockWSACleanupFailure()
{
	return SOCKET_ERROR;   // This function will return SOCKET_ERROR upon failure
}



/////////////////////////////////////////////////////////////////////////////////////////////




namespace SwapifyServerTests
{
	TEST_CLASS(SwapifyServerTests)
	{
	public:


		////Sets up a test file to be tested with a itemType, linedetails and TEXT_FILE
		//void fileSetup(const string TEXT_FILE) {
		//	std::ofstream ofs(TEXT_FILE);
		//	ofs << "Source:source" << std::endl;
		//	ofs << "Destination:destination" << std::endl;
		//	ofs << "Route:route" << std::endl;
		//	ofs << "Authorization:1" << std::endl;
		//	ofs << "Length:12" << std::endl;
		//	ofs << "User:user" << std::endl;
		//	ofs << "Listing:u" << std::endl;
		//	ofs << "Message:u" << std::endl;
		//	ofs << "Checksum:7b" << std::endl;
		//	ofs.close();
		//}



		//TEST_METHOD(TestMethod3_logPacketLine_Same)
		//{
		//	string TEXT_FILE = "TempFil.txt";
		//	fileSetup(TEXT_FILE);  //create file with expected data
		//	vector<char*> expectedDatafromFile = readFromFile("TempFil.txt"); //read data to vector

		//	char user[] = "user";

		//	Packet p; //fill the packet with expected information

		//	p.setSource("source");
		//	p.setDestination("destination");
		//	p.setRoute("route");
		//	p.setAuthorization(true);
		//	p.setLength(12);
		//	p.setUser(user);
		//	p.setListing(user);
		//	p.setMessage(user);
		//	p.setTail(123);

		//	RequestLogger log("testFil.txt");
		//	log.logPacket(p); //write to log file
		//	vector<char*> actualdatafromFile = readFromFile("testFil.txt"); //read from log file into actual results vector

		//	for (size_t i = 0; i < expectedDatafromFile.size(); ++i) {
		//		Assert::AreEqual(0, strcmp(expectedDatafromFile[i], actualdatafromFile[i])); //assert to check that each char is identical
		//	}

		//}

		TEST_METHOD(TestMethod2_TESTSVR015_Server_Packet_GetHead) //tests the GetHead() in the Packet.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			string expectedSource = "Client";
			string expectedDestination = "Server";
			string expectedRoute = "Route1";
			bool expectedAuthorization = true;
			unsigned int expectedLength = 12;


			//Act
			string actualSource = actual->GetHead()->Source;
			string actualDestination = actual->GetHead()->Destination;
			string actualRoute = actual->GetHead()->Route;
			bool actualAuthorization = actual->GetHead()->Authorization;
			unsigned int actualLength = actual->GetHead()->Length;


			//Assert 
			Assert::AreEqual(expectedSource, actualSource);
			Assert::AreEqual(expectedDestination, actualDestination);
			Assert::AreEqual(expectedRoute, actualRoute);
			Assert::AreEqual(expectedAuthorization, actualAuthorization);
			Assert::AreEqual(expectedLength, actualLength);

			DestroyPacket(actual);
		}

		TEST_METHOD(TestMethod3_TESTSVR16_Server_Packet_GetBody) //tests the GetBody function in Packet.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			int DataSize = 5;
			char* expectedData = "data";
			unsigned char expectedUser = 2;


			//Act
			char* actualData = actual->GetBody()->Data;
			unsigned char actualUser = actual->GetBody()->User;


			//Assert
			Assert::AreEqual(expectedUser, actualUser);
			for (int i = 0; i < DataSize; ++i) {
				Assert::AreEqual(expectedData[i], actualData[i]);
			}

			DestroyPacket(actual);
		}



		TEST_METHOD(TestMethod4_TESTSVR017_Server_Packet_GetTail) //tests the GetTail function in Packet.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			unsigned int expectedChecksum = 2;


			//Act
			unsigned int actualChecksum = actual->GetTail()->Checksum;


			//Assert
			Assert::AreEqual(expectedChecksum, actualChecksum);

			DestroyPacket(actual);
		}


		TEST_METHOD(TestMethod5_TESTSVR18_Server_Packet_CreatePacket) //tests the CreatePacket function in Packet.h
		{
			//Arrange
			

			//Act
			Packet* packet = CreatePacket();
			

			//Assert
			Assert::IsTrue(packet);

			DestroyPacket(packet);
		}

		TEST_METHOD(TestMethod6_TESTSVR19_Server_Packet_DestroyPacket) //tests the DestroyPacket function in Packet.h
		{
			//Arrange
			Packet* packet = dummyPacket();
			unsigned int expected = 2;


			//Act
			DestroyPacket(packet);
			unsigned int actual = packet->GetHead()->Length;

			//Assert
			Assert::AreNotEqual(expected, actual);

		}

		TEST_METHOD(TestMethod7_TESTSVR20_Server_Packet_DisplayFunction) //tests the Display function in PAcket.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			LogIn login = dummyLogin();
			SignUp signin = dummySignin();
			Listing list;
			std::stringstream actualOutput;
			string expectedOutput = "Source:  Client\n"
				"Destination: Server\n"
				"Route:  Route1\n"
				"Authorization:  1\n"
				"Length:  12\n"
				"User:  \n" 
				"Data:  data\n";


			//Act
			Display(actual, actualOutput, login, signin, list);


			//Assert
			Assert::AreEqual(expectedOutput, actualOutput.str());

			DestroyPacket(actual);

		}




		TEST_METHOD(TestMethod8_TESTSVR05_Server_Packet_Deserializefunction) //tests the deserialize function in Packet.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			LogIn login;
			SignUp signin;
			SignUpCheck check;
			Listing list;
			int size = 0;
			char* serializedBuffer = SerializeData(actual, size);
			Packet* deserializedPacket = CreatePacket();
			
			string expectedSource = "Client";
			string expectedDestination = "Server";
			string expectedRoute = "Route1";
			bool expectedAuthorization = true;
			unsigned int expectedLength = 12;
			unsigned char expectedUser = 4278255360;
			unsigned int expectedChecksum = 2;


			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin, check, list);
			string actualSource = deserializedPacket->GetHead()->Source;
			string actualDestination = deserializedPacket->GetHead()->Destination;
			string actualRoute = deserializedPacket->GetHead()->Route;
			bool actualAuthorization = deserializedPacket->GetHead()->Authorization;
			unsigned int actualLength = deserializedPacket->GetHead()->Length;
			unsigned char actualUser = deserializedPacket->GetBody()->User;
			unsigned int actualChecksum = deserializedPacket->GetTail()->Checksum;



			//Assert
			Assert::AreEqual(actualSource, expectedSource);
			Assert::AreEqual(actualDestination, expectedDestination);
			Assert::AreEqual(actualRoute, expectedRoute);
			Assert::AreEqual(actualAuthorization, expectedAuthorization);
			Assert::AreEqual(actualLength, expectedLength);
			Assert::AreEqual(actualUser, expectedUser);
			Assert::AreEqual(0, strcmp(actual->GetBody()->Data, deserializedPacket->GetBody()->Data));
			Assert::AreEqual(actualChecksum, expectedChecksum);

			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}

		TEST_METHOD(TestMethod88_TESTSVR05_Server_Packet_Deserializefunction) //tests the deserialize function in Packet.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			LogIn login = dummyLogin();
			SignUp signin = dummySignin();
			int size = 0;
			char* serializedBuffer = SerializeData(actual, size);
			Packet* deserializedPacket = CreatePacket();

			string expectedSource = "Client";
			string expectedDestination = "Server";
			string expectedRoute = "Route1";
			bool expectedAuthorization = true;
			unsigned int expectedLength = 12;
			

			//Act
			Deserialization(deserializedPacket, serializedBuffer);
			string actualSource = deserializedPacket->GetHead()->Source;
			string actualDestination = deserializedPacket->GetHead()->Destination;
			string actualRoute = deserializedPacket->GetHead()->Route;
			bool actualAuthorization = deserializedPacket->GetHead()->Authorization;
			unsigned int actualLength = deserializedPacket->GetHead()->Length;


			//Assert
			Assert::AreEqual(actualSource, expectedSource);
			Assert::AreEqual(actualDestination, expectedDestination);
			Assert::AreEqual(actualRoute, expectedRoute);
			Assert::AreEqual(actualAuthorization, expectedAuthorization);
			Assert::AreEqual(actualLength, expectedLength);
			
			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}



		TEST_METHOD(TestMethod9_TESTSVR21_Server_Packet_SetHeaderfunction) //testing the SetHeader function in Packet.h
		{
			//Arrange
			Packet* actual = CreatePacket();
			Packet* tempHeader = dummyPacket();
			string expectedSource = "Client";
			string expectedDestination = "Server";
			string expectedRoute = "Route1";
			bool expectedAuthorization = true;
			unsigned int expectedLength = 12;
			unsigned char expectedUser = 2;
			unsigned int expectedChecksum = 2;


			//Act
			SetHeader(actual, tempHeader->GetHead());
			string actualSource = tempHeader->GetHead()->Source;
			string actualDestination = tempHeader->GetHead()->Destination;
			string actualRoute = tempHeader->GetHead()->Route;
			bool actualAuthorization = tempHeader->GetHead()->Authorization;
			unsigned int actualLength = tempHeader->GetHead()->Length;


			//Assert
			Assert::AreEqual(actualSource, expectedSource);
			Assert::AreEqual(actualDestination, expectedDestination);
			Assert::AreEqual(actualRoute, expectedRoute);
			Assert::AreEqual(actualAuthorization, expectedAuthorization);
			Assert::AreEqual(actualLength, expectedLength);

			DestroyPacket(actual);
			DestroyPacket(tempHeader);

		}

		TEST_METHOD(TestMethod10_TESTSVR22_Server_Packet_SetBodyfunction) //testing the set body function in Packet.h
		{
			//Arrange
			Packet* actual = CreatePacket();
			unsigned char expectedUser = 5;
			char expectedData[] = "data";
			unsigned int expectedLength = strlen(expectedData);


			//Act
			SetBody(actual, expectedUser, expectedData, expectedLength);
			unsigned char actualUser = actual->GetBody()->User;
			unsigned int actualLength = actual->GetHead()->Length;

			//Assert
			Assert::AreEqual(actualUser, expectedUser);
			Assert::AreEqual(0, strcmp(actual->GetBody()->Data, expectedData));
			Assert::AreEqual(actualLength,expectedLength+1);

			DestroyPacket(actual);

		}

		TEST_METHOD(TestMethod11_TESTSVR04_Server_Packet_Serializefunction) //testing the Serialize function in Packet.h
		{
			//Arrange
			Packet* actual = dummyPacket();

			//Act
			int size = 0;
			char* serializedBuffer = nullptr;
			serializedBuffer = SerializeData(actual, size);

			//Assert
			Assert::IsNotNull(serializedBuffer);

			DestroyPacket(actual);


		}


		//TEST_METHOD(TestMethod12_TESTSVR23_Server_Listing_ParseDataFunction)
		//{
		//	std::string listingString = "1,Sample Title,Sample Description,Sample Category,99.99,Sample Condition,Sample Location,Sample Seller,Sample Status,image1.jpg,2024-03-12";

		//	// Parse the listing string
		//	Listing listing = listing.parseListing(listingString);

		//	// Verify that the parsed listing matches the expected values
		//	Assert::AreEqual(1, listing.getId());
		//	Assert::AreEqual("Sample Title", listing.getTitle().c_str());
		//	Assert::AreEqual("Sample Description", listing.getDescription().c_str());
		//	Assert::AreEqual("Sample Category", listing.getCategory().c_str());
		//	Assert::AreEqual(99.99, listing.getPrice());
		//	Assert::AreEqual("Sample Condition", listing.getCondition().c_str());
		//	Assert::AreEqual("Sample Location", listing.getLocation().c_str());
		//	Assert::AreEqual("Sample Seller", listing.getSeller().c_str());
		//	Assert::AreEqual("Sample Status", listing.getStatus().c_str());
		//	Assert::AreEqual("Sample Title", listing.getTitle().c_str());
		//	Assert::AreEqual("2024-03-12", listing.getListingDate().c_str());

		//}





		//TEST_METHOD(TestMethod13_TESTSVR11_Server_Packet_Packet_CheckSumDrop)  //test that the packet is dropped if checksum do not match
		//{
		//	//Arrange
		//	Packet* actual = dummyPacket();
		//	LogIn login = dummyLogin();
		//	SignUp signin = dummySignin();
		//	int size = 0;
		//	Packet* deserializedPacket = CreatePacket();
		//	char expectedSource = '\0';


		//	//Act
		//	char* serializedBuffer = SerializeData(actual, size);
		//	Deserialization(deserializedPacket, serializedBuffer, login, signin);
		//	char actualSource = deserializedPacket->GetHead()->Source[0];


		//	//Assert
		//	Assert::AreEqual(expectedSource, actualSource); 

		//	DestroyPacket(actual);
		//	DestroyPacket(deserializedPacket);

		//}

		TEST_METHOD(TestMethod14_TESTSVR12_Server_Serialize_NoBody)  //tests the Serialize function with no body
		{
			//Arrange
			Packet* actual = dummyPacketNoBody();
			int size = 0;
			

			//Act
			char* serializedBuffer = SerializeData(actual, size);


			//Assert
			Assert::AreNotEqual(serializedBuffer, nullptr); 

			DestroyPacket(actual);

		}

		TEST_METHOD(TestMethod15_TESTSVR13_Server_GetBody)  
		{
			Packet* actual = dummyPacket();

			char Data[] = "data";
	

			unsigned char user = 2;


			Assert::AreEqual(actual->GetBody()->User, user);
			Assert::AreEqual((string)actual->GetBody()->Data, (string)Data); 


			DestroyPacket(actual);

		}

		TEST_METHOD(TestMethod16_TESTSVR14_Server_GetTail) 
		{
			Packet* actual = dummyPacket();


			Assert::IsTrue(actual->GetTail()->Checksum == 2); 


			DestroyPacket(actual);

		}

		TEST_METHOD(TestMethod17_TESTSVR_Server_User_Parseuserinfo)  //test parse user function in User.h
		{
			//Arrange
			int id = 0;
			const std::string emailAddress = "test@example.com";
			const std::string password = "password";
			const std::string firstName = "John";
			const std::string lastName = "Doe";
			unsigned char profilepic = 2;
			const std::vector<unsigned char> profilePicture = { 2 };
			const std::string dateCreated = "2024-03-14";
			const std::string accountStatus = "active";
			const std::string userString = "test@example.com,password,John,Doe,image.jpg,2024-03-14,active";
			User expectedUser(id, emailAddress, password, firstName, lastName, profilePicture, dateCreated, accountStatus);
			User user;
			
			
			//Act
			User actual = user.parseUser(userString);


			//Assert
			Assert::AreEqual(actual.getEmailAddress(), expectedUser.getEmailAddress());
			Assert::AreEqual(actual.getPassword(), expectedUser.getPassword());
			Assert::AreEqual(actual.getFirstName(), expectedUser.getFirstName());
			Assert::AreEqual(actual.getLastName(), expectedUser.getLastName());
			Assert::AreEqual(actual.getProfilePicture(), expectedUser.getProfilePicture());
			Assert::AreEqual(actual.getDateCreated(), expectedUser.getDateCreated());
			Assert::AreEqual(actual.getAccountStatus(), expectedUser.getAccountStatus());
		}

		TEST_METHOD(TestMethod18_TESTSVR16_Server_User_ComparePasswords) //tests validatePassword function in User.h
		{
			//Arrange
			int id = 0;
			const std::string emailAddress = "test@example.com";
			const std::string password = "password";
			const std::string firstName = "John";
			const std::string lastName = "Doe";
			unsigned char profilepic = 2;
			const std::vector<unsigned char> profilePicture = { 2 };
			const std::string dateCreated = "2024-03-14";
			const std::string accountStatus = "active";
			string userString = "test@example.com,password,John,Doe,image.jpg,2024-03-14,active";
			User expectedUser(id, emailAddress, password, firstName, lastName, profilePicture, dateCreated, accountStatus);
			bool expected = true;


			//Act
			bool result = expectedUser.validatePassword(password, expectedUser.getPassword());

			
			//Assert
			Assert::AreEqual(expected, result);
		}

		TEST_METHOD(TestMethod19_TESTSVR17_Server_UserRoutes_HashFunction) //tests validate Hashfunction in User.h
		{
			//Arrange
			int id = 0;
			const std::string emailAddress = "test@example.com";
			const std::string password = "password";
			const std::string firstName = "John";
			const std::string lastName = "Doe";
			unsigned char profilepic = 2;
			const std::vector<unsigned char> profilePicture = { 2 };
			const std::string dateCreated = "2024-03-14";
			const std::string accountStatus = "active";
			string expected = "password";
			string userString = "test@example.com,password,John,Doe,image.jpg,2024-03-14,active";
			User expectedUser(id, emailAddress, password, firstName, lastName, profilePicture, dateCreated, accountStatus);


			//Act
			string actual = expectedUser.generateHash(password);

			
			//Assert
			Assert::AreEqual(expected, actual);

		}

		TEST_METHOD(TestMethod20__Server_UserRoutes_HandleLogin) //tests validate HandleLogin function in UsersRoutes
		{
			//Arrange
			UserRoutes userRoutes;
			int id = 0;
			const std::string emailAddress = "test@example.com";
			const std::string password = "password";
			const std::string firstName = "John";
			const std::string lastName = "Doe";
			unsigned char profilepic = 2;
			const std::vector<unsigned char> profilePicture = { 2 };
			const std::string dateCreated = "2024-03-14";
			const std::string accountStatus = "active";
			string userString = "test@example.com,password,John,Doe,image.jpg,2024-03-14,active";
			User expectedUser(id, emailAddress, password, firstName, lastName, profilePicture, dateCreated, accountStatus);


			//Act
			Packet* result = userRoutes.handleLogin(expectedUser);
			
			
			//Assert
			Assert::IsTrue(result == nullptr); 

		}

		TEST_METHOD(TestMethod21__Server_HandleRegister) //tests handleRegister function is userRoutes
		{
			//Arrange
			UserRoutes userRoutes;
			int id = 0;
			const std::string emailAddress = "test@example.com";
			const std::string password = "password";
			const std::string firstName = "John";
			const std::string lastName = "Doe";
			unsigned char profilepic = 2;
			const std::vector<unsigned char> profilePicture = { 2 };
			const std::string dateCreated = "2024-03-14";
			const std::string accountStatus = "active";
			string userString = "test@example.com,password,John,Doe,image.jpg,2024-03-14,active";
			User expectedUser(id, emailAddress, password, firstName, lastName, profilePicture, dateCreated, accountStatus);


			//Act
			Packet* result = userRoutes.handleRegister(expectedUser);
			
			
			//Assert
			Assert::IsTrue(result != nullptr); 

		}

		TEST_METHOD(TestMethod21__Server_UserRoutes_HandleUpdateUser)  //tests handleUpdateUser function in UserRoutes
		{
			//Arrange
			UserRoutes userRoutes;
			int id = 0;
			const std::string emailAddress = "test@example.com";
			const std::string password = "password";
			const std::string firstName = "John";
			const std::string lastName = "Doe";
			unsigned char profilepic = 2;
			const std::vector<unsigned char> profilePicture = { 2 };
			const std::string dateCreated = "2024-03-14";
			const std::string accountStatus = "active";
			string userString = "test@example.com,password,John,Doe,image.jpg,2024-03-14,active";
			User expectedUser(id, emailAddress, password, firstName, lastName, profilePicture, dateCreated, accountStatus);


			//Act
			Packet* result = userRoutes.handleUpdateUser(expectedUser);
			
			
			//Assert
			Assert::IsTrue(result != nullptr);

		}

		TEST_METHOD(TestMethod22__Server_HandleDeleteUser)  //tests handleDeleteUser function in UserRoutes
		{
			//Arrange
			UserRoutes userRoutes;
			int id = 0;
			const std::string emailAddress = "test@example.com";
			const std::string password = "password";
			const std::string firstName = "John";
			const std::string lastName = "Doe";
			unsigned char profilepic = 2;
			const std::vector<unsigned char> profilePicture = { 2 };
			const std::string dateCreated = "2024-03-14";
			const std::string accountStatus = "active";
			string userString = "test@example.com,password,John,Doe,image.jpg,2024-03-14,active";
			User expectedUser(id, emailAddress, password, firstName, lastName, profilePicture, dateCreated, accountStatus);


			//Act
			Packet* result = userRoutes.handleDeleteUser(expectedUser);
			
			
			//Assert
			Assert::IsTrue(result != nullptr); 

		}

		TEST_METHOD(TestMethod23__Server_UsersRoutes_getDateCreated) //test getDateCreated function in UsersRoutes
		{
			//Arrange
			int id = 0;
			const std::string emailAddress = "test@example.com";
			const std::string password = "password";
			const std::string firstName = "John";
			const std::string lastName = "Doe";
			unsigned char profilepic = 2;
			const std::vector<unsigned char> profilePicture = { 2 };
			const std::string dateCreated = "2024-03-14";
			const std::string accountStatus = "active";
			User expectedUser(id, emailAddress, password, firstName, lastName, profilePicture, dateCreated, accountStatus);
			tm datecreated_tm = {};
			stringstream ss(dateCreated);
			ss >> get_time(&datecreated_tm, "%Y-%m-%d");
			time_t expectedDateCreated = mktime(&datecreated_tm);


			//Act
			time_t actualDateCreated = expectedUser.getDateCreated();


			//Assert
			Assert::AreEqual(actualDateCreated, expectedDateCreated);

		}


		TEST_METHOD(TestMethod24__Server_UserRoutes_HandleUserRequest_Login) //test handleUserReqest with login request in UsersRoutes
		{
			//Arrange
			Packet* packet = dummyPacket();
			UserRoutes userRoutes;
			MockClientSocket clientSocket;



			//Act
			Packet* result = userRoutes.handleUserRequest(*packet, (SOCKET)&clientSocket, "login");



			//Assert
			Assert::IsNull(result); 
			
			DestroyPacket(packet);

		}

		TEST_METHOD(TestMethod25__Server_UserRoutes_HandleUserRequest_register) //test handleUserRequest with register request in UserRoutes
		{
			//Arrange
			Packet* packet = dummyPacket();
			UserRoutes userRoutes;
			MockClientSocket clientSocket;



			//Act
			Packet* result = userRoutes.handleUserRequest(*packet, (SOCKET)&clientSocket, "register");



			//Assert
			Assert::IsNotNull(result);
			
			DestroyPacket(packet);

		}
		
		TEST_METHOD(TestMethod26__Server_UserRoutes_HandleUserRequest_update) //test handleUserRequest with update request in UserRoutes
		{
			//Arrange
			Packet* packet = dummyPacket();
			UserRoutes userRoutes;
			MockClientSocket clientSocket;



			//Act
			Packet* result = userRoutes.handleUserRequest(*packet, (SOCKET)&clientSocket, "update");



			//Assert
			Assert::IsNotNull(result); 
			
			DestroyPacket(packet);

		}

		TEST_METHOD(TestMethod26__Server_UserRoutes_HandleUserRequest_delete) //test handleUserRequest with delete request in UsersRoute
		{
			//Arrange
			Packet* packet = dummyPacket();
			UserRoutes userRoutes;
			MockClientSocket clientSocket;



			//Act
			Packet* result = userRoutes.handleUserRequest(*packet, (SOCKET)&clientSocket, "delete");



			//Assert
			Assert::IsNotNull(result); 
			
			DestroyPacket(packet);

		}

		TEST_METHOD(TestMethod27__Server_UserRoutes_HandleUserRequest_invalidroute) //test handleUserRequest with invalid request in UsersRoute
		{
			//Arrange
			Packet* packet = dummyPacket();
			UserRoutes userRoutes;
			MockClientSocket clientSocket;



			//Act
			Packet* result = userRoutes.handleUserRequest(*packet, (SOCKET)&clientSocket, "invalidRoute");
			


			//Assert
			Assert::IsNotNull(result);

			DestroyPacket(packet);

		}

		//TEST_METHOD(TestMethod28__Server_Deserializefunction_RouteSignUp) //test Deserialize function with SignUp route in Packet.h
		//{
		//	//Arrange
		//	Packet* actual = dummyPacketSignUp();
		//	LogIn login = dummyLogin();
		//	SignUp signin = dummySignin();
		//	int size = 0;
		//	char* serializedBuffer = SerializeData(actual, size);
		//	Packet* deserializedPacket = CreatePacket();
		//	char* expectedSource = actual->GetHead()->Source;


		//	//Act
		//	Deserialization(deserializedPacket, serializedBuffer, login, signin);
		//	char* actualSource = deserializedPacket->GetHead()->Source;


		//	//Assert
		//	for (int i = 0; i < 7; ++i) {
		//		Assert::AreEqual(actualSource, expectedSource);
		//	}
		//	

		//	DestroyPacket(actual);
		//	DestroyPacket(deserializedPacket);

		//}


		///////////////////////////////////////////////////
		TEST_METHOD(TestMethod29_SQLiteDatabase_Constructor)
		{
			//Arrange
			std::string dbPath = "database.db";
			//
			MockSQLiteDatabase sqldb(dbPath);
			bool actual = sqldb.isOpen();

			//Assert
			Assert::IsTrue(actual);
		}


		///////////////////////////////////////////////////
		TEST_METHOD(TestMethod30_SQLiteDatabase_ExecuteQuery)
		{
			//Arrange
			std::string dbPath = "executeQuereDB.db";
			MockSQLiteDatabase sqldb(dbPath);
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
			Assert::IsTrue(query_exe_result);
		}

		TEST_METHOD(TestMethod31_SQLiteDatabase_PostInsert)
		{
			//Arrange
			std::string dbPath = "executeQuereDB.db";
			MockSQLiteDatabase sqldb(dbPath);
			Packet packet;
			Listing listing;
			int expected = 0;
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

			//Act
			sqlite3_stmt* stmt = nullptr;
			int ListingPostInsertionReturn = sqldb.ListingPostInsert(&stmt, &packet, listing);


			//Assert
			Assert::AreEqual(expected, ListingPostInsertionReturn);
			sqlite3_finalize(stmt);
		}

		//TEST_METHOD(TestMethod32_SQLiteDatabase_GetDB)
		//{
		//	//Arrange
		//	std::string dbPath = "executeQuereDB.db";
		//	MockSQLiteDatabase sqldb(dbPath);
		//	sqlite3* expected = 0;

		//	//Act
		//	sqlite3* actual = sqldb.getDB();
		//	

		//	//Assert
		//	Assert::AreEqual(expected, actual);
		//}

		TEST_METHOD(TestMethod33_SQLiteDatabase_isOpen)
		{
			//Arrange
			std::string dbPath = "isOpenDB.db";
			MockSQLiteDatabase sqldb(dbPath);
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
	
			//Act
			bool actual = sqldb.isOpen();


			//Assert
			Assert::IsTrue(actual);
		}


		TEST_METHOD(TestMethod34_Packet_SetHeader)
		{
			//Arrange
			Packet pkt;
			char source[20] = "127.0.0.1";
			int source_size = sizeof(source);
			char destination[20] = "127.0.0.1";
			int destination_size = sizeof(destination);
			char Route[40] = "LOGIN";
			int Route_size = sizeof(Route);
			bool userFound = false;
			bool Authorization = userFound;
			int length = 0;
			unsigned int lengthCheck = 0;

			//Act
			SetHeaderInformation(&pkt, source, source_size, destination, destination_size, Route, Route_size, Authorization, length);

			//Assert
			Assert::AreEqual(pkt.GetHead()->Source, "127.0.0.1");
			Assert::AreEqual(pkt.GetHead()->Destination, "127.0.0.1");
			Assert::AreEqual(pkt.GetHead()->Route, "LOGIN");
			Assert::AreEqual(pkt.GetHead()->Authorization, false);
			Assert::AreEqual(pkt.GetHead()->Length, lengthCheck);

		}


		TEST_METHOD(TestMethod35_SQLiteDatabase_SignUpWithImageDataInsert)
		{
			//Arrange 
			Packet* packet = dummyPacket();
			SignUp signup = dummySignin();
			std::string dbPath = "isOpenDB.db";
			MockSQLiteDatabase sqldb(dbPath);
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
			int SignUpdataInsertionReturn = sqldb.SignUpWithImageDataInsert(&stmt, packet, signup);

			//Assert
			Assert::AreEqual(0, SignUpdataInsertionReturn);
		}

		TEST_METHOD(TestMethod36_SQLiteDatabase_SignUpWithoutImageDataInsert)
		{
			//Arrange 
			Packet* packet = dummyPacket();
			SignUp signup = dummySignin();
			std::string dbPath = "isOpenDB.db";
			MockSQLiteDatabase sqldb(dbPath);
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
			int SignUpdataInsertionReturn = sqldb.SignUpWithoutImageDataInsert(&stmt, packet, signup);

			//Assert
			Assert::AreEqual(0, SignUpdataInsertionReturn);
		}

		TEST_METHOD(TestMethod37_SQLiteDatabase_SignUpWithoutImageDataInsert)
		{
			//Arrange 
			Packet* packet = dummyPacket();
			SignUp signup = dummySignin();
			std::string dbPath = "isOpenDB.db";
			MockSQLiteDatabase sqldb(dbPath);
			sqlite3_stmt* stmt = nullptr;
			int SignUpdataInsertionReturn = sqldb.SignUpWithImageDataInsert(&stmt, packet, signup);
			char* imageArray = nullptr;
			int imageSize = 0;


			//Act
			int actual = sqldb.FetchImage(&stmt, signup.email, &imageArray, imageSize);


			//Assert
			Assert::AreEqual(0, actual);
		}

		TEST_METHOD(TestMethod38_SerializeUserCheckingData)
		{
			//Arrange 
			Packet* packet = dummyPacket();
			char* expected = "ClientServerRoute11\x0C";
			int TotalSize = 0;
			


			//Act
			char* actual = SerializeUserCheckingData(packet, TotalSize);

			//Assert
			for (int i = 0; i < TotalSize; ++i) {
				Assert::AreEqual(expected[i], actual[i]);
			}
			
		}


		TEST_METHOD(TestMethod39_DeserializationLogIn)
		{
			//Arrange 
			Packet* packet = dummyPacketLogIn();
			LogIn login;
			strcpy_s(login.password, "");
			strcpy_s(login.username, "");
			SignUp signin;
			SignUpCheck check;
			Listing list;
			int size = 0;
			char* serializedBuffer = SerializeData(packet, size);
			Packet* deserializedPacket = CreatePacket();



			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin, check, list);


			//Assert
			Assert::IsTrue(login.password != "");
			Assert::IsTrue(login.username != "");

		}

		TEST_METHOD(TestMethod40_Deserialization_SIGNUP_IMAGEUPLOADED)
		{
			//Arrange 
			Packet* packet = dummyPacketSignUp();
			LogIn login;
			SignUp signin;
			strcpy_s(signin.password, "");
			strcpy_s(signin.username, "");
			strcpy_s(signin.email, "");
			SignUpCheck check;
			Listing list;
			int size = 0;
			char* serializedBuffer = SerializeData(packet, size);
			Packet* deserializedPacket = CreatePacket();



			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin, check, list);


			//Assert
			Assert::IsTrue(signin.password != "");
			Assert::IsTrue(signin.username != "");
			Assert::IsTrue(signin.email != "");
		}


		TEST_METHOD(TestMethod41_Deserialization_SIGNUP_USERCHECK)
		{
			//Arrange 
			Packet* packet = dummyPacketSignUpUserCheck();
			LogIn login;
			SignUp signin;
			SignUpCheck check;
			strcpy_s(check.username, "");
			strcpy_s(check.email, "");
			Listing list;
			int size = 0;
			char* serializedBuffer = SerializeData(packet, size);
			Packet* deserializedPacket = CreatePacket();



			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin, check, list);


			//Assert
			Assert::IsTrue(check.username != "");
			Assert::IsTrue(check.email != "");
		}


		TEST_METHOD(TestMethod42_Deserialization_POST)
		{
			//Arrange 
			Packet* packet = dummyPacketPost();
			LogIn login;
			SignUp signin;
			SignUpCheck check;
			Listing list;
			strcpy_s(list.Title, "");
			int size = 0;
			char* serializedBuffer = SerializeData(packet, size);
			Packet* deserializedPacket = CreatePacket();



			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin, check, list);


			//Assert
			Assert::IsTrue(list.Title != "");
		}



		//The following test were written by Saba Berenji
		// 
		// 
		//void SQLiteDatabase::closeDatabase(sqlite3_stmt** stmt) {
		//	sqlite3_finalize(*stmt);
		//	sqlite3_close(db);
		//}

		TEST_METHOD(TestMethod42_SQLiteDatabase_closeDatabase)
		{
			//Arrange
			std::string dbPath = "closedDB.db";
			MockSQLiteDatabase sqldb(dbPath);
			sqlite3_stmt* stmt = nullptr;

			sqldb.closeDatabase(&stmt);

			//Act
			bool actual = sqldb.isOpen();


			//Assert
			Assert::IsFalse(actual);
		}

		///// <summary>
		///// This test will ensure that the setupConnection2 returns a valid socket
		///// Mock functions are used for this test for functions such as connect which required the presence of a server
		///// </summary>
		TEST_METHOD(TestMethod43_setupConnection2_ValidSocket)
		{

			// Arrange
			WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
			socketFunc socketfunc = MockSocketSuccess;
			connectFunc connectfunc = MockConnectSuccess;
			WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;



			// Act
			SOCKET clientSocket = setupConnection2(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc);


			// Assert 
			Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

		}



		///// <summary>
		///// This test will ensure that the setupConnection2 returns 0 if the socket function returns an invalid socket (INVALID_SOCKET)
		///// Mock functions are used for this test for functions such as connect which required the presence of a server
		///// </summary>
		TEST_METHOD(TestMethod43_etupConnection2_InvalidSocket)
		{

			// Arrange
			WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
			socketFunc socketfunc = MockSocketFailure;
			connectFunc connectfunc = MockConnectSuccess;
			WSACleanupFunc wsacleanup = MockWSACleanupSuccess;


			// Act
			SOCKET clientSocket = setupConnection2(wsastartupfunc, socketfunc, connectfunc, wsacleanup);


			// Assert 
			Assert::AreEqual((SOCKET)0, clientSocket);
		}


		///// <summary>
		///// This test will ensure that the setupConnection2 returns 0 (exits) if the WSAStartup function does not return 0
		///// Mock functions are used for this test for functions such as WSAStartup 
		///// </summary>
		TEST_METHOD(TestMethod44_setupConnection2_WSAStartupFailure)
		{

			// Arrange
			WSAStartupFunc wsastartupfunc = MockWSAStartupFailure;
			socketFunc socketfunc = MockSocketSuccess;
			connectFunc connectfunc = MockConnectSuccess;
			WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;



			// Act
			SOCKET clientSocket = setupConnection2(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc);


			// Assert 
			Assert::AreEqual((SOCKET)0, clientSocket);
		}




		///// <summary>
		///// This test will ensure that the setupConnection2 returns 0 (exits) if the connect function return SOCKET_ERROR and is unable to connect to the server
		///// Mock functions are used for this test for functions such as connect which required the presence of a server
		///// </summary>
		TEST_METHOD(TestMethod45_setupConnection2_ConnectFailure)
		{

			// Arrange
			WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
			socketFunc socketfunc = MockSocketSuccess;
			connectFunc connectfunc = MockConnectFailure;
			WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;



			// Act
			SOCKET clientSocket = setupConnection2(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc);


			// Assert 
			Assert::AreEqual((SOCKET)0, clientSocket);
		}




		/// <summary>
		/// This test will ensure that the CloseSocket funciton returns 1 upon success and handles all the closing and cleaning properly
		/// </summary>
		TEST_METHOD(TestMethod46_CloseSocket_ValidSocket)
		{
			// Arrange
			WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
			socketFunc socketfunc = MockSocketSuccess;
			connectFunc connectfunc = MockConnectSuccess;
			WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;


			SOCKET clientSocket = setupConnection2(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc);

			Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

			closesocketFunc closeMocFunc = MockCloseSocketSuccess;


			// Act
			int result = CloseSocket(clientSocket, closeMocFunc, wsacleanupfunc);

			// Assert
			Assert::AreEqual(1, result);
		}


		/// <summary>
		/// This test will ensure that the CloseSocket funciton returns 0 if the socket is not able to connect to the server
		/// close the socket properly
		/// </summary>
		TEST_METHOD(TestMethod47_CloseSocket_InvalidSocket)
		{
			// Arrange
			WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
			socketFunc socketfunc = MockSocketSuccess;
			connectFunc connectfunc = MockConnectFailure;
			WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;

			closesocketFunc closeMocFunc = MockCloseSocketFailure;

			SOCKET clientSocket = setupConnection2(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc);

			Assert::AreEqual((SOCKET)0, clientSocket);



			// Act
			int result = CloseSocket(clientSocket, closeMocFunc, wsacleanupfunc);

			// Assert
			Assert::AreEqual(0, result);
		}



		/// <summary>
		/// This test will ensure that the CloseSocket funciton returns 2 if there is a problem is cleaning the WSA library
		/// </summary>

		TEST_METHOD(TestMethod48_CloseSocket_WSALibraryCleaningFailure)
		{
			// Arrange
			WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
			socketFunc socketfunc = MockSocketSuccess;
			connectFunc connectfunc = MockConnectSuccess;
			WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;


			SOCKET clientSocket = setupConnection2(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc);

			Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

			closesocketFunc closeMocFunc = MockCloseSocketSuccess;
			WSACleanupFunc wsacleanupfuncfailure = MockWSACleanupFailure;

			// Act
			int result = CloseSocket(clientSocket, closeMocFunc, wsacleanupfuncfailure);

			// Assert
			Assert::AreEqual(2, result);
		}


		///// <summary>
		///// This test ensures that the sendData function (here using a mock function) will return 1 in case of a successfull send
		///// </summary>
		TEST_METHOD(TestMethod49_sendData_ValidSocket_SuccessfullSend)
		{
			// Arrange

			WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
			socketFunc socketfunc = MockSocketSuccess;
			connectFunc connectfunc = MockConnectSuccess;
			WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;
			SendFunction sendfunc = MockSendSuccess;

			SOCKET clientSocket = setupConnection2(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc);


			Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

			char TxBuffer[20] = "This is for testing";
			int totalSize = sizeof(TxBuffer);

			// Act
			int result = sendData(clientSocket, TxBuffer, totalSize, sendfunc);

			// Assert
			Assert::AreEqual(1, result);
		}



		///// <summary>
		///// This test ensures that the sendData function will return 0 in case of an unsuccessfull send
		///// </summary>
		TEST_METHOD(TestMethod45_sendData_InvalidSocket_UnsuccessfulSend)
		{
			// Arrange

			WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
			socketFunc socketfunc = MockSocketSuccess;
			connectFunc connectfunc = MockConnectSuccess;
			WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;
			SendFunction sendfunc = MockSendFailure;

			SOCKET clientSocket = setupConnection2(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc);


			Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

			char TxBuffer[20] = "This is for testing";
			int totalSize = sizeof(TxBuffer);

			// Act
			int result = sendData(clientSocket, TxBuffer, totalSize, sendfunc);

			// Assert
			Assert::AreEqual(0, result);
		}

	};
}

