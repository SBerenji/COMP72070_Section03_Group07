#include "pch.h"
#include "CppUnitTest.h"
#include "../TCP_Client/Packet.h"
#include "../TCP_Client/PacketWrapper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" int CloseSocket(SOCKET ClientSocket, closesocketFunc closeFunc = closesocket, WSACleanupFunc wsacleanupfunc = WSACleanup);
extern "C" int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc = send);
extern "C" SOCKET setupConnection2(WSAStartupFunc wsaStartup = WSAStartup, socketFunc socketfunc = socket, connectFunc connectfunc = connect, WSACleanupFunc wsacleanup = WSACleanup);
extern "C" char* SerializeData(Packet * Pkt, int& TotalSize);
extern "C" Packet * CreatePacket();
extern "C" void DestroyPacket(Packet*& Pkt);
extern "C" void FreeBuffer(char*& Buffer);
extern "C" char* AllocateHeaderPtr();
extern "C" void Deserialization(Packet * Pkt, char* src);
extern "C" void SetHeader(Packet * Pkt, void* Head);
extern "C" void SetBody(Packet * Pkt, unsigned char User, char* Data, int DataSize);

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



namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		
		///// <summary>
		///// This test will ensure that the setupConnection2 returns a valid socket
		///// Mock functions are used for this test for functions such as connect which required the presence of a server
		///// </summary>
		TEST_METHOD(setupConnection2_ValidSocket)
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
		TEST_METHOD(setupConnection2_InvalidSocket)
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
		TEST_METHOD(setupConnection2_WSAStartupFailure)
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
		TEST_METHOD(setupConnection2_ConnectFailure)
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
		TEST_METHOD(CloseSocket_ValidSocket)
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
		TEST_METHOD(CloseSocket_InvalidSocket)
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

		TEST_METHOD(CloseSocket_WSALibraryCleaningFailure)
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
		TEST_METHOD(sendData_ValidSocket_SuccessfullSend)
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
		TEST_METHOD(sendData_InvalidSocket_UnsuccessfulSend)
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
		///// This test ensures that the CreatePacket function successfully instantiates the Packet class 
		///// </summary>

		TEST_METHOD(CreatePacket_Success)
		{
			// Act
			Packet* packet = CreatePacket();

			// Assert
			Assert::IsNotNull(packet);

			// Clean up
			delete packet;
		}
		
		

		///// <summary>
		///// This test ensures that the DestroyPacket function successfully deletes the packet created from the memory
		///// </summary>

		TEST_METHOD(DestroyPacket_Success)
		{
			// Arrange
			Packet* Pkt = new Packet();

			Pkt->GetBody()->Data = nullptr;


			memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
			memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
			memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


			Assert::IsNotNull(Pkt);

			// Act
			DestroyPacket(Pkt);


			// Assert
			Assert::IsNull(Pkt);
		}


		///// <summary>
		///// This test ensures that the FreeBuffer function successfully deletes the buffer pointer created and make sures there is no dangling pointer
		///// </summary>

		TEST_METHOD(FreeBuffer_Success)
		{
			char* TxBuffer = new char[10];
			std::string test = "Testing";

			memcpy(TxBuffer, test.c_str(), test.length());

			// Clean up
			FreeBuffer(TxBuffer);

			Assert::IsNull(TxBuffer);
		}
		
		
		TEST_METHOD(AllocateHeaderPtr_Success)
		{
		
			// Act
			char* result = AllocateHeaderPtr();

			// Assert
			Assert::IsNotNull(result);
		}


		///// <summary>
		///// This test ensures that the Deserialization function successfully deserializes the serialized data passed to it.
		///// </summary>
		TEST_METHOD(Deserialization_ValidData)
		{
			// Arrange
			Packet* Pkt = new Packet(); // create a new packet:


			Pkt->GetBody()->Data = nullptr;


			memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
			memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
			memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));


			Pkt->GetBody()->Data = new char[10];
			Pkt->GetBody()->Data = "Testing";
			Pkt->GetHead()->Length = 10; // Set the length

			int totalSize = 0;

			Pkt->GetTail()->Checksum = 0xFF00FF00;

			totalSize = sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length + sizeof(*(Pkt->GetTail()));

			Pkt->GetTxBuffer() = new char[totalSize];


			memcpy(Pkt->GetTxBuffer(), Pkt->GetHead(), sizeof(*(Pkt->GetHead())));

			memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())), &(Pkt->GetBody()->User), sizeof(Pkt->GetBody()->User));
			memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), Pkt->GetBody()->Data, Pkt->GetHead()->Length);
			memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length, Pkt->GetTail(), sizeof(*(Pkt->GetTail())));

			Packet* packet = new Packet(); // create a new packet:


			packet->GetBody()->Data = nullptr;


			memset(packet->GetHead(), 0, sizeof(*(packet->GetHead())));
			memset(&(packet->GetBody()->User), 0, sizeof(packet->GetBody()->User));
			memset(&(packet->GetTail()->Checksum), 0, sizeof(packet->GetTail()->Checksum));

			// Act
			Deserialization(packet, Pkt->GetTxBuffer()); // Deserializing the data into the new packet

			// Assert 
			Assert::AreEqual(10, (int)Pkt->GetHead()->Length);
			Assert::IsNotNull(Pkt->GetBody()->Data);
		}



		///// <summary>
		///// This test ensures that the SerializeData function successfully deserializes the serialized data passed to it.
		///// </summary>
		TEST_METHOD(SerializeData_ValidData)
		{
			// Arrange
			Packet* Pkt = new Packet(); // create a new packet:


			Pkt->GetBody()->Data = nullptr;


			memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
			memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
			memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));

			// initializing the packet body
			Pkt->GetBody()->Data = new char[10];
			Pkt->GetBody()->Data = "Testing";
			Pkt->GetHead()->Length = 10; // Set the length element of the header

			int totalSize = 0;

			// Act
			char* TxBuffer = nullptr;
			TxBuffer = SerializeData(Pkt, totalSize);

			// Assert 
			Assert::IsNotNull(TxBuffer);
		}


		///// <summary>
		///// This test ensures that the setHeader function can properly set the header of the packet
		///// </summary>




		///// <summary>
		///// This test ensures that the setBody function can properly set the body of the packet
		///// </summary>


	};

}