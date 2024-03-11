#pragma once

#include <string>
class Route {
public:
	virtual Packet* handleRequest(Packet& packet, SOCKET clientSocket, std::string action) = 0;
};