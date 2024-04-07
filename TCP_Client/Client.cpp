#include <iostream>

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <fstream>
#include <string>

#include "Packet.h"
#include "PacketWrapper.h"
#include "Logging.h"

//extern "C" __declspec(dllexport) int setupConnection();
extern "C" __declspec(dllexport) SOCKET setupConnection(WSAStartupFunc wsaStartup = WSAStartup, socketFunc socketfunc = socket, connectFunc connectfunc = connect, WSACleanupFunc wsacleanupfunc = WSACleanup, int socketType = SOCK_STREAM);
extern "C" __declspec(dllexport) SOCKET setupConnection2();

extern "C" __declspec(dllexport) int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize);

extern "C" __declspec(dllexport) int sendDataFunc(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc = send);

extern "C" __declspec(dllexport) int recvData(SOCKET ClientSocket, char** RxBuffer, int RxBufferSize);
extern "C" __declspec(dllexport) int recvDataFunc(SOCKET ClientSocket, char*** RxBuffer, int RxBufferSize, RecvFunction recvFunc = recv);


extern "C" __declspec(dllexport) int CloseSocket(SOCKET ClientSocket);
extern "C" __declspec(dllexport) int CloseSocketFunc(SOCKET ClientSocket, closesocketFunc closeFunc = closesocket, WSACleanupFunc wsacleanupfunc = WSACleanup);




void mockLogFile(char** TxBuffer) {
    std::string filename = "Client_Log.txt";
    
    Logging log(filename);
}


int CloseSocket(SOCKET ClientSocket) {
    return CloseSocketFunc(ClientSocket, closesocket, WSACleanup);
}


///// Most of the sockets defined will receive a default parameter; this is for testing purposes
int CloseSocketFunc(SOCKET ClientSocket, closesocketFunc closeFunc, WSACleanupFunc wsacleanupfunc)
{

    // Cleaning up the socket
    if (closeFunc(ClientSocket) == SOCKET_ERROR)
    {
        std::cerr << "Error closing the socket" << std::endl;
        return 0;
    }

    std::cout << "Client Socket Closed" << std::endl;

    // Cleaning up the winsock library
    if (wsacleanupfunc() == SOCKET_ERROR)
    {
        std::cerr << "Error cleaning WSACleanup" << std::endl;
        return 2;
    }

    std::cout << "WSA Closed" << std::endl;

    return 1;
}


int sendData(SOCKET ClientSocket, const char* TxBuffer, int totalSize)
{
    return sendDataFunc(ClientSocket, TxBuffer, totalSize, send);
}

int sendDataFunc(SOCKET ClientSocket, const char* TxBuffer, int totalSize, SendFunction sendFunc)
{

    int sendSize = sendFunc(ClientSocket, TxBuffer, totalSize, 0);
    // here we are passing the clientSocket, the address of the string and the size of the string that we want the server to receive
    // +1 becuase of NULL terminator

    if (sendSize < 0)
    {
        std::cout << "Sending Failed" << std::endl;

        CloseSocket(ClientSocket);

        return 0;
    }

    else
    {

        std::cout << "Message Succesfully sent: " << TxBuffer << std::endl;
    }

    return 1;
    // returning 1 upon successful completion.
}


int recvData(SOCKET ClientSocket, char** RxBuffer, int RxBufferSize)
{
    return recvDataFunc(ClientSocket, &RxBuffer, RxBufferSize, recv);
}


int recvDataFunc(SOCKET ClientSocket, char*** RxBuffer, int RxBufferSize, RecvFunction recvFunc) {
    // The order matters, therefore the send is before receive on Client side.
    // Send

    //char sendBuffer[20] = "Hello World!";
    // buffer to store address of the string

    **RxBuffer = new char[RxBufferSize];

    memset(**RxBuffer, 0, RxBufferSize);

    int recvSize = recvFunc(ClientSocket, **RxBuffer, RxBufferSize, 0);
    // here we are passing the clientSocket, the address of the string and the size of the string that we want the server to receive
    // +1 becuase of NULL terminator

    if (recvSize < 0)
    {
        std::cout << "Sending Failed" << std::endl;

        CloseSocket(ClientSocket);

        return 0;
    }

    else
    {

        std::cout << "Message Succesfully sent: " << **RxBuffer << std::endl;
    }

    return 1;
    // returning 1 upon successful completion.
}



// this is for compatibility with c# since c# does not support default parameters
SOCKET setupConnection2()
{
    return setupConnection();   // no need to pass arguments since the paremters have default values
}



