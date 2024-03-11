#pragma once

#include "Packet.h"
#include "TCPServer.h"
#include "Route.h"
#include "SQLiteDatabase.h"
#include "User.h"


class UserRoutes : public Route {
public:
    Packet* handleRequest(Packet& packet, SOCKET clientSocket, std::string action) override;

private:
    Packet* handleLogin(User user);
    Packet* handleRegister(User user);
    Packet* handleUpdateUser(User user);
    Packet* handleDeleteUser(User user);
};