#pragma once
#include <string>
class Route {
public:
	virtual void handleRequest(Packet& packet, SOCKET clientSocket, std::string action) = 0;
};