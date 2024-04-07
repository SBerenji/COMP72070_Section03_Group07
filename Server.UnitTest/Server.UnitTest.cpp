#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include "CppUnitTest.h"
#include "../SwapifyServer/file_utils.h"
#include "../SwapifyServer/Packet.h"
#include "../SwapifyServer/SQLiteDatabase.h"
#include "../SwapifyServer/User.h"
#include "../SwapifyServer/UsersRoute.h"
#include "../SwapifyServer/RequestLogger.h"
#include <fstream>
#include <sstream>
#include <memory>
#include <thread>
#include <string>
#include <vector>


using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern bool userFound;
extern int callback(void* NotUsed, int argc, char** argv, char** azColName);
extern int main();



std::vector<char*> readFromFile(const std::string& filename) {
	std::vector<char*> result; // Store the converted values
	std::string inputStr = "";

	std::ifstream file(filename);
	if (file.is_open()) {
		while (std::getline(file, inputStr)) {
			std::istringstream issLine(inputStr);
			std::string title;
			std::getline(issLine, title, ':');

			std::string value;
			std::getline(issLine, value, ';');

			// Allocate memory for the C-style string
			char* newValue = new char[value.size() + 1];
			strncpy(newValue, value.c_str(), value.size() + 1);

			// Store the result
			result.push_back(newValue);
		}
		file.close();
	}
	else {
		std::cerr << "Error opening file: " << filename << std::endl;
	}

	return result;
}



//void  RequestLogger::logPacket(Packet& packet) {
//	std::ofstream outFile(filename, std::ios::app); // Open in append mode
//	if (outFile.is_open()) {
//		auto head = packet.GetHead();
//
//		outFile << std::dec;
//
//		time_t currentTime = time(0); //determines the current dat and time on system
//		char* timeString = ctime(&currentTime); //converts time to string 
//
//		outFile << "Time: " << timeString << " Source: " << "Client" << " Destination: " << "Server" << " Route: " << head->Route << std::endl;
//
//		outFile.close();
//	}
//	else {
//		std::cerr << "Error opening log file." << std::endl;
//	}
//}

Packet* dummyPacket() //creates a filled packet to be tested
{
	Packet* actual = CreatePacket(); //creates a new packet
	char* Source = "127.0.0.1";
	char* Destination = "127.0.0.1";
	char* Route = "LOGIN";
	bool Authorization = true;
	unsigned int Length = 0;
	unsigned int User = 2;
	char* Data = "username..password..";
	unsigned int checksum = CalculateChecksum();


	SetHeaderInformation(actual, Source, strlen(Source), Destination, strlen(Destination), Route, strlen(Route), Authorization, Length);
	SetBody(actual, User, Data, strlen(Data));
	actual->GetTail()->Checksum = checksum;

	return actual;
}

