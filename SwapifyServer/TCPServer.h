#pragma once
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <winsock2.h>
#include "UsersRoute.h"
//#include "ListingRoutes.h"

#pragma comment(lib, "ws2_32.lib")

class TCPServer
{
public:
    TCPServer(int port);
    void start();
private:
    int port_;
    std::map<std::string, std::function<void(SOCKET)>> router_;
    void handleConnection(SOCKET clientSocket);
};