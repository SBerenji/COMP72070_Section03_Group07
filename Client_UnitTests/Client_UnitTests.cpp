#include "pch.h"
#include "CppUnitTest.h"
#include "../TCP_Client/Packet.h"
#include "../TCP_Client/PacketWrapper.h"
#include "../TCP_Client/Logging.h"
#include <iostream>
#include <cstring>
#include <sys/types.h>

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
extern "C" void SetHeader(Packet * Pkt, void* Head);
extern "C" void SetBody(Packet * Pkt, unsigned int User, char* Data, int DataSize);
extern "C" void DeserializationWithoutTail(Packet * Pkt, char* src);
extern "C" unsigned int DeserializeClientID(char* src);
extern "C" unsigned int DeserializeHeaderLengthMember(char* RxBuffer);
extern "C" void DeserializeHeader(Packet * pkt, char**** TxBuffer);
extern "C" void DeserializePostCountBuffer(Packet * pkt, char* src, int& numberOfPosts);
extern "C" unsigned int DeserializeImageLength(char* RxBuffer, int offset);
extern "C" unsigned int CalculateChecksum();
extern "C" char* SerializeMyPostCountData(Packet * Pkt, int& totalSize);
extern "C" char* SerializeData(Packet * Pkt, int& TotalSize);
extern "C" char* SerializeHeaderOnlyPkt(Packet * Pkt, int& TotalSize);
extern "C" void DeserializeHeader(Packet * pkt, char**** TxBuffer);
extern "C" void DeserializePostCountBuffer(Packet * pkt, char* src, int& numberOfPosts);

//  *****  Creating mock functions for the sendData() function   ******

// The actual send function returns the total size sent upon successful sending
int MockSendSuccess(SOCKET ClientSocket, const char* TxBuffer, int totalSize, int flag)
{
    return totalSize; // Return the number of bytes sent
}

// This is the Mock function for sendData in order to simulate UNsucessful sending
// The actual send function returns -1 upon UNsuccessful sending
int MockSendFailure(SOCKET ClientSocket, const char* TxBuffer, int totalSize, int flag)
{
    return -1;
}

//  *****  Creating mock functions for the setupConnection() function   ******

// WSAStartup success:
int MockWSAStartupSuccess(WORD wVersionRequested, LPWSADATA lpWSAData)
{
    return 0; // simulate success
}

// WSAStartup failure:
int MockWSAStartupFailure(WORD wVersionRequested, LPWSADATA lpWSAData)
{
    return 1; // simulate failure
}

// Socket success:
SOCKET MockSocketSuccess(int, int, int)
{
    return 1; // simulate success (anything but -1)
}

// Socket failure:
SOCKET MockSocketFailure(int, int, int)
{
    return INVALID_SOCKET; // Simulate failure (must return INVALID_SOCKET)
}

// connect success:
int MockConnectSuccess(SOCKET, const struct sockaddr*, int)
{
    return 1; // simulate success (anything but -1)
}

// connect failure:
int MockConnectFailure(SOCKET, const struct sockaddr*, int)
{
    return SOCKET_ERROR; // simulate failure (must be -1)
}

//****    Mock funciton for the CloseSocket function    *****
int MockCloseSocketSuccess(SOCKET)
{
    return 0; // This function will return 0 upon success
}

int MockCloseSocketFailure(SOCKET)
{
    return SOCKET_ERROR; // This function will return 0 upon success
}

int MockWSACleanupSuccess()
{
    return 0; // This function will return 0 upon success
}

int MockWSACleanupFailure()
{
    return SOCKET_ERROR; // This function will return SOCKET_ERROR upon failure
}

//  *****  Creating mock functions for the recvDataFunc() function   ******
int MockRecvFailure(SOCKET s, char* buf, int len, int flags)
{
    return -1; // This function will return -1 upon failure
}

int MockRecvSuccess(SOCKET s, char* buf, int len, int flags)
{
    return 1; // This function will return 1 upon success
}



namespace ClientUnitTests
{
	TEST_CLASS(ClientUnitTests)
	{
	public:


        ///// <summary>
       ///// This test ensures that the recvDataFunc function will return 1 upon successfully receiving data
       ///// </summary> 
        TEST_METHOD(TEST_CLI_16_RecvPacket_Success)
        {

            WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
            socketFunc socketfunc = MockSocketSuccess;
            connectFunc connectfunc = MockConnectSuccess;
            WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;
            RecvFunction recvFunction = MockRecvSuccess;

            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc, SOCK_STREAM);

            Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

            char* testBuff = nullptr;

            char** RxBuffer = &testBuff;
            int totalSize = sizeof(RxBuffer);

            // Act
            int result = recvDataFunc(clientSocket, &RxBuffer, totalSize, recvFunction);

            // Assert
            Assert::AreEqual(1, result);
        }


