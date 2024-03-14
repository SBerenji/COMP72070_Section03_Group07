#include "pch.h"
#include "CppUnitTest.h"
#include "../TCP_Client/Packet.h"
#include "../TCP_Client/PacketWrapper.h"
//extern "C" typedef int SendFunction(SOCKET, const char*, int, int);


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
		///// This test ensures that the sendData function (here a mock function) will return 0 in case of an unsuccessfull send
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

	};

}