SOCKET setupConnection(WSAStartupFunc wsaStartup, socketFunc socketfunc, connectFunc connectfunc, WSACleanupFunc wsacleanupfunc, int socketType) {
    // starting up and configuring the Winsock dynamically linked library

    WSADATA wsaData;

    if (wsaStartup(MAKEWORD(2, 2), &wsaData) != 0)
        // providing the address of the object (&wsaData) into the library's startup function
    {
        std::cout << "ERROR: Failed to start WSA" << std::endl;

        return 0;
    }

    else {
        std::cout << "WSA Started" << std::endl;
    }

    // Create a Socket
    // This is to make the call to the server

    SOCKET ClientSocket;
    ClientSocket = socketfunc(AF_INET, socketType, IPPROTO_TCP);
    // here we are using TCP protocol

    if (ClientSocket == INVALID_SOCKET)
    {
        wsacleanupfunc();

        std::cout << "ERROR: Failed to create ServerSocket" << std::endl;

        return 0;
    }

    else {
        std::cout << "Client Socket Created" << std::endl;
    }

    // Connect
    // starting up the three way handshake process
    // The server should have binded and configured a socket for Listening by now.

    sockaddr_in SvrAddr;
    // This object contains necessary information needed by the library to setup the TCP/IP headers correctly.

    SvrAddr.sin_family = AF_INET;
    // The family which specifies the protocol that will be used.

    SvrAddr.sin_port = htons(27000);
    // The client is using the port 27500.

    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // The IPV4 address the server is located on. Since different network we are gonna send the request to the NAT using localhost.

    if ((connectfunc(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR)
    {
        CloseSocket(ClientSocket);

        std::cout << "ERROR: Connection attempted Failed" << std::endl;

        return 0;
    }

    else {
        std::cout << "Connection Established" << std::endl;
    }


    return ClientSocket;
}





//int main(int argc, char* argv[]) {
//    
//
//    //// Receive
//    //char recvBuffer[17];
//    //// creating a receive buffer
//
//    //int recvSize = recv(ClientSocket, recvBuffer, sizeof(recvBuffer), 0);
//    //// using the recv to get the message from the server.
//
//    //if (recvSize < 0)
//    //{
//    //    std::cout << "Receiving Failed";
//
//    //    closesocket(ClientSocket);
//
//    //    WSACleanup();
//
//    //    return 0;
//    //}
//
//    //else
//    //{
//    //    std::cout << "Message Succesfully Received: " << recvBuffer << std::endl;
//    //}
//
//    //closesocket(ClientSocket);
//    //// Cleaning up the socket
//
//    //std::cout << "Client Socket Closed" << std::endl;
//
//    //WSACleanup();
//    //// Cleaning up the winsock library
//
//    //std::cout << "WSA Closed" << std::endl;
//
//    //return 1;
//    // returning 1 upon successful completion.
//}





//extern "C" {
//
//    __declspec(dllexport) Packet* CreatePacket();
//}

// extern "C" __declspec(dllexport) int sendData(SOCKET ClientSocket);
//extern "C" __declspec(dllexport) int main(int argc, char* argv[]);

//void Print() {  
//    std::cout << "Hello World!!" << std::endl;
//}
//
//void Print2() {
//    std::cout << "Hello World!!" << std::endl;
//}
//


//
//int setupConnection() {
//    // starting up and configuring the Winsock dynamically linked library
//
//    WSADATA wsaData;
//
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//        // providing the address of the object (&wsaData) into the library's startup function
//    {
//        std::cout << "ERROR: Failed to start WSA" << std::endl;
//
//        return 0;
//    }
//
//    else {
//        std::cout << "WSA Started" << std::endl;
//    }
//
//    // Create a Socket
//    // This is to make the call to the server
//
//    SOCKET ClientSocket;
//    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    // here we are using TCP protocol
//
//    if (ClientSocket == INVALID_SOCKET)
//    {
//        WSACleanup();
//
//        std::cout << "ERROR: Failed to create ServerSocket" << std::endl;
//
//        return 0;
//    }
//
//    else {
//        std::cout << "Client Socket Created" << std::endl;
//    }
//
//    // Connect
//    // starting up the three way handshake process
//    // The server should have binded and configured a socket for Listening by now.
//
//    sockaddr_in SvrAddr;
//    // This object contains necessary information needed by the library to setup the TCP/IP headers correctly.
//
//    SvrAddr.sin_family = AF_INET;
//    // The family which specifies the protocol that will be used.
//
//    SvrAddr.sin_port = htons(27000);
//    // The client is using the port 27500.
//
//    SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    // The IPV4 address the server is located on. Since different network we are gonna send the request to the NAT using localhost.
//
//    if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR)
//    {
//        closesocket(ClientSocket);
//        WSACleanup();
//
//        std::cout << "ERROR: Connection attempted Failed" << std::endl;
//
//        return 0;
//    }
//
//    else {
//        std::cout << "Connection Established" << std::endl;
//    }
//
//    return 1;
//}
