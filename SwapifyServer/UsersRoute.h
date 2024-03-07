#pragma once

#include "Packet.h"
#include "TCPServer.h"
#include "Route.h"
#include "SQLiteDatabase.h"
#include "User.h"


class UserRoutes : public Route {
public:
    void handleRequest(Packet& packet, SOCKET clientSocket, std::string action) override;

private:
    void handleLogin(User user);
    void handleRegister(User user);
    User handleGetUser(User user);
    void handleUpdateUser(User user);
    void handleDeleteUser(User user);


};