Packet* dummyPacketNoBody() //creates a filled packet to be tested
{
	Packet* actual = CreatePacket(); //creates a new packet
	char* Source = "127.0.0.1";
	char* Destination = "127.0.0.1";
	char* Route = "LOGIN";
	bool Authorization = true;
	unsigned int Length = 0;
	unsigned int checksum = CalculateChecksum();


	SetHeaderInformation(actual, Source, strlen(Source), Destination, strlen(Destination), Route, strlen(Route), Authorization, Length);
	actual->GetTail()->Checksum = checksum;

	return actual;
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

Packet* dummyPacketPostInfo() {
	Packet* Pkt = new Packet();
	Listing list;
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

Packet* dummyPacketSignUp(SignUp& signup)
{
	Packet* Pkt = new Packet();
	Pkt->GetBody()->Data = nullptr;


	memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
	memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
	memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


	SetHeaderInformation(Pkt, "Source", strlen("Source"), "Destination", strlen("Destination"), "SIGNUP_IMAGEUPLOADED", strlen("SIGNUP_IMAGEUPLOADED"), true, 0);


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

Packet* dummyPacketSignUp()
{
	Packet* Pkt = new Packet();
	SignUp signup;
	Pkt->GetBody()->Data = nullptr;


	memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
	memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
	memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


	SetHeaderInformation(Pkt, "Source", strlen("Source"), "Destination", strlen("Destination"), "SIGNUP_IMAGEUPLOADED", strlen("SIGNUP_IMAGEUPLOADED"), true, 0);


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


Packet* dummyPacketSignUpUserCheck()
{
	Packet* Pkt = new Packet();
	SignUpCheck check;
	Pkt->GetBody()->Data = nullptr;


	memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
	memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
	memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


	SetHeaderInformation(Pkt, "Source", strlen("Source"), "Destination", strlen("Destination"), "SIGNUP_USERCHECK", strlen("SIGNUP_USERCHECK"), true, 0);
	strcpy_s(check.username, "Username");
	strcpy_s(check.email, "Email");
	SetBody(Pkt, 'S', reinterpret_cast<char*>(&check), sizeof(check));


	return Pkt;
}

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



namespace ServerUnitTest
{
	TEST_CLASS(ServerUnitTest)
	{
	public:

		TEST_METHOD(TESTSVR04_GetHead) //tests the GetHead() in the Packet.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			char* expectedSource = "127.0.0.1";
			char* expectedDestination = "127.0.0.1";
			char* expectedRoute = "LOGIN";
			bool expectedAuthorization = true;
			unsigned int expectedLength = 21;


			//Act
			char* actualSource = actual->GetHead()->Source;
			char* actualDestination = actual->GetHead()->Destination;
			char* actualRoute = actual->GetHead()->Route;
			bool actualAuthorization = actual->GetHead()->Authorization;
			unsigned int actualLength = actual->GetHead()->Length;


			//Assert 
			for (int i = 0; i < strlen(expectedSource); i++)
			{
				Assert::AreEqual(expectedSource[i], actualSource[i]);
				Assert::AreEqual(expectedDestination[i], actualDestination[i]);
			}
			for (int i = 0; i < strlen(expectedRoute); i++)
			{
				Assert::AreEqual(expectedRoute[i], actualRoute[i]);
			}
			Assert::AreEqual(expectedAuthorization, actualAuthorization);
			Assert::AreEqual(expectedLength, actualLength);

			DestroyPacket(actual);
		}


		TEST_METHOD(TESTSVR26_GetBody) //tests the GetBody function in Packet.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			int DataSize = 21;
			char* expectedData = "username..password..";
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


		TEST_METHOD(TESTSVR22_GetTail) //tests the GetTail function in Packet.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			unsigned int expectedChecksum = 0xFF00FF00;


			//Act
			unsigned int actualChecksum = actual->GetTail()->Checksum;


			//Assert
			Assert::AreEqual(expectedChecksum, actualChecksum);

			DestroyPacket(actual);
		}

		TEST_METHOD(TESTSVR23_CreatePacket) //tests the CreatePacket function in Packet.h
		{
			//Arrange


			//Act
			Packet* packet = CreatePacket();


			//Assert
			Assert::IsTrue(packet);

			DestroyPacket(packet);
		}

		TEST_METHOD(TESTSVR24_DestroyPacket) //tests the DestroyPacket function in Packet.h
		{
			//Arrange
			Packet* packet = dummyPacket();
			unsigned int expected = 21;


			//Act
			DestroyPacket(packet);
			unsigned int actual = packet->GetHead()->Length;

			//Assert
			Assert::AreNotEqual(expected, actual);

		}



		TEST_METHOD(TESTSVR25_DisplayFunction) //tests the Display function in PAcket.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			LogIn login;
			SignUp signin;
			Listing list;
			std::stringstream actualOutput;
			string expectedOutput = "Source:  127.0.0.1\n"
				"Destination: 127.0.0.1\n"
				"Route:  LOGIN\n"
				"Authorization:  1\n"
				"Length:  21\n"
				"User:  2\n"
				"Data:  username..password..\n";


			//Act
			Display(actual, actualOutput, login, signin, list);


			//Assert
			Assert::AreEqual(expectedOutput, actualOutput.str());

			DestroyPacket(actual);

		}

		TEST_METHOD(TESTSVR10_Deserializefunction) //tests the deserialize function in Packet.h
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

			char* expectedSource = "127.0.0.1";
			char* expectedDestination = "127.0.0.1";
			char* expectedRoute = "LOGIN";
			bool expectedAuthorization = true;
			unsigned int expectedLength = 21;
			unsigned char expectedUser = 2;
			unsigned int expectedChecksum = 0xFF00FF00;


			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin, check, list);
			char* actualSource = deserializedPacket->GetHead()->Source;
			char* actualDestination = deserializedPacket->GetHead()->Destination;
			char* actualRoute = deserializedPacket->GetHead()->Route;
			bool actualAuthorization = deserializedPacket->GetHead()->Authorization;
			unsigned int actualLength = deserializedPacket->GetHead()->Length;
			unsigned char actualUser = deserializedPacket->GetBody()->User;
			unsigned int actualChecksum = deserializedPacket->GetTail()->Checksum;



			//Assert
			for (int i = 0; i < strlen(expectedSource); i++)
			{
				Assert::AreEqual(expectedSource[i], actualSource[i]);
				Assert::AreEqual(expectedDestination[i], actualDestination[i]);
			}
			for (int i = 0; i < strlen(expectedRoute); i++)
			{
				Assert::AreEqual(expectedRoute[i], actualRoute[i]);
			}
			Assert::AreEqual(actualAuthorization, expectedAuthorization);
			Assert::AreEqual(actualLength, expectedLength);
			Assert::AreEqual(actualUser, expectedUser);
			Assert::AreEqual(0, strcmp(actual->GetBody()->Data, deserializedPacket->GetBody()->Data));
			Assert::AreEqual(actualChecksum, expectedChecksum);

			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}

		TEST_METHOD(TESTSVR06_Deserializefunction_LogIn) //tests the deserialize function in Packet.h
		{
			//Arrange
			Packet* actual = dummyPacket();
			LogIn login;
			SignUp signin;
			int size = 0;
			char* serializedBuffer = SerializeData(actual, size);
			Packet* deserializedPacket = CreatePacket();

			char* expectedSource = "127.0.0.1";
			char* expectedDestination = "127.0.0.1";
			char* expectedRoute = "LOGIN";
			bool expectedAuthorization = true;
			unsigned int expectedLength = 21;
			unsigned char expectedUser = 2;
			unsigned int expectedChecksum = 0xFF00FF00;


			//Act
			Deserialization(deserializedPacket, serializedBuffer);
			char* actualSource = deserializedPacket->GetHead()->Source;
			char* actualDestination = deserializedPacket->GetHead()->Destination;
			char* actualRoute = deserializedPacket->GetHead()->Route;
			bool actualAuthorization = deserializedPacket->GetHead()->Authorization;
			unsigned int actualLength = deserializedPacket->GetHead()->Length;


			//Assert
			for (int i = 0; i < strlen(expectedSource); i++)
			{
				Assert::AreEqual(expectedSource[i], actualSource[i]);
				Assert::AreEqual(expectedDestination[i], actualDestination[i]);
			}
			for (int i = 0; i < strlen(expectedRoute); i++)
			{
				Assert::AreEqual(expectedRoute[i], actualRoute[i]);
			}
			Assert::AreEqual(actualAuthorization, expectedAuthorization);
			Assert::AreEqual(actualLength, expectedLength);

			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}

		TEST_METHOD(TESTSVR10_Deserializefunction_SignUp) //test Deserialize function with SignUp route in Packet.h
		{
			//Arrange
			SignUp signin;
			Packet* actual = dummyPacketSignUp();
			LogIn login;
			SignUpCheck check;
			Listing list;
			int size = 0;
			char* serializedBuffer = SerializeData(actual, size);
			Packet* deserializedPacket = CreatePacket();
			char* expectedUsername = "Username";


			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin, check, list);
			char* actualUsername = signin.username;


			//Assert
			for (int i = 0; i < strlen(expectedUsername); ++i) {
				Assert::AreEqual(expectedUsername[i], actualUsername[i]);
			}
			

			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}

		TEST_METHOD(TESTSVR10_Deserializefunction_SIGNUP_USERCHECK) //test Deserialize function with SignUp route in Packet.h
		{
			//Arrange
			SignUp signin;
			Packet* actual = dummyPacketSignUpUserCheck();
			
			LogIn login;
			SignUpCheck check;
			Listing list;
			int size = 0;
			char* serializedBuffer = SerializeData(actual, size);
			Packet* deserializedPacket = CreatePacket();
			char* expectedEmail = "Email";


			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin, check, list);
			char* actualEmail = check.email;


			//Assert
			for (int i = 0; i < strlen(expectedEmail); ++i) {
				Assert::AreEqual(expectedEmail[i], actualEmail[i]);
			}


			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}

		TEST_METHOD(TESTSVR10_Deserializefunction_POST) //test Deserialize function with SignUp route in Packet.h
		{
			//Arrange
			SignUp signin;
			Packet* actual = dummyPacketPostInfo();

			LogIn login;
			SignUpCheck check;
			Listing list;
			int size = 0;
			char* serializedBuffer = SerializeData(actual, size);
			Packet* deserializedPacket = CreatePacket();
			char* expectedDelivery = "Delivery";


			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin, check, list);
			char* actualDelivery = list.Delivery;


			//Assert
			for (int i = 0; i < strlen(expectedDelivery); ++i) {
				Assert::AreEqual(expectedDelivery[i], actualDelivery[i]);
			}


			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}

		///////////////////////////
		TEST_METHOD(TESTSVR27_SetHeaderfunction) //testing the SetHeader function in Packet.h
		{
			//Arrange
			Packet* actual = CreatePacket();
			Packet* tempHeader = dummyPacket();
			char* expectedSource = "127.0.0.1";
			char* expectedDestination = "127.0.0.1";
			char* expectedRoute = "LOGIN";
			bool expectedAuthorization = true;
			unsigned int expectedLength = 21;

		


			//Act
			SetHeader(actual, tempHeader->GetHead());
			char* actualSource = actual->GetHead()->Source;
			char* actualDestination = actual->GetHead()->Destination;
			char* actualRoute = actual->GetHead()->Route;
			bool actualAuthorization = actual->GetHead()->Authorization;
			unsigned int actualLength = actual->GetHead()->Length;


			//Assert
			for (int i = 0; i < strlen(expectedSource); i++)
			{
				Assert::AreEqual(expectedSource[i], actualSource[i]);
				Assert::AreEqual(expectedDestination[i], actualDestination[i]);
			}
			for (int i = 0; i < strlen(expectedRoute); i++)
			{
				Assert::AreEqual(expectedRoute[i], actualRoute[i]);
			}
			Assert::AreEqual(actualAuthorization, expectedAuthorization);
			Assert::AreEqual(actualLength, expectedLength);

			DestroyPacket(actual);
			DestroyPacket(tempHeader);

		}

		TEST_METHOD(TESTSVR28_SetBodyfunction) //testing the set body function in Packet.h
		{
			//Arrange
			Packet* actual = CreatePacket();
			unsigned char expectedUser = 2;
			char* expectedData = "username..password..";
			unsigned int expectedLength = 20;


			//Act
			SetBody(actual, expectedUser, expectedData, expectedLength);
			unsigned char actualUser = actual->GetBody()->User;
			unsigned int actualLength = actual->GetHead()->Length;

			//Assert
			Assert::AreEqual(actualUser, expectedUser);
			Assert::AreEqual(0, strcmp(actual->GetBody()->Data, expectedData));
			Assert::AreEqual(actualLength, expectedLength + 1);

			DestroyPacket(actual);

		}

		TEST_METHOD(TESTSVR05_Serializefunction) //testing the Serialize function in Packet.h
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


		TEST_METHOD(TESTSVR12_Serialize_NoBody)  //tests the Serialize function with no body
		{
			//Arrange
			Packet* actual = dummyPacketNoBody();
			int size = 0;


			//Act
			char* serializedBuffer = nullptr;
			serializedBuffer = SerializeData(actual, size);


			//Assert
			Assert::IsNotNull(serializedBuffer);

			DestroyPacket(actual);

		}


		TEST_METHOD(TESTSVR13_GetBody)
		{
			//Arrange
			Packet* actual = dummyPacket();
			char* expectedData = "username..password..";
			unsigned int user = 2;


			//Act
			unsigned int actualUser = actual->GetBody()->User;
			char* actualData = actual->GetBody()->Data;


			//Assert
			Assert::AreEqual(user, actualUser);
			Assert::AreEqual(0, strcmp(expectedData, actualData));


			DestroyPacket(actual);

		}


		TEST_METHOD(TESTSVR14_GetTail)
		{
			//Arrange
			Packet* actual = dummyPacket();
			unsigned int expectedChecksum = 0xFF00FF00;

			//Act
			unsigned int actualChecksum = actual->GetTail()->Checksum;


			//Assert
			Assert::AreEqual(expectedChecksum, actualChecksum);


			DestroyPacket(actual);

		}

		



		


		TEST_METHOD(TESTSVR15_SetHeader)
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


		TEST_METHOD(TESTSVR05_SerializeUserCheckingData)
		{
			//Arrange 
			Packet* packet = dummyPacket();
			char* expected = "127.0.0.1";
			int TotalSize = 0;



			//Act
			char* actual = SerializeUserCheckingData(packet, TotalSize);

			//Assert
			for (int i = 0; i < strlen(actual); i++)
			{
				Assert::AreEqual(expected[i], actual[i]);
			}

		}


		TEST_METHOD(TESTSVR06_DeserializationLogIn)
		{
			//Arrange 
			Packet* packet = dummyPacket();
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

		TEST_METHOD(TESTSVR02_ExecuteQuery)
		{
			//Arrange
			SQLiteDatabase sqldb(":memory:");
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


			sqlite3_stmt* stmt = nullptr;
			sqldb.closeDatabase(&stmt);
		}
		///////////////////////////////////////////////
		TEST_METHOD(TESTSVR09_ListingPostInsert)
		{
			//Arrange
			Listing list;
			Packet* Pkt = dummyPacketPostInfo(list);
		
			SQLiteDatabase sqldb(":memory:");
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
			Assert::AreEqual(0, ListingPostInsertionReturn);

			sqldb.closeDatabase(&stmt);
		}

		TEST_METHOD(TESTSVR01_SignUpWithImageDataInsert)
		{
			//Arrange
			SignUp signup;
			Packet* packet = dummyPacketSignUp(signup);
			SQLiteDatabase sqldb(":memory:");
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
			Assert::AreEqual(0, SignUpdataInsertionReturn);

			sqldb.closeDatabase(&stmt);
		}

		TEST_METHOD(TESTSVR03_SignUpWithoutImageDataInsert) 
		{
			//Arrange
			SignUp signup;
			Packet* packet = dummyPacketSignUp(signup);
			SQLiteDatabase sqldb(":memory:");
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
			Assert::AreEqual(0, SignUpdataInsertionReturn);

			sqldb.closeDatabase(&stmt);
		}

		TEST_METHOD(TESTSVR30_FetchImage_Successfu) {
			//Arrange
			SignUp signup;

			Packet* Pkt = dummyPacketSignUp(signup);


			SQLiteDatabase sqldb(":memory:");

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


			//Act
			int fetch = sqldb.FetchImage(&stmt, signup.email, &imageArray, imageSize);


			//Assert
			Assert::AreEqual(0, fetch);
		}

		TEST_METHOD(TESTSVR30_GetDB)
		{
			//Arrange			
			SQLiteDatabase sqldb(":memory:");
			sqlite3* sq = nullptr;
			

			//Act
			sq = sqldb.getDB();


			//Assert
			Assert::IsNotNull(sq);
		}


		TEST_METHOD(TESTSVR29_isOpen)
		{
			//Arrange			
			SQLiteDatabase sqldb(":memory:");
			//sqlite3* sq = nullptr;


			//Act
			bool isopen = sqldb.isOpen();


			//Assert
			Assert::IsTrue(isopen);
		}


		//The following test were written by Saba Berenji
		// 
		// 

		///// <summary>
		///// This test will ensure that the setupConnection2 returns a valid socket
		///// Mock functions are used for this test for functions such as connect which required the presence of a server
		///// </summary>
		TEST_METHOD(TESTSV18_setupConnection2_ValidSocket)
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
		TEST_METHOD(TESTSVR18_SetupConnection2_InvalidSocket)
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
		TEST_METHOD(TESTSVR31_setupConnection2_WSAStartupFailure)
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
		TEST_METHOD(TESTSVR33_setupConnection2_ConnectFailure)
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
		TEST_METHOD(TESTSVR19_CloseSocket_ValidSocket)
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
		TEST_METHOD(TESTSVR35_CloseSocket_InvalidSocket)
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

		TEST_METHOD(TESTSVR31_CloseSocket_WSALibraryCleaningFailure)
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
		TEST_METHOD(TESTSVR18_sendData_ValidSocket_SuccessfullSend)
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
		TEST_METHOD(TESTSVR32_sendData_InvalidSocket_UnsuccessfulSend)
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



		///// <summary>
		///// This test ensures that the sendData function will return 0 in case of an unsuccessfull send
		///// </summary>
		TEST_METHOD(TESTSVR03_callback)
		{
			// Arrange
			char* dummyargv[] = { "row1", "row2" };
			char* dummyazColName[] = { "col1", "col2" };


			// Act
			callback(NULL, 2, dummyargv, dummyazColName);
			
			// Assert
			Assert::IsTrue(userFound);
		}


		//TEST_METHOD(TESTSVR_logger)
		//{
		//	// Arrange
		//	RequestLogger logger("Log_File_Server.txt");
		//	LogIn log;
		//	SignUp signup;
		//	SignUpCheck check;
		//	Listing list;
		//	Deserialization(Pkt, RxBuffer, log, signup, check, list);

		//	logger.logPacket(*Pkt);

		//	// Act
		//	callback(NULL, 2, dummyargv, dummyazColName);

		//	// Assert
		//	Assert::IsTrue(userFound);
		//}


		//Sets up a test file to be tested with a itemType, linedetails and TEXT_FILE
		void fileSetup(const string TEXT_FILE) 
		{
			std::ofstream ofs(TEXT_FILE);
			ofs << "Time: Sun Apr  7 10:01:47 2024" << std::endl;
			ofs << "; Source: Client; Destination: Server; Route: LOGIN" << std::endl;
			ofs.close();
		}

		//Sets up a test file to be tested with a itemType, linedetails and TEXT_FILE
		void fileSetupListing(const string TEXT_FILE)
		{
			std::ofstream ofs(TEXT_FILE);
			ofs << "Time: Sun Apr  7 10:01:47 2024" << std::endl;
			ofs << "; Source: Server; Destination: Client; Route: Listing" << std::endl;
			ofs.close();
		}
		
		//Sets up a test file to be tested with a itemType, linedetails and TEXT_FILE
		void fileSetupImage(const string TEXT_FILE)
		{
			std::ofstream ofs(TEXT_FILE);
			ofs << "Time: Sun Apr  7 10:01:47 2024" << std::endl;
			ofs << "; Source: Server; Destination: Client; Route: Image" << std::endl;
			ofs.close();
		}

		TEST_METHOD(TestMethod3_logPacketLine_Same)
		{
			string TEXT_FILE = "TempFil.txt";
			fileSetup(TEXT_FILE);  //create file with expected data
			vector<char*> expectedDatafromFile = readFromFile("TempFil.txt"); //read data to vector

			Packet* p = dummyPacket();
			


			RequestLogger log("testFil.txt");
			log.logPacket(*p); //write to log file
			vector<char*> actualdatafromFile = readFromFile("testFil.txt"); //read from log file into actual results vector

			for (size_t i = 31; i < expectedDatafromFile.size(); ++i) {
				Assert::AreEqual(expectedDatafromFile[i], actualdatafromFile[i]); //assert to check that each char is identical
			}

		}


		TEST_METHOD(TestMethod)
		{
			string TEXT_FILE = "ListingTemp.txt";
			fileSetupListing(TEXT_FILE);  //create file with expected data
			vector<char*> expectedDatafromFile = readFromFile("ListingTemp.txt"); //read data to vector

			Packet* p = dummyPacket();



			RequestLogger log("ListingTest.txt");
			log.logListingSend(); //write to log file
			vector<char*> actualdatafromFile = readFromFile("ListingTest.txt"); //read from log file into actual results vector

			for (size_t i = 31; i < expectedDatafromFile.size(); ++i) {
				Assert::AreEqual(expectedDatafromFile[i], actualdatafromFile[i]); //assert to check that each char is identical
			}

		}


		TEST_METHOD(TestMethod_Image)
		{
			string TEXT_FILE = "ImageTemp.txt";
			fileSetupImage(TEXT_FILE);  //create file with expected data
			vector<char*> expectedDatafromFile = readFromFile("ImageTemp.txt"); //read data to vector

			Packet* p = dummyPacket();



			RequestLogger log("ImageTest.txt");
			log.logImageSend(); //write to log file
			vector<char*> actualdatafromFile = readFromFile("ImageTest.txt"); //read from log file into actual results vector

			for (size_t i = 31; i < expectedDatafromFile.size(); ++i) {
				Assert::AreEqual(expectedDatafromFile[i], actualdatafromFile[i]); //assert to check that each char is identical
			}

		}


		TEST_METHOD(TESTSVR30_FetchImage_UnSuccessfu) 
		{
			//Arrange
			SignUp signup;

			SQLiteDatabase sqldb("");

			sqlite3_stmt* stmt = nullptr;

			// Retrieve BLOB data
			char* imageArray = nullptr;
			int imageSize = 0;


			//Act
			int fetch = sqldb.FetchImage(&stmt, signup.email, &imageArray, imageSize);


			//Assert
			Assert::AreEqual(-1, fetch);
		}



		TEST_METHOD(TESTSVR03_SignUpWithoutImageDataInsert_Unsuccessful)
		{
			//Arrange
			SignUp signup;
			Packet* packet = CreatePacket();
			SQLiteDatabase sqldb("");
	
			sqlite3_stmt* stmt = nullptr;

			sqldb.closeDatabase(&stmt);

			//Act
			int SignUpdataInsertionReturn = sqldb.SignUpWithoutImageDataInsert(&stmt, packet, signup);


			//Assert
			Assert::AreEqual(-1, SignUpdataInsertionReturn);

		}

		

	};


}


