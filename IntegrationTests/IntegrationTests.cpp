#include "pch.h"
#include "CppUnitTest.h"
#include "../TCP_Client/Packet.h"
#include "../TCP_Client/PacketWrapper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" int CloseSocket(SOCKET ClientSocket, closesocketFunc closeFunc = closesocket, WSACleanupFunc wsacleanupfunc = WSACleanup);
extern "C" int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc = send);
extern "C" SOCKET setupConnection2(WSAStartupFunc wsaStartup = WSAStartup, socketFunc socketfunc = socket, connectFunc connectfunc = connect, WSACleanupFunc wsacleanup = WSACleanup);
extern "C" char* SerializeData(Packet * Pkt, int& TotalSize);
extern "C" void Deserialization(Packet * Pkt, char* src);
extern "C" Packet * CreatePacket();
extern "C" void DestroyPacket(Packet * &Pkt);
extern "C" void FreeBuffer(char*& Buffer);
extern "C" char* AllocateHeaderPtr();

namespace IntegrationTests
{
	TEST_CLASS(IntegrationTests)
	{
	public:
		

		///// <summary>
		///// This test ensures that the Deserialization function successfully deserializes the serialized data passed to it.
		////  In order to test this funciton 
		///// </summary>
		TEST_METHOD(Serialization_and_Deserialiation)
		{
			// Arrange
			Packet* packet = CreatePacket();
			packet->GetBody()->Data = new char[10]; // Assuming data length is 10
			packet->GetBody()->Data = "Testing";
			packet->GetHead()->Length = 10; // Set the length

			int totalSize = 0;  
			char* serializedData = SerializeData(packet, totalSize); // Serialize the packet to get the serialized data

			
			Packet* deserializedPacket = CreatePacket();  // Create a new packet to deserialize into

			Deserialization(deserializedPacket, serializedData); // Deserializing the data into the new packet

			// Assert 
			Assert::AreEqual(10, (int)deserializedPacket->GetHead()->Length);
			Assert::IsNotNull(deserializedPacket->GetBody()->Data);

			// Clean up
			DestroyPacket(packet);
			FreeBuffer(serializedData);
			DestroyPacket(deserializedPacket);
		}



		// test the acutal funcitonality of the sendData function
		// test the acutal funcitonality of the closeSocket function
		


















	};
}
