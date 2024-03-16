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

Packet* dummyPacketSignUp() //creates fummy signup packet
{
	Packet* actual = CreatePacket();


	strcpy_s(actual->GetHead()->Source, "Client"); //sets source
	strcpy_s(actual->GetHead()->Destination, "Server"); //sets destination
	strcpy_s(actual->GetHead()->Route, "SIGNUP"); //set route to sign up 
	actual->GetHead()->Authorization = true; //sets authorization
	actual->GetHead()->Length = 12; //sets length
	int DataSize = 5; 
	char* Data = "data"; 
	actual->GetBody()->User = 2; //sets user 
	actual->GetBody()->Data = new char[DataSize]; //assigns memory for user
	actual->GetBody()->Data[DataSize - 1] = '\0'; //sets lat item in data to null
	memcpy(actual->GetBody()->Data, Data, DataSize); //sets data
	actual->GetTail()->Checksum = 2; //sets checksum

	return actual;
}

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


namespace SwapifyServerTests
{
	TEST_CLASS(SwapifyServerTests)
	{
	public:

		//TEST_METHOD(TestMethod1_TESTSVR08_logPacketLine_Same)
		//{

		//	Packet* p = CreatePacket();

		//	p->accessHeader().Length = 10;
		//	p->accessBody().User = 5;
		//	p->accessBody().Data = new char[p->accessHeader().Length];
		//	strcpy_s(p->accessBody().Data, p->accessHeader().Length + 1, "TestData");
		//	p->accessTail().Checksum = 123;

		//	RequestLogger log("testLogFile.txt");
		//	log.logPacket(*p);
		//	vector<char*> actualdatafromFile = readFromFile("testFile.txt");

		//	std::ifstream inFile("testLogFile.txt");
		//	std::string line;

		//	// Read the logged data from the file
		//	std::getline(inFile, line); // Source
		//	Assert::AreEqual(std::string("Source:  "), line.substr(0, 9));

		//	std::getline(inFile, line); // Destination
		//	Assert::AreEqual(std::string("Destination: "), line.substr(0, 13));

		//	std::getline(inFile, line); // Route
		//	Assert::AreEqual(std::string("Route:  "), line.substr(0, 8));

		//	std::getline(inFile, line); // Authorization
		//	Assert::AreEqual(std::string("Authorization: "), line.substr(0, 15));

		//	std::getline(inFile, line); // Length
		//	Assert::AreEqual(std::string("Length:  10"), line.substr(0, 11));

		//	std::getline(inFile, line); // User
		//	Assert::AreEqual(std::string("User:  "), line.substr(0, 7));

		//	std::getline(inFile, line); // Data
		//	Assert::AreEqual(std::string("Data:  T"), line.substr(0, 14));

		//	std::getline(inFile, line); // Checksum
		//	Assert::AreEqual(std::string("Checksum:  7b"), line.substr(0, 13));

		//	// Close the file
		//	inFile.close();

		//	// Clean up
		//	DestroyPacket(p);
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
			std::stringstream actualOutput;
			string expectedOutput = "Source:  Client\n"
				"Destination: Server\n"
				"Route:  Route1\n"
				"Authorization:  1\n"
				"Length:  12\n"
				"User:  \n" 
				"Username: username\n"
				"Password: password\n"
				"Email: email\n"
				"Image Array: i\n"
				"Data:  data\n"
				"Checksum  2\n";


			//Act
			Display(actual, actualOutput, login, signin);


			//Assert
			Assert::AreEqual(expectedOutput, actualOutput.str());

			DestroyPacket(actual);

		}

		TEST_METHOD(TestMethod8_TESTSVR05_Server_Packet_Deserializefunction) //tests the deserialize function in Packet.h
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
			unsigned char expectedUser = 2;
			unsigned int expectedChecksum = 2;


			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin);
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


		TEST_METHOD(TestMethod13_TESTSVR11_Server_Packet_Packet_CheckSumDrop)  //test that the packet is dropped if checksum do not match
		{
			//Arrange
			Packet* actual = dummyPacket();
			LogIn login = dummyLogin();
			SignUp signin = dummySignin();
			int size = 0;
			Packet* deserializedPacket = CreatePacket();
			char expectedSource = '\0';


			//Act
			char* serializedBuffer = SerializeData(actual, size);
			Deserialization(deserializedPacket, serializedBuffer, login, signin);
			char actualSource = deserializedPacket->GetHead()->Source[0];


			//Assert
			Assert::AreEqual(expectedSource, actualSource); 

			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}

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

		//TEST_METHOD(TestMethod15_TESTSVR13_Server_GetBody)  /
		//{
		//	Packet* actual = dummyPacket();

		//	char Data[] = "data";
	

		//	unsigned char user = 2;


		//	Assert::AreEqual(actual->GetBody()->User, user);
		//	Assert::AreEqual((string)actual->GetBody()->Data, (string)Data); 


		//	DestroyPacket(actual);

		//}

		//TEST_METHOD(TestMethod16_TESTSVR14_Server_GetTail) 
		//{
		//	Packet* actual = dummyPacket();


		//	Assert::IsTrue(actual->GetTail()->Checksum == 2); 


		//	DestroyPacket(actual);

		//}

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

		TEST_METHOD(TestMethod28__Server_Deserializefunction_RouteSignUp) //test Deserialize function with SignUp route in Packet.h
		{
			//Arrange
			Packet* actual = dummyPacketSignUp();
			LogIn login = dummyLogin();
			SignUp signin = dummySignin();
			int size = 0;
			char* serializedBuffer = SerializeData(actual, size);
			Packet* deserializedPacket = CreatePacket();
			char* expectedSource = actual->GetHead()->Source;


			//Act
			Deserialization(deserializedPacket, serializedBuffer, login, signin);
			char* actualSource = deserializedPacket->GetHead()->Source;


			//Assert
			for (int i = 0; i < 7; ++i) {
				Assert::AreEqual(actualSource, expectedSource);
			}
			

			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}

	};
}


