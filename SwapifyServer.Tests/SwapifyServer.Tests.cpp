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


Packet* dummyPacket()
{
	Packet* actual = CreatePacket();


	strcpy_s(actual->GetHead()->Source, "Client");
	strcpy_s(actual->GetHead()->Destination, "Server");
	strcpy_s(actual->GetHead()->Route, "Route1");
	actual->GetHead()->Authorization = true;
	actual->GetHead()->Length = 12;
	int DataSize = 5;
	char* Data = "data";
	actual->GetBody()->User = 2;
	actual->GetBody()->Data = new char[DataSize];
	actual->GetBody()->Data[DataSize - 1] = '\0';
	memcpy(actual->GetBody()->Data, Data, DataSize);
	actual->GetTail()->Checksum = 2;
	
	return actual;
}


LogIn dummyLogin()
{
	LogIn log;
	strcpy_s(log.username, "username");
	strcpy_s(log.password, "password");
	return log;
}

SignUp dummySignin()
{
	SignUp signin;
	strcpy_s(signin.username, "username");
	strcpy_s(signin.password, "password");
	strcpy_s(signin.email, "email");
	char* imagestructarray = "isa";
	signin.ImageStructArray = new char[sizeof(imagestructarray)];
	memcpy(signin.ImageStructArray, imagestructarray, 3);

	return signin;
}


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
			std::getline(issLine, value);

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


