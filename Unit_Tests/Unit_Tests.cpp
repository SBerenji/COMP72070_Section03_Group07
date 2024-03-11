#include "pch.h"
#include "CppUnitTest.h"
#include "../TCP_Client/Packet.h"
#include "../TCP_Client/PacketWrapper.h"
//extern "C" typedef int SendFunction(SOCKET, const char*, int, int);


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" int CloseSocket(SOCKET ClientSocket);
//extern "C" int sendData(SOCKET ClientSocket, char* TxBuffer, int totalSize, SendFunction sendFunc = send);
extern "C" SOCKET setupConnection2(WSAStartupFunc wsaStartup = WSAStartup, socketFunc socketfunc = socket, connectFunc connectfunc = connect);


// This is the Mock function for sendData in order to simulate sucessful sending
// The actual send function returns the total size sent upon successful sending
int MockSendSuccess(SOCKET ClientSocket, const char* TxBuffer, int totalSize, int flag) {
	return totalSize; // Return the number of bytes sent
}

// This is the Mock function for sendData in order to simulate UNsucessful sending
// The actual send function returns -1 upon UNsuccessful sending
int MockSendUnsuccessful(SOCKET ClientSocket, const char* TxBuffer, int totalSize, int flag) {
	return -1; 
}




//  *****  Creating mock functions for the setupConnection2() function   ******

// WSAStartup success:
int MockWSAStartupSuccess(WORD wVersionRequested, LPWSADATA lpWSAData) {
	return 0; // simulate success
}


// WSAStartup failure:
int MockWSAStartupFailure(WORD wVersionRequested, LPWSADATA lpWSAData) {
	return 2; // simulate failure
}



// Socket success:
SOCKET MockSocketSuccess(int, int, int)
{
	return 1;  //simulate success (anything but -1)
}


// Socket failure:
SOCKET MockSocketFailure(int, int, int)
{
	return -1;  //simulate failure (must return -1)
}


// connect success:
int MockConnectSuccess(SOCKET, const struct sockaddr*, int)
{
	return 1; //simulate success (anything but -1)
}


// connect failure:
int MockConnectFailure(SOCKET, const struct sockaddr*, int)
{
	return -1; //simulate failure (must be -1)
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



			// Act
			SOCKET clientSocket = setupConnection2(MockWSAStartupSuccess, MockSocketSuccess, MockConnectSuccess);


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

				

			// Act
			SOCKET clientSocket = setupConnection2(MockWSAStartupSuccess, MockSocketFailure, MockConnectSuccess);


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



			// Act
			SOCKET clientSocket = setupConnection2(MockWSAStartupFailure, MockSocketFailure, MockConnectSuccess);


			// Assert 
			Assert::AreEqual((SOCKET)0, clientSocket);
		}




		///// <summary>
		///// This test will ensure that the setupConnection2 returns 0 (exits) if the connect function return -1 and is unable to connect to the server
		///// Mock functions are used for this test for functions such as connect which required the presence of a server
		///// </summary>
		TEST_METHOD(setupConnection2_ConnectFailure)
		{

			// Arrange
			WSAStartupFunc wsastartupfunc = MockWSAStartupFailure;
			socketFunc socketfunc = MockSocketFailure;
			connectFunc connectfunc = MockConnectFailure;



			// Act
			SOCKET clientSocket = setupConnection2(MockWSAStartupFailure, MockSocketFailure, MockConnectFailure);


			// Assert 
			Assert::AreEqual((SOCKET)0, clientSocket);
		}




		/// <summary>
		/// This test will ensure that the CloseSocket funciton returns 1 and handles all the closing and cleaning properly
		/// </summary>
		//TEST_METHOD(CloseSocket_ValidSocket)
		//{
		//	// Arrange
		//	WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
		//	socketFunc socketfunc = MockSocketSuccess;
		//	connectFunc connectfunc = MockConnectSuccess;


		//	SOCKET clientSocket = setupConnection2(MockWSAStartupSuccess, MockSocketSuccess, MockConnectSuccess);

		//	Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

		//	// Act
		//	int result = CloseSocket(clientSocket);

		//	// Assert
		//	Assert::AreEqual(1, result);
		//}


		/// <summary>
		/// This test will ensure that the CloseSocket funciton returns 0 if the socket is not valid since it is not able to close and clean the WSA library properly
		/// </summary>
		//TEST_METHOD(CloseSocket_InvalidSocket)
		//{
		//	// Arrange
		//	SOCKET clientSocket = INVALID_SOCKET;

		//	// Act
		//	int result = CloseSocket(clientSocket);

		//	// Assert
		//	Assert::AreEqual(0, result);
		//}


		///// <summary>
		///// This test ensures that the sendData function (here using a mock function) will return 1 in case of a successfull send
		///// </summary>
		//TEST_METHOD(sendData_ValidSocket_SuccessfullSend)
		//{
		//	// Arrange
		//	SOCKET clientSocket = setupConnection2();
		//	Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

		//	char TxBuffer[20] = "This is for testing";
		//	int totalSize = sizeof(TxBuffer);


		//	// Act
		//	int result = sendData(clientSocket, TxBuffer, totalSize, &MockSendSuccess);

		//	// Assert
		//	Assert::AreEqual(1, result);
		//}



		///// <summary>
		///// This test ensures that the sendData function (here a mock function) will return -1 in case of an unsuccessfull send
		///// </summary>
		//TEST_METHOD(sendData_InvalidSocket_UnsuccessfulSend)
		//{
		//	// Arrange
		//	SOCKET clientSocket = setupConnection2();
		//	Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

		//	char TxBuffer[20] = "This is for testing";
		//	int totalSize = sizeof(TxBuffer);


		//	// Act
		//	int result = sendData(clientSocket, TxBuffer, totalSize, MockSendUnsuccessful);

		//	// Assert
		//	Assert::AreEqual(-1, result);
		//}

	};

}
