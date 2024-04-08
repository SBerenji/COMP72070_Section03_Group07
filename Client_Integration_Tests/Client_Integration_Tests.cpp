#include "pch.h"
#include "CppUnitTest.h"
#include "../TCP_Client/Packet.h"
#include "../TCP_Client/PacketWrapper.h"
#include "string.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" int CloseSocketFunc(SOCKET ClientSocket, closesocketFunc closeFunc = closesocket, WSACleanupFunc wsacleanupfunc = WSACleanup);
extern "C" int sendDataFunc(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc = send);
extern "C" int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize);
extern "C" SOCKET setupConnection(WSAStartupFunc wsaStartup = WSAStartup, socketFunc socketfunc = socket, connectFunc connectfunc = connect, WSACleanupFunc wsacleanup = WSACleanup, int socketType = SOCK_STREAM);
extern "C" char* SerializeData(Packet * Pkt, int& TotalSize);
extern "C" Packet * CreatePacket();
extern "C" void DestroyPacket(Packet * &Pkt);
extern "C" void FreeBuffer(char*& Buffer);
extern "C" void SetHeader(Packet * Pkt, void* Head);
extern "C" void SetBody(Packet * Pkt, unsigned int User, char* Data, int DataSize);
extern "C" int recvDataFunc(SOCKET ClientSocket, char*** RxBuffer, int RxBufferSize, RecvFunction recvFunc = recv);
extern "C" char* AllocateHeaderPtr();
extern "C" char* AllocateLoginPtr();
extern "C" char* AllocateHeapMemory(int size);
extern "C" void CopyBufferToHeap(char* heapBuffer, char* srcBuffer, int size);
extern "C" char* AllocateSignupPtr(int imageSize);
extern "C" char* AllocateListingPtr(int imageSize);
extern "C" void DeserializePostCountBuffer(Packet * pkt, char* src, int& numberOfPosts);



int GetSocketType(SOCKET socket)
{
	int type;
	int length = sizeof(int);

	getsockopt(socket, SOL_SOCKET, SO_TYPE, (char*)&type, &length);

	if (type == SOCK_STREAM)
		return 1;

	else
		return -1;
}


namespace IntegrationTests
{
	TEST_CLASS(IntegrationTests)
	{
	public:


		///// <summary>
		 ///// This test ensures that the connection between the client connects to the server using a TCP protocol
		 ///// </summary> 
		TEST_METHOD(TEST_CLI_23_TCP_Confirmation)
		{
			// Arrange

			SOCKET clientSocket;

			// Act
			clientSocket = setupConnection();

			// Assert
			Assert::AreNotEqual((SOCKET)0, clientSocket);  // check if socket creation was successful

			// Get the socket type of the created socket
			int socketType = GetSocketType(clientSocket);

			// Assert 
			Assert::AreEqual(SOCK_STREAM, socketType);   // this is to assert that the socket created is of type SOCK_STREAM which indicated TCP connection
		}




		/// <summary>
		/// This test ensures that the sendData function of client can send a packet without any body to the server after estalishing a connection with the server
		///// </summary>
		TEST_METHOD(TEST_CLI_14_SendPacketNoBody)
		{
			// Arrange
			SOCKET clientSocket = setupConnection();
			std::string serverAddress = "127.0.0.1";
			std::string clientAddress = "127.0.0.1";

			Packet* packet = CreatePacket();
			memcpy(packet->GetHead()->Source, serverAddress.c_str(), serverAddress.length());

			memcpy(packet->GetHead()->Destination, clientAddress.c_str(), clientAddress.length());

			std::string route = "LOGIN";
			memcpy(packet->GetHead()->Route, route.c_str(), route.length());

			packet->GetHead()->Authorization = true;

			packet->GetHead()->Length = 0;

			int totalsize = sizeof(*(packet->GetHead()));

			char* TxBuffer = new char[totalsize];

			memset(TxBuffer, 0, totalsize);

			memcpy(TxBuffer, packet->GetHead(), totalsize);

			// Act
			int sendDataResult = sendData(clientSocket, TxBuffer, totalsize);

			FreeBuffer(TxBuffer);

			DestroyPacket(packet);

			// Assert
			Assert::AreEqual(1, sendDataResult);

		}




		/// <summary>
		/// This test ensures that the sendData function of client can send a complete packet of log in information to the server after estalishing a connection with the server
		///// </summary>
		TEST_METHOD(TEST_CLI_25_SendPacketWithLogInBody)
		{
			// Arrange

			SOCKET clientSocket = setupConnection();
			std::string serverAddress = "127.0.0.1";
			std::string clientAddress = "127.0.0.1";

			Packet* packet = CreatePacket();

			// Set source, destination, and route in the packet header
			memcpy(packet->GetHead()->Source, serverAddress.c_str(), serverAddress.length());
			memcpy(packet->GetHead()->Destination, clientAddress.c_str(), clientAddress.length());
			std::string route = "LOGIN";
			memcpy(packet->GetHead()->Route, route.c_str(), route.length());

			// Set authorization flag in the packet header
			packet->GetHead()->Authorization = true;

			// Initialize login information for the body
			LogIn login;
			std::string email = "berenji@gmail.com";
			memcpy(login.username, email.c_str(), email.length());
			std::string password = "student123";
			memcpy(login.password, password.c_str(), password.length());

			// Set body data in the packet
			packet->GetBody()->User = 100; 
			packet->GetBody()->Data = new char[sizeof(login)];

			// Calculate total size of the packet
			int totalSize = sizeof(*(packet->GetHead())) + sizeof(packet->GetBody()->User) + sizeof(login);

			// Serialize packet data into TxBuffer
			char* TxBuffer = SerializeData(packet, totalSize);

			// Act
			int sendDataResult = sendData(clientSocket, TxBuffer, totalSize);

			// Clean up
			FreeBuffer(TxBuffer);
			DestroyPacket(packet);

			// Assert
			Assert::AreEqual(1, sendDataResult);

		}



		///// <summary>
		///// This test ensures that the setupConnection returns a clientsocket upon successfully establishing a connection with the server
		//// This test should not pass whithout running the server
		///// </summary>
		// False positive

		TEST_METHOD(TEST_CLI_25_succesfulConnectionToServer)
		{
			// Arrange
			SOCKET clientSocket;

			// Act

			clientSocket = setupConnection();

			// Assert 
			Assert::AreNotEqual(INVALID_SOCKET, clientSocket);
		}


		/////// <summary>
		/////// This test ensures that the setupConnection returns 0 if unsuccesful in establishing a connection to the server 
		/////// Server will not run while executing this test in order to show the functionality of the function
		/////// </summary>
		TEST_METHOD(TEST_CLI_25_succesfulConnectionToServer_true_negative)
		{
			// Arrange
			SOCKET clientSocket;

			// Act

			clientSocket = setupConnection();

			// Assert 
			Assert::AreNotEqual((SOCKET)0, clientSocket);
		}


		// test the acutal funcitonality of the closeSocket function



















	};
}