namespace SwapifyServerTests
{
	TEST_CLASS(SwapifyServerTests)
	{
	public:
		////Sets up a test file to be tested with a itemType, linedetails and TEXT_FILE
		void fileSetup(const string TEXT_FILE) {
			std::ofstream ofs(TEXT_FILE);
			ofs << "Source:  Client" << std::endl;
			ofs << "Destination: Server" << std::endl;
			ofs << "Route:  Route1" << std::endl;
			ofs << "Authorization:  1" << std::endl;
			ofs << "Length:  " << std::endl;
			ofs << "" << std::endl;
			ofs << "User:  user1" << std::endl;
			ofs << "Data:  u" << std::endl;
			ofs << "Checksum:  7b" << std::endl;
			ofs.close();
		}



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

		TEST_METHOD(TestMethod2_TESTSVR015_Server_GetHeadFunction)
		{
			Packet* actual = dummyPacket();

			unsigned int length = 12;

			Assert::AreEqual("Client", actual->GetHead()->Source);
			Assert::AreEqual("Server", actual->GetHead()->Destination);
			Assert::AreEqual("Route1", actual->GetHead()->Route);
			Assert::AreEqual(true, actual->GetHead()->Authorization);
			Assert::AreEqual(length, actual->GetHead()->Length);

			DestroyPacket(actual);
		}

		TEST_METHOD(TestMethod3_TESTSVR16_Server_GetBodyFunction)
		{
			Packet* actual = dummyPacket();


			int DataSize = 5;
			char* data = "data";
			unsigned char user = 2;

			Assert::AreEqual(user, actual->GetBody()->User);
			Assert::AreEqual(data, actual->GetBody()->Data);

			DestroyPacket(actual);
		}



		TEST_METHOD(TestMethod4_TESTSVR017_Server_GetTailFunction)
		{
	
			Packet* actual = dummyPacket();

			unsigned int checksum = 2;

			Assert::AreEqual(checksum, actual->GetTail()->Checksum);

			DestroyPacket(actual);
		}


		TEST_METHOD(TestMethod5_TESTSVR18_Server_CreatePacketFunction)
		{
			Packet* packet = CreatePacket();


			Assert::IsTrue(packet);

			DestroyPacket(packet);
		}

		TEST_METHOD(TestMethod6_TESTSVR19_Server_DestroyPacketFunction)
		{
			Packet* packet = dummyPacket();
			unsigned int expected = 2;

			DestroyPacket(packet);

			Assert::AreNotEqual(expected, packet->GetHead()->Length);

		}

		TEST_METHOD(TestMethod7_TESTSVR20_Server_DisplayFunction)
		{
			Packet* actual = dummyPacket();
			LogIn login = dummyLogin();
			SignUp signin = dummySignin();

			std::stringstream outputStream;

			// Call the Display function with the Packet object and the stringstream
			Display(actual, outputStream, login, signin);

			// Check if the output matches the expected result
			std::string expectedOutput = "Source:  Client\n"
				"Destination: Server\n"
				"Route:  Route1\n"
				"Authorization:  1\n"
				"Length:  12\n"
				"User:  \n" //this will have to be changed when the packet.h changes
				"Data:  data\n"
				"Checksum  2\n";

			Assert::AreEqual(expectedOutput, outputStream.str());

			DestroyPacket(actual);

		}

		TEST_METHOD(TestMethod8_TESTSVR05_Server_Deserializefunction)
		{
			Packet* actual = dummyPacket();
			LogIn login = dummyLogin();
			SignUp signin = dummySignin();

			int size = 0;
			char* serializedBuffer = SerializeData(actual, size);

			Packet* deserializedPacket = CreatePacket();

			Deserialization(deserializedPacket, serializedBuffer, login, signin);

			Assert::AreEqual(actual->GetHead()->Source, deserializedPacket->GetHead()->Source);
			Assert::AreEqual(actual->GetHead()->Destination, deserializedPacket->GetHead()->Destination);
			Assert::AreEqual(actual->GetHead()->Route, deserializedPacket->GetHead()->Route);
			Assert::AreEqual(actual->GetHead()->Authorization, deserializedPacket->GetHead()->Authorization);
			Assert::AreEqual(actual->GetHead()->Length, deserializedPacket->GetHead()->Length);

			Assert::AreEqual(actual->GetBody()->User, deserializedPacket->GetBody()->User);
			Assert::AreEqual(0, strcmp(actual->GetBody()->Data, deserializedPacket->GetBody()->Data));
			Assert::AreEqual(actual->GetTail()->Checksum, deserializedPacket->GetTail()->Checksum);

			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}


		TEST_METHOD(TestMethod9_TESTSVR21_Server_SetHeaderfunction)
		{
			Packet* actual = CreatePacket();
			Packet* tempHeader = dummyPacket();


			SetHeader(actual, tempHeader->GetHead());


			Assert::AreEqual(tempHeader->GetHead()->Source, actual->GetHead()->Source);
			Assert::AreEqual(tempHeader->GetHead()->Destination, actual->GetHead()->Destination);
			Assert::AreEqual(tempHeader->GetHead()->Route, actual->GetHead()->Route);
			Assert::AreEqual(tempHeader->GetHead()->Authorization, actual->GetHead()->Authorization);
			Assert::AreEqual(tempHeader->GetHead()->Length, actual->GetHead()->Length);

			DestroyPacket(actual);
			DestroyPacket(tempHeader);

		}

		TEST_METHOD(TestMethod10_TESTSVR22_Server_SetBodyfunction)
		{
			Packet* actual = CreatePacket();


			unsigned char user = 5;
			char data[] = "data";
			unsigned int dataSize = strlen(data);


			SetBody(actual, user, data, dataSize);

			Assert::AreEqual(actual->GetBody()->User, user);
			Assert::AreEqual(0, strcmp(actual->GetBody()->Data, data));
			Assert::AreEqual(actual->GetHead()->Length, dataSize + 1);

			DestroyPacket(actual);

		}

		TEST_METHOD(TestMethod11_TESTSVR04_Server_Serializefunction)
		{
			Packet* actual = dummyPacket();

			int size = 0;
			char* serializedBuffer = nullptr;
			serializedBuffer = SerializeData(actual, size);

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


		TEST_METHOD(TestMethod13_TESTSVR11_Server_Packet_CheckSumDrop) //failing now need to have function to drop packet that has invalid checksum
		{
			Packet* actual = dummyPacket();
			LogIn login = dummyLogin();
			SignUp signin = dummySignin();

			int size = 0;
			char* serializedBuffer = SerializeData(actual, size);

			Packet* deserializedPacket = CreatePacket();

			Deserialization(deserializedPacket, serializedBuffer, login, signin);

			Assert::AreEqual(deserializedPacket->GetHead()->Source[0], '\0'); // Assuming the source field should be empty if the packet is dropped

			DestroyPacket(actual);
			DestroyPacket(deserializedPacket);

		}

		TEST_METHOD(TestMethod14_TESTSVR12_Server_Serialize_NoBody) //failing now need to have function to drop packet that has invalid checksum
		{
			Packet* actual = dummyPacket();

			int size = 0;
			char* serializedBuffer = SerializeData(actual, size);

			Assert::IsTrue(serializedBuffer != nullptr); // Assuming the source field should be empty if the packet is dropped

			DestroyPacket(actual);

		}

		TEST_METHOD(TestMethod15_TESTSVR13_Server_GetBody) //failing now need to have function to drop packet that has invalid checksum
		{
			Packet* actual = dummyPacket();

			char Data[] = "data";
	

			unsigned char user = 2;


			Assert::AreEqual(actual->GetBody()->User, user); // Assuming the source field should be empty if the packet is dropped
			Assert::AreEqual((string)actual->GetBody()->Data, (string)Data); // Assuming the source field should be empty if the packet is dropped


			DestroyPacket(actual);

		}

		TEST_METHOD(TestMethod16_TESTSVR14_Server_GetTail) //failing now need to have function to drop packet that has invalid checksum
		{
			Packet* actual = dummyPacket();


			Assert::IsTrue(actual->GetTail()->Checksum == 2); // Assuming the source field should be empty if the packet is dropped


			DestroyPacket(actual);

		}

		TEST_METHOD(TestMethod17_TESTSVR_Server_Parseuserinfo) //failing now need to have function to drop packet that has invalid checksum
		{
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

			User user = User().parseUser(userString);

			Assert::AreEqual(user.getEmailAddress(), expectedUser.getEmailAddress());
			Assert::AreEqual(user.getPassword(), expectedUser.getPassword());
			Assert::AreEqual(user.getFirstName(), expectedUser.getFirstName());
			Assert::AreEqual(user.getLastName(), expectedUser.getLastName());
			Assert::AreEqual(user.getProfilePicture(), expectedUser.getProfilePicture());
			Assert::AreEqual(user.getDateCreated(), expectedUser.getDateCreated());
			Assert::AreEqual(user.getAccountStatus(), expectedUser.getAccountStatus());


		}

		TEST_METHOD(TestMethod18_TESTSVR16_Server_ComparePasswords) //failing now need to have function to drop packet that has invalid checksum
		{

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


			bool result = expectedUser.validatePassword(password, expectedUser.getPassword());

			bool expected = true;
			Assert::AreEqual(expected, result);

		}

		TEST_METHOD(TestMethod19_TESTSVR17_Server_HashFunction) //failing now need to have function to drop packet that has invalid checksum
		{

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



			string actual = expectedUser.generateHash(password);


			string expected = "password";
			Assert::AreEqual(expected, actual);

		}



	};
}
