#include "pch.h"
//#include "CppUnitTest.h"
//#include "../TCP_Client/Packet.h"
//#include "../TCP_Client/PacketWrapper.h"
//#include "string.h"
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//
//extern "C" int CloseSocketFunc(SOCKET ClientSocket, closesocketFunc closeFunc = closesocket, WSACleanupFunc wsacleanupfunc = WSACleanup);
//extern "C" int sendDataFunc(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc = send);
//extern "C" int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize);
//extern "C" SOCKET setupConnection(WSAStartupFunc wsaStartup = WSAStartup, socketFunc socketfunc = socket, connectFunc connectfunc = connect, WSACleanupFunc wsacleanup = WSACleanup, int socketType = SOCK_STREAM);
//extern "C" char* SerializeData(Packet * Pkt, int& TotalSize);
//extern "C" Packet * CreatePacket();
//extern "C" void DestroyPacket(Packet * &Pkt);
//extern "C" void FreeBuffer(char*& Buffer);
//extern "C" char* AllocateHeaderPtr();
//extern "C" void Deserialization(Packet * Pkt, char* src);
//extern "C" void SetHeader(Packet * Pkt, void* Head);
//extern "C" void SetBody(Packet * Pkt, unsigned int User, char* Data, int DataSize);
//extern "C" int recvDataFunc(SOCKET ClientSocket, char* RxBuffer, int RxBufferSize, RecvFunction recvFunc = recv);
//extern "C" int recvData(SOCKET ClientSocket, char* RxBuffer, int RxBufferSize);
//
//namespace IntegrationTests
//{
//	TEST_CLASS(IntegrationTests)
//	{
//	public:
//
//
//		///// <summary>
//		///// This test ensures that the Deserialization function successfully deserializes the serialized data passed to it.
//		////  In order to test this funciton 
//		///// </summary>
//		TEST_METHOD(Serialization_and_Deserialiation)
//		{
//			// Arrange
//			Packet* packet = CreatePacket();
//			packet->GetBody()->Data = new char[10]; // Assuming data length is 10
//			packet->GetBody()->Data = "Testing";
//			packet->GetHead()->Length = 10; // Set the length of the head
//
//			int totalSize = 0;
//			char* serializedData = SerializeData(packet, totalSize); // Serialize the packet to get the serialized data
//
//
//			Packet* deserializedPacket = CreatePacket();  // Create a new packet to deserialize into
//
//			Deserialization(deserializedPacket, serializedData); // Deserializing the data into the new packet
//
//			// Assert 
//			Assert::AreEqual(10, (int)deserializedPacket->GetHead()->Length);
//			Assert::IsNotNull(deserializedPacket->GetBody()->Data);
//
//			// Clean up
//			DestroyPacket(packet);
//			FreeBuffer(serializedData);
//			DestroyPacket(deserializedPacket);
//		}
//
//
//
//		///// <summary>
//		///// This test ensures that the sendData function can send a packet without any body to the server after estalishing a connection with the server
//		////  A true positive test
//		///// </summary>
//		TEST_METHOD(SendDataSuccessful_noPacketBody)
//		{
//			// Arrange
//			SOCKET clientSocket = setupConnection();
//			std::string serverAddress = "127.0.0.1";
//
//
//			Packet* packet = CreatePacket();
//			memcpy(packet->GetHead()->Source, serverAddress.c_str(), serverAddress.length());
//
//			std::string clientAddress = "127.0.0.1";
//			memcpy(packet->GetHead()->Destination, clientAddress.c_str(), clientAddress.length());
//
//			std::string route = "LOGIN";
//			memcpy(packet->GetHead()->Route, route.c_str(), route.length());
//
//			packet->GetHead()->Authorization = true;
//
//			packet->GetHead()->Length = 0;
//
//			int totalsize = sizeof(*(packet->GetHead()));
//
//			char* TxBuffer = new char[totalsize];
//
//			memset(TxBuffer, 0, totalsize);
//
//			memcpy(TxBuffer, packet->GetHead(), totalsize);
//
//
//
//			// Act
//			int sendDataResult = sendData(clientSocket, TxBuffer, totalsize);
//
//			FreeBuffer(TxBuffer);
//
//			DestroyPacket(packet);
//
//			// Assert
//			Assert::AreEqual(1, sendDataResult);
//
//		}
//
//
//
//		// test the acutal funcitonality of the sendData function
//		//TEST_METHOD(SendDataSuccessful_PackeetWithBody)
//		//{
//		//	// Arrange
//		//	SOCKET clientSocket = setupConnection2();
//		//	std::string serverAddress = "127.0.0.1";
//
//
//		//	Packet* packet = CreatePacket();
//		//	memcpy(packet->GetHead()->Source, serverAddress.c_str(), serverAddress.length());
//
//		//	std::string clientAddress = "127.0.0.1";
//		//	memcpy(packet->GetHead()->Destination, clientAddress.c_str(), clientAddress.length());
//
//		//	std::string route = "LOGIN";
//		//	memcpy(packet->GetHead()->Route, route.c_str(), route.length());
//
//		//	packet->GetHead()->Authorization = true;
//
//		//	packet->GetHead()->Length = 0;
//
//
//
//
//
//
//		//	int totalsize = sizeof(*(packet->GetHead()));
//
//
//
//
//
//
//
//		//	char* TxBuffer = new char[totalsize];
//
//
//
//
//
//		//	// Act
//		//	int sendDataResult = sendData(clientSocket, TxBuffer, totalsize);
//
//		//	FreeBuffer(TxBuffer);
//
//		//	DestroyPacket(packet);
//
//		//	// Assert
//		//	Assert::AreEqual(1, sendDataResult);
//
//		//}
//
//
//
//
//		///// <summary>
//		///// This test ensures that the setupConnection2 returns a clientsocket upon successfully establishing a connection with the server
//		//// This test should not pass whithout running the server
//		///// </summary>
//
//
//		/// <summary>
//		///  ****************************** needs fixing, currently a false positive test
//		/// </summary>
//		TEST_METHOD(SetUpConnection2_Successful)
//		{
//			// Arrange
//			SOCKET clientSocket;
//
//			// Act
//
//			clientSocket = setupConnection();
//
//			// Assert 
//			Assert::AreNotEqual(INVALID_SOCKET, clientSocket);
//		}
//
//
//
//		///// <summary>
//		///// This test ensures that the setupConnection2 returns 0 if unsuccesful in establishing a connection to the server 
//		///// Server will not run while executing this test in order to show the functionality of the function
//		///// </summary>
//		TEST_METHOD(SetUpConnection2_Unsuccessful)
//		{
//			// Arrange
//			SOCKET clientSocket;
//
//			// Act
//
//			clientSocket = setupConnection();
//
//			// Assert 
//			Assert::AreEqual((SOCKET)0, clientSocket);
//		}
//
//
//		// test the acutal funcitonality of the closeSocket function
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//	};
//}
