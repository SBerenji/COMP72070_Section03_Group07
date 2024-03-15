#include <iostream>

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include <fstream>
#include <string>

#include "Packet.h"
#include "PacketWrapper.h"

extern "C" __declspec(dllexport) int setupConnection();
extern "C" __declspec(dllexport) SOCKET setupConnection2();
extern "C" __declspec(dllexport) int sendData(SOCKET ClientSocket, char* TxBuffer, int totalSize);
extern "C" __declspec(dllexport) int recvData(SOCKET ClientSocket, char* RxBuffer, int RxBufferSize);
extern "C" __declspec(dllexport) int CloseSocket(SOCKET ClientSocket);

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
int CloseSocket(SOCKET ClientSocket) {
    closesocket(ClientSocket);
    // Cleaning up the socket

    std::cout << "Client Socket Closed" << std::endl;

    WSACleanup();
    // Cleaning up the winsock library

    std::cout << "WSA Closed" << std::endl;

    return 1;
}

int sendData(SOCKET ClientSocket, char* TxBuffer, int totalSize) {
    // The order matters, therefore the send is before receive on Client side.
    // Send

    //char sendBuffer[20] = "Hello World!";
    // buffer to store address of the string

    int sendSize = send(ClientSocket, TxBuffer, totalSize, 0);
    // here we are passing the clientSocket, the address of the string and the size of the string that we want the server to receive
    // +1 becuase of NULL terminator

    if (sendSize < 0)
    {
        std::cout << "Sending Failed" << std::endl;

        closesocket(ClientSocket);

        WSACleanup();

        return 0;
    }

    else
    {

        std::cout << "Message Succesfully sent: " << TxBuffer << std::endl;
    }

    return 1;
    // returning 1 upon successful completion.
}

int recvData(SOCKET ClientSocket, char* RxBuffer, int RxBufferSize) {
    // The order matters, therefore the send is before receive on Client side.
    // Send

    //char sendBuffer[20] = "Hello World!";
    // buffer to store address of the string

    int recvSize = recv(ClientSocket, RxBuffer, RxBufferSize, 0);
    // here we are passing the clientSocket, the address of the string and the size of the string that we want the server to receive
    // +1 becuase of NULL terminator

    if (recvSize < 0)
    {
        std::cout << "Sending Failed" << std::endl;

        closesocket(ClientSocket);

        WSACleanup();

        return 0;
    }

    else
    {

        std::cout << "Message Succesfully sent: " << RxBuffer << std::endl;
    }

    return 1;
    // returning 1 upon successful completion.
}

int setupConnection() {
    // starting up and configuring the Winsock dynamically linked library

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
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
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // here we are using TCP protocol

    if (ClientSocket == INVALID_SOCKET)
    {
        WSACleanup();

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

    if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR)
    {
        closesocket(ClientSocket);
        WSACleanup();

        std::cout << "ERROR: Connection attempted Failed" << std::endl;

        return 0;
    }

    else {
        std::cout << "Connection Established" << std::endl;
    }

    return 1;
}






SOCKET setupConnection2() {
    // starting up and configuring the Winsock dynamically linked library

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
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
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // here we are using TCP protocol

    if (ClientSocket == INVALID_SOCKET)
    {
        WSACleanup();

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

    if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR)
    {
        closesocket(ClientSocket);
        WSACleanup();

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