#pragma once
#include "User.h"




class UserRoutes{
public:
    Packet* handleUserRequest(Packet& packet, SOCKET clientSocket, std::string action);

    Packet* handleLogin(User user);
    Packet* handleRegister(User user);
    Packet* handleUpdateUser(User user);
    Packet* handleDeleteUser(User user);
};