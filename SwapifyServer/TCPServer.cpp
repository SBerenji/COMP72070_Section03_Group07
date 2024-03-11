
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <winsock2.h>
#include <thread>
#include <sstream>
#include <vector>
#include <string>
#include "TCPServer.h"



#pragma comment(lib, "ws2_32.lib")


    TCPServer::TCPServer(int port) : port_(port) {}

    void TCPServer::start() {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed: " << result << std::endl;
            return;
        }

        SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSocket == INVALID_SOCKET) {
            std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr.sin_port = htons(port_);

        result = ::bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));
        if (result == SOCKET_ERROR) {
            std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return;
        }

        result = listen(listenSocket, SOMAXCONN);
        if (result == SOCKET_ERROR) {
            std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return;
        }

        std::cout << "Server listening on port " << port_ << std::endl;

        while (true) {
            SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                closesocket(listenSocket);
                WSACleanup();
                return;
            }

            std::thread(&TCPServer::handleConnection, this, clientSocket).detach();
        }
    }

    void TCPServer::handleConnection(SOCKET clientSocket) {
        char buffer[1024] = { 0 };

        int valread = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (valread == SOCKET_ERROR) {
            std::cerr << "Failed to read from socket" << std::endl;
            return;
        }
        Packet* RxPkt = CreatePacket();
        Deserialization(RxPkt, buffer);
       
       
        std::string route = RxPkt->GetHead()->Route;
        std::istringstream ss(route);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, '/')) {
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }

        if (tokens[0] == "user") {
            UserRoutes userRoutes;
            Packet* resultPacket = userRoutes.handleUserRequest(*RxPkt, clientSocket, tokens[1]);
            //return here to socket
        }
        else if (tokens[0] == "listing") {
            // Handle listing request
           /* ListingRoutes listingRoutes;
            Packet* resultPacket = listingRoutes.handleListingRequest(*RxPkt, clientSocket, tokens[1]);*/
        }
        else {
            // Handle unknown request
            std::cerr << "Unknown request: " << tokens[0] << std::endl;
        }

    }