        ///// <summary>
      ///// This test ensures that the recvDataFunc function will return 0 upon unsuccessfully receiving data
      ///// </summary> 
        TEST_METHOD(TEST_CLI_17_RecvPacket_Failure)
        {
            // Arrange
            WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
            socketFunc socketfunc = MockSocketSuccess;
            connectFunc connectfunc = MockConnectSuccess;
            WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;
            RecvFunction recvFunction = MockRecvFailure;

            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc, SOCK_STREAM);

            Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

            char* testBuff = nullptr;

            char** RxBuffer = &testBuff;
            int totalSize = sizeof(RxBuffer);

            // Act
            int result = recvDataFunc(clientSocket, &RxBuffer, totalSize, recvFunction);

            // Assert
            Assert::AreEqual(0, result);
        }




        ///// <summary>
        ///// This test will ensure that the setupConnection2 returns a valid socket
        ///// Mock functions are used for this test for functions such as connect which required the presence of a server
        ///// </summary>
        TEST_METHOD(TEST_CLI_05_setupConnection_ValidSocket) {

            // Arrange
            WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
            socketFunc socketfunc = MockSocketSuccess;
            connectFunc connectfunc = MockConnectSuccess;
            WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;

            // Act
            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc, SOCK_STREAM);

            // Assert
            Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

        }

        ///// <summary>
        ///// This test will ensure that the setupConnection2 returns 0 if the socket function returns an invalid socket (INVALID_SOCKET)
        ///// Mock functions are used for this test for functions such as connect which required the presence of a server
        ///// </summary>
        TEST_METHOD(TEST_CLI_06_setupConnection_InvalidSocket)
        {

            // Arrange
            WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
            socketFunc socketfunc = MockSocketFailure;
            connectFunc connectfunc = MockConnectSuccess;
            WSACleanupFunc wsacleanup = MockWSACleanupSuccess;

            // Act
            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanup, SOCK_STREAM);

            // Assert
            Assert::AreEqual((SOCKET)0, clientSocket);
        }

        ///// <summary>
        ///// This test will ensure that the setupConnection2 returns 0 (exits) if the WSAStartup function does not return 0
        ///// Mock functions are used for this test for functions such as WSAStartup
        ///// </summary>
        TEST_METHOD(TEST_CLI_06_setupConnection_WSAStartupFailure)
        {

            // Arrange
            WSAStartupFunc wsastartupfunc = MockWSAStartupFailure;
            socketFunc socketfunc = MockSocketSuccess;
            connectFunc connectfunc = MockConnectSuccess;
            WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;

            // Act
            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc, SOCK_STREAM);

            // Assert
            Assert::AreEqual((SOCKET)0, clientSocket);
        }

        ///// <summary>
        ///// This test will ensure that the setupConnection2 returns 0 (exits) if the connect function return SOCKET_ERROR and is unable to connect to the server
        ///// Mock functions are used for this test for functions such as connect which required the presence of a server
        ///// </summary>
        TEST_METHOD(TEST_CLI_06_setupConnection_ConnectFailure)
        {

            // Arrange
            WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
            socketFunc socketfunc = MockSocketSuccess;
            connectFunc connectfunc = MockConnectFailure;
            WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;

            // Act
            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc, SOCK_STREAM);

            // Assert
            Assert::AreEqual((SOCKET)0, clientSocket);
        }

        /// <summary>
        /// This test will ensure that the CloseSocket funciton returns 1 upon success and handles all the closing and cleaning properly
        /// </summary>
        TEST_METHOD(TEST_CLI_07_CloseSocket_ValidSocket)
        {
            // Arrange
            WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
            socketFunc socketfunc = MockSocketSuccess;
            connectFunc connectfunc = MockConnectSuccess;
            WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;

            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc, SOCK_STREAM);

            Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

            closesocketFunc closeMocFunc = MockCloseSocketSuccess;

            // Act
            int result = CloseSocketFunc(clientSocket, closeMocFunc, wsacleanupfunc);

            // Assert
            Assert::AreEqual(1, result);
        }

        /// <summary>
        /// This test will ensure that the CloseSocket funciton returns 0 if the socket is not able to connect to the server
        /// close the socket properly
        /// </summary>
        TEST_METHOD(TEST_CLI_09_CloseSocket_InvalidSocket)
        {
            // Arrange
            WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
            socketFunc socketfunc = MockSocketSuccess;
            connectFunc connectfunc = MockConnectFailure;
            WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;

            closesocketFunc closeMocFunc = MockCloseSocketFailure;

            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc, SOCK_STREAM);

            Assert::AreEqual((SOCKET)0, clientSocket);

            // Act
            int result = CloseSocketFunc(clientSocket, closeMocFunc, wsacleanupfunc);

            // Assert
            Assert::AreEqual(0, result);
        }

        /// <summary>
        /// This test will ensure that the CloseSocket funciton returns 2 if there is a problem is cleaning the WSA library
        /// </summary>

        TEST_METHOD(TEST_CLI_09_CloseSocket_WSALibraryCleaningFailure)
        {
            // Arrange
            WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
            socketFunc socketfunc = MockSocketSuccess;
            connectFunc connectfunc = MockConnectSuccess;
            WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;

            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc, SOCK_STREAM);

            Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

            closesocketFunc closeMocFunc = MockCloseSocketSuccess;
            WSACleanupFunc wsacleanupfuncfailure = MockWSACleanupFailure;

            // Act
            int result = CloseSocketFunc(clientSocket, closeMocFunc, wsacleanupfuncfailure);

            // Assert
            Assert::AreEqual(2, result);
        }

        ///// <summary>
        ///// This test ensures that the sendData function (here using a mock function) will return 1 in case of a successfull send
        ///// </summary>
        TEST_METHOD(TEST_CLI_12_sendDataFunc_ValidSocket_SuccessfullSend)
        {
            // Arrange

            WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
            socketFunc socketfunc = MockSocketSuccess;
            connectFunc connectfunc = MockConnectSuccess;
            WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;
            SendFunction sendfunc = MockSendSuccess;

            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc, SOCK_STREAM);

            Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

            char TxBuffer[20] = "This is for testing";
            int totalSize = sizeof(TxBuffer);

            // Act
            int result = sendDataFunc(clientSocket, TxBuffer, totalSize, sendfunc);

            // Assert
            Assert::AreEqual(1, result);
        }

        ///// <summary>
        ///// This test ensures that the sendData function will return 0 in case of an unsuccessfull send
        ///// </summary>
        TEST_METHOD(TEST_CLI_13_sendDataFunc_ValidSocket_UnsuccessfulSend)
        {
            // Arrange

            WSAStartupFunc wsastartupfunc = MockWSAStartupSuccess;
            socketFunc socketfunc = MockSocketSuccess;
            connectFunc connectfunc = MockConnectSuccess;
            WSACleanupFunc wsacleanupfunc = MockWSACleanupSuccess;
            SendFunction sendfunc = MockSendFailure;

            SOCKET clientSocket = setupConnection(wsastartupfunc, socketfunc, connectfunc, wsacleanupfunc, SOCK_STREAM);

            Assert::AreNotEqual(INVALID_SOCKET, clientSocket);

            char TxBuffer[20] = "This is for testing";
            int totalSize = sizeof(TxBuffer);

            // Act
            int result = sendDataFunc(clientSocket, TxBuffer, totalSize, sendfunc);

            // Assert
            Assert::AreEqual(0, result);
        }




        ///// <summary>
        ///// This test ensures that the CreatePacket function successfully instantiates the Packet class
        ///// </summary>

        TEST_METHOD(TEST_CLI_01_CreatePacket_Success)
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

        TEST_METHOD(TEST_CLI_02_DestroyPacket_Success)
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

        TEST_METHOD(TEST_CLI_15_FreeBuffer_Success)
        {
            char* TxBuffer = new char[10];
            std::string test = "Testing";

            memcpy(TxBuffer, test.c_str(), test.length());

            // Clean up
            FreeBuffer(TxBuffer);

            Assert::IsNull(TxBuffer);
        }


        ///// <summary>
        ///// This test ensures that the AllocateHeaderPtr function successfully allocates pointer for the header of the packet
        ///// </summary>
        TEST_METHOD(TEST_CLI_22_AllocateHeaderPtr)
        {

            // Act
            char* result = AllocateHeaderPtr();

            // Assert
            Assert::IsNotNull(result);
        }
           
        

        ///// <summary>
        ///// This test ensures that the AllocateHeaderPtr function successfully allocates a login pointer based on the size of the Login struct
        ///// </summary>
        TEST_METHOD(TEST_CLI_22_AllocateLoginPtr)
        {
           // Arrange
           char zeroBuffer[sizeof(LogIn)] = { 0 };  // Create a buffer with size LogIn filled with zeros

            // Act
            char* result = AllocateLoginPtr();


            // Compare the allocated memory with the zero buffer
            bool memoryInitializedCorrectly = (memcmp(result, zeroBuffer, sizeof(LogIn)) == 0);


            // Assert 
            Assert::IsNotNull(result);
            Assert::IsTrue(memoryInitializedCorrectly);

           // Clean up
            delete[] result;
        }


        ///// <summary>
        ///// This test ensures that the AllocateHeapMemory function successfully allocates a pointer based on the size given to it
        ///// </summary>
        TEST_METHOD(TEST_CLI_22_AllocateHeapMemory)
        {
           // Arrange
           const int size = 10;
           char zeroBuffer[size] = { 0 };

            // Act
            char* result = AllocateHeapMemory(size);


            // Compare the allocated memory with the zero buffer
            bool memoryInitializedCorrectly = (memcmp(result, zeroBuffer, size) == 0);


            // Assert 
            Assert::IsNotNull(result);
            Assert::IsTrue(memoryInitializedCorrectly);

           // Clean up
            delete[] result;
        }


        /////// <summary>
        /////// This test ensures that the CopyBufferToHeap function successfullycopies the content of the buffer to heap
        /////// </summary>
        TEST_METHOD(CopyBufferToHeap_Test)
        {
            // Arrange 
            char* srcBuffer = "Hello";
            int size = strlen(srcBuffer) + 1; // +1 for null terminator

            // Allocate memory for heapBuffer
            char* heapBuffer = new char[size];

            // Act
            CopyBufferToHeap(heapBuffer, srcBuffer, size);
            bool memoryCopiedCorrectly = (strcmp(heapBuffer, srcBuffer) == 0);

            // Assert
            Assert::IsTrue(memoryCopiedCorrectly);

            // Clean up
            delete[] heapBuffer;
        }




        ///// <summary>
        ///// This test ensures that the AllocateSignupPtr function successfully allocates a pointer for the sign up packet based on the size of the image
        ///// </summary>
        TEST_METHOD(TEST_CLI_22_AllocateSignupPtry)
        {
            // Arrange
            const int imageSize = 500;
            SignUp signup;

            const int size = sizeof(signup.username) + sizeof(signup.password) + sizeof(signup.email) + imageSize;
            char zeroBuffer[size] = { 0 };

            // Act
            char* result = AllocateSignupPtr(imageSize);


            // Compare the allocated memory with the zero buffer
            bool memoryInitializedCorrectly = (memcmp(result, zeroBuffer, size) == 0);


            // Assert 
            Assert::IsNotNull(result);
            Assert::IsTrue(memoryInitializedCorrectly);

            // Clean up
            delete[] result;
        } 
        
       




















        ///// <summary>
        ///// This test ensures that the serializeLoginData function successfully serializes the data in the Login structure into a pointer 
        ///// </summary>
        //TEST_METHOD(TEST_CLI_10_serializeLoginData)
        //{
        //    // Arrange

        //    char* heapBuffer = new char[sizeof(SignUp)];
        //    char* username = "testuser";
        //    char* password = "testpassword";
        //    char* email = "user@gmail.com";

        //    // Act
        //    SerializeStaticDataToBufferheapBuffer(heapBuffer, username, password, email);

        //    // Assert
        //    Assert::IsTrue(memcmp(&loginData, BodyBuffer, sizeof(LogIn)) == 0);

        //}




         ///// <summary>
        ///// This test ensures that the AllocateListingPtr function successfully allocates a pointer for the listing packet based on the size of the image
        ///// </summary>
        TEST_METHOD(TEST_CLI_22_AllocateListingPtr)
        {
            // Arrange
            const int imageSize = 500;
            Listing list;

            const int size = (sizeof(list.Title) + sizeof(list.Location) + sizeof(list.Condition) + sizeof(list.EstimatedWorth) + sizeof(list.Delivery) + sizeof(list.LookingFor)) + imageSize;
            char zeroBuffer[size] = { 0 };
            memset(zeroBuffer, 0, size);

            // Act
            char* result = AllocateListingPtr(imageSize);


            // Compare the allocated memory with the zero buffer
            bool memoryInitializedCorrectly = (memcmp(result, zeroBuffer, size) == 0);


            // Assert 
            Assert::IsNotNull(result);
            Assert::IsTrue(memoryInitializedCorrectly);

            // Clean up
            delete[] result;
        }




        /// <summary>
       /// This test ensures that the DeserializePostCountBuffer function successfully deserializes the post package that contains the number of posts
       /// </summary>


        TEST_METHOD(TEST_CLI_11_DeserializationWithoutTail_ValidPacket)
        {
            // Arrange
            Packet* pkt = CreatePacket();
            char* testData = new char[sizeof(Packet::Header) + sizeof(unsigned int) + 10]; 

            
            Packet::Header testHeader;
            testHeader.Length = 10; 
            memcpy(testData, &testHeader, sizeof(Packet::Header));
            unsigned int testUser = 123; 
            memcpy(testData + sizeof(Packet::Header), &testUser, sizeof(unsigned int));
            char* testDataBody = "Test data"; 
            memcpy(testData + sizeof(Packet::Header) + sizeof(unsigned int), testDataBody, testHeader.Length);

            // Act
            DeserializationWithoutTail(pkt, testData);

            // Assert
            Assert::AreEqual(testHeader.Length, pkt->GetHead()->Length);
            Assert::AreEqual(testUser, pkt->GetBody()->User);
            Assert::IsTrue(strcmp(testDataBody, pkt->GetBody()->Data) == 0);

            // Clean up
            delete[] testData;
            DestroyPacket(pkt);
        }


        TEST_METHOD(TEST_CLI_11_DeserializeClientID_ValidData)
        {
            // Arrange
            const unsigned int expectedClientID = 123; 
            char testData[sizeof(Packet::Header) + sizeof(unsigned int) + 10]; 

            Packet::Header testHeader;
            testHeader.Length = 10; 
            memcpy(testData, &testHeader, sizeof(Packet::Header));
            memcpy(testData + sizeof(Packet::Header), &expectedClientID, sizeof(unsigned int));
            char* testDataBody = "Test data";
            memcpy(testData + sizeof(Packet::Header) + sizeof(unsigned int), testDataBody, testHeader.Length);

            // Act
            unsigned int actualClientID = DeserializeClientID(testData);
            
                 
            // Assert
            Assert::AreEqual(expectedClientID, actualClientID);
        }



        TEST_METHOD(TEST_CLI_11_DeserializeHeaderLengthMember_ValidData)
        {
            // Arrange
            const unsigned int expectedLength = 20; 
            char testData[sizeof(Packet::Header)]; 

            Packet::Header testHeader;
            testHeader.Length = expectedLength;
            memcpy(testData, &testHeader, sizeof(Packet::Header));

            // Act
            unsigned int actualLength = DeserializeHeaderLengthMember(testData);

            // Assert
            Assert::AreEqual(expectedLength, actualLength);
        }



        /// <summary>
        /// This test ensures that the setHeader function can properly set the header of the packet
        /// </summary>
        TEST_METHOD(TEST_CLI_01_SetHeader_ValidData)
        {
            // Arrange
            Packet* pkt = CreatePacket();
            Packet::Header testHeader;
            testHeader.Source[0] = 'S'; 
            testHeader.Destination[0] = 'D';
            testHeader.Route[0] = 'R'; 
            testHeader.Authorization = true; 
            testHeader.Length = 100; 

            // Act
            SetHeader(pkt, &testHeader);

            // Assert
            Assert::AreEqual(testHeader.Source, pkt->GetHead()->Source, L"Source mismatch.");
            Assert::AreEqual(testHeader.Destination, pkt->GetHead()->Destination, L"Destination mismatch.");
            Assert::AreEqual(testHeader.Route, pkt->GetHead()->Route, L"Route mismatch.");
            Assert::AreEqual(testHeader.Authorization, pkt->GetHead()->Authorization, L"Authorization mismatch.");
            Assert::AreEqual(testHeader.Length, pkt->GetHead()->Length, L"Length mismatch.");

            // Clean up
            DestroyPacket(pkt);
        }



        /// <summary>
        /// This test ensures that the setBody function can properly set the body of the packet
        /// </summary>
        TEST_METHOD(TEST_CLI_01_SetBody_ValidData)
        {
            // Arrange
            Packet* pkt = CreatePacket();
            const unsigned int expectedUser = 123; 
            const int dataSize = 10; 
            char testData[dataSize]; 
            for (int i = 0; i < dataSize; ++i) {
                testData[i] = static_cast<char>(i);
            }

            // Act
            SetBody(pkt, expectedUser, testData, dataSize);

            // Assert
            Assert::AreEqual(expectedUser, pkt->GetBody()->User); 

            Assert::IsNotNull(pkt->GetBody()->Data, L"Data field should not be null.");

            for (int i = 0; i < dataSize; ++i) {
                Assert::AreEqual(static_cast<wchar_t>(testData[i]), static_cast<wchar_t>(pkt->GetBody()->Data[i])); 
            }

            Assert::AreEqual(dataSize +1, static_cast<int>(pkt->GetHead()->Length)); 

            // Clean up
            DestroyPacket(pkt);
        }



        TEST_METHOD(TEST_CLI_01_Tail_Checksum)
        {          
            // Arrange
            unsigned int actual;
            unsigned int expected = 0xFF00FF00;

            // Act
            actual = CalculateChecksum();

            // Assert
            Assert::AreEqual(expected, actual);

        }


        TEST_METHOD(TEST_CLI_10_SerializeMyPostCountData)
        {
            // Arrange
            Packet* pkt = CreatePacket();
            pkt->GetBody()->User = 123; 
            int totalSize;

            // Act
            char* serializedData = SerializeMyPostCountData(pkt, totalSize);

            // Assert
            Assert::IsNotNull(serializedData, L"Serialized data should not be null.");

            Assert::AreEqual(static_cast<int>(sizeof(Packet::Header) + sizeof(pkt->GetBody()->User)), totalSize, L"Total size mismatch.");

            // Clean up
            delete[] serializedData; // Free the memory allocated for serialized data
            DestroyPacket(pkt);
        }



        ///// <summary>
        /// This test ensures that the SerializeData function successfully deserializes the serialized data passed to it.
        /// </summary>
        TEST_METHOD(TEST_CLI_10_SerializeData)
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
        Pkt->GetHead()->Length = 10; 

        int totalSize = 0;

        // Act
        char* TxBuffer = nullptr;
        TxBuffer = SerializeData(Pkt, totalSize);

        // Assert
        Assert::IsNotNull(TxBuffer);
        }


        TEST_METHOD(TEST_CLI_10_SerializeHeaderOnlyPkt)
        {
            // Arrange
            // Create a Packet instance
            Packet* pkt = CreatePacket();
            // Set some sample data for the header
            pkt->GetHead()->Source[0] = 'S';
            pkt->GetHead()->Destination[0] = 'D';
            pkt->GetHead()->Route[0] = 'R';
            pkt->GetHead()->Authorization = true;
            pkt->GetHead()->Length = 10;

            // Act
            int totalSize = 0;
            char* serializedData = SerializeHeaderOnlyPkt(pkt, totalSize);

            // Assert
            Assert::AreEqual(static_cast<int>(sizeof(Packet::Header)), totalSize);


            Assert::AreEqual('S', serializedData[0]);
            Assert::AreEqual('D', serializedData[sizeof(pkt->GetHead()->Source)]);
            Assert::AreEqual('R', serializedData[sizeof(pkt->GetHead()->Source) + sizeof(pkt->GetHead()->Destination)]);
            Assert::IsTrue(serializedData[sizeof(pkt->GetHead()->Source) + sizeof(pkt->GetHead()->Destination) + sizeof(pkt->GetHead()->Route)] != 0);

            // Clean up
            delete[] serializedData; 
            DestroyPacket(pkt);
        }



        TEST_METHOD(TEST_DeserializeHeader_ValidData)
        {
            // Arrange
            Packet* pkt = CreatePacket();
            char headerBuffer[sizeof(Packet::Header)]; 

            memset(headerBuffer, 'S', sizeof(headerBuffer));

            char* bufferPtr = headerBuffer;
            char** bufferPtrPtr = &bufferPtr;
            char*** bufferPtrPtrPtr = &bufferPtrPtr;


            // Act
            DeserializeHeader(pkt, &bufferPtrPtrPtr);

            // Assert
            Assert::AreEqual('S', pkt->GetHead()->Source[0]);
            Assert::AreEqual('S', pkt->GetHead()->Destination[0]);

            // Clean up
            DestroyPacket(pkt);
        }



        TEST_METHOD(TEST_DeserializePostCountBuffer)
        {
            // Arrange
            // Create a Packet instance
            Packet* pkt = CreatePacket();

            // Prepare sample data for the header
            Packet::Header expectedHeader;
            strcpy_s(expectedHeader.Source, "Source");
            strcpy_s(expectedHeader.Destination, "Destination");
            strcpy_s(expectedHeader.Route, "Route");
            expectedHeader.Authorization = true;
            expectedHeader.Length = 10; 

            char buffer[sizeof(Packet::Header)];
            memcpy(buffer, &expectedHeader, sizeof(Packet::Header));

            int numberOfPosts;

            // Act
            DeserializePostCountBuffer(pkt, buffer, numberOfPosts);

            // Assert
            Assert::AreEqual(static_cast<int>(expectedHeader.Length), numberOfPosts);

            // Clean up
            DestroyPacket(pkt);
        }


        TEST_METHOD(CreatePacket_ReturnsNonNullPointer)
        {
            // Act
            Packet* packet = CreatePacket();

            // Assert
            Assert::IsNotNull(packet);

            // Clean up
            delete packet;
        }



        ///// <summary>
         ///// This test ensures that the logPacket function sucessfully opends the log file in the append mode
         ///// </summary>
        TEST_METHOD(TEST_CLI_21_LogPacketTest)
        {
            // Arrange
            std::string filename = "test_log.txt";
            Logging logging(filename);
            char** txBuffer = new char* [1];
            txBuffer[0] = new char[100]; 

            // Act
            logging.logPacket(&txBuffer);

            // Assert
            // Check if the log file was created
            std::ifstream file(filename);
            Assert::IsTrue(file.good());

            // Clean up
            delete[] txBuffer[0];
            delete[] txBuffer;
        }




        ///// <summary>
         ///// This test ensures that the log file contains the expected data
         ///// </summary>
        TEST_METHOD(TEST_CLI_21_LogListingSendTest)
        {
            // Arrange
            std::string filename = "test_log.txt";
            Logging logging(filename);
            Listing listing;
            // Populate the listing with test data
            strcpy_s(listing.Title, "Test Title");
            strcpy_s(listing.Location, "Test Location");
            strcpy_s(listing.Condition, "Test Condition");
            strcpy_s(listing.EstimatedWorth, "Test Estimated Worth");
            strcpy_s(listing.Delivery, "Test Delivery");
            strcpy_s(listing.LookingFor, "Test Looking For");

            // Act
            logging.logListingSend(&listing);

            // Assert
            // Check if the log file was created and contains expected data
            std::ifstream file(filename);
            std::string line;
            bool found = false;
            while (std::getline(file, line)) {
                if (line.find("Test Title") != std::string::npos) {
                    found = true;
                    break;
                }
            }
            Assert::IsTrue(found);

            // Clean up
            file.close();
        }





        // Testing the logging functions

        //TEST_METHOD(Logging_LogPacket_ValidData)
        //{
        //    // Arrange

        //    Logging logging{ "test.log" };

        //    Packet testPacket;
        //    testPacket.GetHead()->Source = "Source";
        //    testPacket.GetHead()->Destination = "Destination";
        //    testPacket.GetHead()->Route = "Route";
        //    testPacket.GetHead()->Authorization = "Authorization";
        //    testPacket.GetHead()->Length = 10; // Assuming length is 10

        //    // Mock TxBuffer data
        //    char headerBuffer[sizeof(Packet::Header)];
        //    memcpy(headerBuffer, testPacket.GetHead(), sizeof(Packet::Header));
        //    char*** TxBuffer = &headerBuffer;

        //    // Act
        //    logging.logPacket(TxBuffer);

        //    // Assert
        //    // Check if log file is correctly updated with the expected data
        //    std::ifstream logFile("test.log");
        //    std::string line;
        //    //if (logFile.is_open()) {
        //    //    // Check if header information is logged correctly
        //    //    Assert::IsTrue(std::getline(logFile, line));
        //    //    Assert::AreEqual(std::string("Time: "), line.substr(0, 6));
        //    //    Assert::IsTrue(std::getline(logFile, line));
        //    //    Assert::AreEqual(std::string("Source: ") + testPacket.GetHead()->Source, line);
        //    //    Assert::IsTrue(std::getline(logFile, line));
        //    //    Assert::AreEqual(std::string("Destination: ") + testPacket.GetHead()->Destination, line);
        //    //    Assert::IsTrue(std::getline(logFile, line));
        //    //    Assert::AreEqual(std::string("Route: ") + testPacket.GetHead()->Route, line);
        //    //    Assert::IsTrue(std::getline(logFile, line));
        //    //    Assert::AreEqual(std::string("Authorization: ") + testPacket.GetHead()->Authorization, line);
        //    //    Assert::IsTrue(std::getline(logFile, line));
        //    //    Assert::AreEqual(std::string("Body Length: ") + std::to_string(testPacket.GetHead()->Length), line);
        //        // Close the log file
        //        logFile.close();
        //    }
        //    //else {
        //    //    // Log file couldn't be opened
        //    //    Assert::Fail(L"Unable to open log file.");
        //    //}
        //}

        // Test for logListingSend function
        //TEST_METHOD(Logging_LogListingSend_ValidData)
        //{
        //    // Arrange
        //    Listing testListing{ "Title", "Location", "Condition", 100, "Delivery", "Looking For" };

        //    // Act
        //    logging.logListingSend(&testListing);

        //    // Assert
        //    // Check if log file is correctly updated with the expected data
        //    std::ifstream logFile("test.log");
        //    std::string line;
        //    if (logFile.is_open()) {
        //        // Check if listing information is logged correctly
        //        Assert::IsTrue(std::getline(logFile, line));
        //        Assert::AreEqual(std::string("Time: "), line.substr(0, 6));
        //        Assert::IsTrue(std::getline(logFile, line));
        //        Assert::AreEqual(std::string("Title: ") + testListing.Title, line);
        //        Assert::IsTrue(std::getline(logFile, line));
        //        Assert::AreEqual(std::string("Location: ") + testListing.Location, line);
        //        Assert::IsTrue(std::getline(logFile, line));
        //        Assert::AreEqual(std::string("Condition: ") + testListing.Condition, line);
        //        Assert::IsTrue(std::getline(logFile, line));
        //        Assert::AreEqual(std::string("Estimated Worth: ") + std::to_string(testListing.EstimatedWorth), line);
        //        Assert::IsTrue(std::getline(logFile, line));
        //        Assert::AreEqual(std::string("Delivery: ") + testListing.Delivery, line);
        //        Assert::IsTrue(std::getline(logFile, line));
        //        Assert::AreEqual(std::string("Swap Idea (Looking For): ") + testListing.LookingFor, line);
        //        // Close the log file
        //        logFile.close();
        //    }
        //    else {
        //        // Log file couldn't be opened
        //        Assert::Fail(L"Unable to open log file.");
        //    }
        //}













     };
















        ///// <summary>
        ///// <summary>
        ///// This test ensures that the DeserializePostCountBuffer function successfully deserializes the post package that contains the number of posts
        ///// </summary>
        //TEST_METHOD(TEST_CLI_11_DeserializePostCountBuffer)
        //{
        //    // Arrange
        //    Packet* originalPacket = new Packet();
        //    originalPacket->GetHead()->Length = 10; //  this means there are 10 posts

        //    char* serializedBuffer = new char[sizeof(Packet)]; 
        //    memcpy(serializedBuffer, originalPacket, sizeof(Packet)); 

        //    int numberOfPosts = 0;

        //    // Act
        //    DeserializePostCountBuffer(originalPacket, serializedBuffer, numberOfPosts);

        //    // Assert
        //    Assert::AreEqual(10, numberOfPosts); 

        //    // Clean up
        //    delete[] serializedBuffer;
        //    delete originalPacket;
        //}







   
        ///// <summary>
        ///// This test ensures that the Deserialization function successfully deserializes the serialized data passed to it.
        ///// </summary>
        //TEST_METHOD(TEST_CLI_11_Deserialization)
        //{
        //    // Arrange
        //    Packet* Pkt = new Packet(); // create a new packet:

        //    Pkt->GetBody()->Data = nullptr;

        //    memset(Pkt->GetHead(), 0, sizeof(*(Pkt->GetHead())));
        //    memset(&(Pkt->GetBody()->User), 0, sizeof(Pkt->GetBody()->User));
        //    memset(&(Pkt->GetTail()->Checksum), 0, sizeof(Pkt->GetTail()->Checksum));

        //    Pkt->GetBody()->Data = new char[10];
        //    Pkt->GetBody()->Data = "Testing";
        //    Pkt->GetHead()->Length = 10; // Set the length

        //    int totalSize = 0;

        //    Pkt->GetTail()->Checksum = 0xFF00FF00;

        //    totalSize = sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length + sizeof(*(Pkt->GetTail()));

        //    Pkt->GetTxBuffer() = new char[totalSize];

        //    memcpy(Pkt->GetTxBuffer(), Pkt->GetHead(), sizeof(*(Pkt->GetHead())));

        //    memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())), &(Pkt->GetBody()->User), sizeof(Pkt->GetBody()->User));
        //    memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User), Pkt->GetBody()->Data, Pkt->GetHead()->Length);
        //    memcpy(Pkt->GetTxBuffer() + sizeof(*(Pkt->GetHead())) + sizeof(Pkt->GetBody()->User) + Pkt->GetHead()->Length, Pkt->GetTail(), sizeof(*(Pkt->GetTail())));

        //    Packet* packet = new Packet(); // create a new packet:

        //    packet->GetBody()->Data = nullptr;

        //    memset(packet->GetHead(), 0, sizeof(*(packet->GetHead())));
        //    memset(&(packet->GetBody()->User), 0, sizeof(packet->GetBody()->User));
        //    memset(&(packet->GetTail()->Checksum), 0, sizeof(packet->GetTail()->Checksum));

        //    // Act
        //    Deserialization(packet, Pkt->GetTxBuffer()); // Deserializing the data into the new packet

        //    // Assert
        //    Assert::AreEqual(10, (int)Pkt->GetHead()->Length);
        //    Assert::IsNotNull(Pkt->GetBody()->Data);
        //}




   
}
