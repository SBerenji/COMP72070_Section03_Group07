#include "UsersRoute.h"
#include "PacketWrapper.h"
#include "User.h"
#include "SQLiteDatabase.h"
#include <iostream>
#include <string>

void UserRoutes::handleRequest(Packet& packet, SOCKET clientSocket, std::string action) {
	//parse string to user object
	User user;
	user.parseUser(packet.GetBody()->User);
	//userString parse into user
	if(action == "login") {
		handleLogin(user);
	}
	else if(action == "register") {
		handleRegister(user);
	}
	else if(action == "update") {
		handleUpdateUser(user);
	}
	else if(action == "delete") {
		handleDeleteUser(user);
	}
	else if (action == "get") {
	}
	else {
		std::cout << "Invalid route" << std::endl;
	}
}

void UserRoutes::handleLogin(User user) {

}

void UserRoutes::handleRegister(User user) {
	
}

//User UserRoutes::handleGetUser(Packet& packet, std::string route) {
	// Implementation for handling getting user information
	// Return User object

//}

void UserRoutes::handleUpdateUser(User user) {
	// Implementation for handling updating user information
}

void UserRoutes::handleDeleteUser(User user) {
	// Implementation for handling deleting user information
}