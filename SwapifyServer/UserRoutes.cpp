
#include "UsersRoute.h"
#include <iostream>
#include <string>
//
//Packet* UserRoutes::handleUserRequest(Packet& packet, SOCKET clientSocket, std::string action) {
//
//
//    std::cout << "UserRoutes::handleUserRequest" << std::endl;
//    if (&packet == nullptr) {
//        std::cerr << "Packet is null" << std::endl;
//        return nullptr;
//    }
//
//    //// Check if User field is not null
//    //if (packet.GetBody() == nullptr || packet.GetBody()->User == nullptr) {
//    //    std::cerr << "User field is null" << std::endl;
//    //    return nullptr;
//    //}
//    //parse string to user object
//    User user;
//    //user.parseUser(packet.GetBody()->User);
//    //userString parse into user
//    Packet* resultPacket = CreatePacket();
//    if (action == "login") {
//        resultPacket = handleLogin(user);
//    }
//    else if (action == "register") {
//        resultPacket = handleRegister(user);
//    }
//    else if (action == "update") {
//        resultPacket = handleUpdateUser(user);
//    }
//    else if (action == "delete") {
//        resultPacket = handleDeleteUser(user);
//    }
//    else {
//        std::cout << "Invalid route" << std::endl;
//    }
//    return resultPacket;
//}
//
//Packet* UserRoutes::handleLogin(User user) {
//    std::cout << "UserRoutes::handleLogin" << std::endl;
//    Packet* ResultPacket = CreatePacket();
//    // Check if user exists
//    SQLiteDatabase db("database.db");
//    if (!db.isOpen()) {
//        std::cerr << "Failed to open database." << std::endl;
//        return nullptr; // Return nullptr to indicate failure
//    }
//
//    // Prepare SQL query to select user by username
//    std::string query = "SELECT * FROM users WHERE username = '" + user.getEmailAddress() + "'";
//
//    // Execute query
//    sqlite3_stmt* statement;
//    if (sqlite3_prepare_v2(db.getDB(), query.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
//        std::cerr << "Error preparing SQLite statement." << std::endl;
//        return nullptr;
//    }
//
//    // Check if a row was returned (i.e., if the user exists)
//    int result = sqlite3_step(statement);
//    if (result == SQLITE_ROW) {
//        // User exists, validate password
//        std::string storedPassword = std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
//        if (user.validatePassword(user.getPassword(), storedPassword)) {
//            // Password is correct
//            // You can perform additional operations here if needed
//            return new Packet; // Return a success packet
//        }
//        else {
//            // Incorrect password
//            return new Packet;
//        }
//    }
//    //else if (result == SQLITE_DONE) {
//    //    // User does not exist
//    //    SetDataUser(ResultPacket, user.serializeUser(user));
//    //    return ResultPacket;
//    //}
//    else {
//        // Error occurred
//        std::cerr << "Error executing SQLite query." << std::endl;
//        return nullptr;
//    }
//
//    // Finalize statement
//    sqlite3_finalize(statement);
//}
//
//Packet* UserRoutes::handleRegister(User user) {
//    // Hash the user's password
//    std::string hashedPassword = user.generateHash(user.getPassword());
//
//    // Insert the user into the database
//    SQLiteDatabase db("database.db");
//    if (!db.isOpen()) {
//        std::cerr << "Failed to open database." << std::endl;
//        return new Packet;
//    }
//
//    // Prepare SQL query to insert user
//    std::string query = "INSERT INTO users (id, email_address, password, first_name, last_name, profile_picture, date_created, account_status) VALUES ("
//        + user.getIdAsString() + ", "
//        + "'" + user.getEmailAddress() + "', "
//        + "'" + hashedPassword + "', "
//        + "'" + user.getFirstName() + "', "
//        + "'" + user.getLastName() + "', "
//        + "'" + user.getProfilePicture() + "', "
//        + "'" + user.getDateCreatedAsString() + "', "
//        + "'" + user.getAccountStatus() + "')";
//
//    // Execute query
//    if (!db.executeQuery(query.c_str())) {
//        std::cerr << "Failed to execute query." << std::endl;
//        return new Packet;
//    }
//
//    return new Packet;
//}
//
//
//Packet* UserRoutes::handleUpdateUser(User user) {
//    if (!user.getPassword().empty()) {
//        std::string hashedPassword = user.generateHash(user.getPassword());
//        user.setPassword(hashedPassword); // Update the user's password
//    }
//
//    // Update the user information in the database
//    SQLiteDatabase db("database.db");
//    if (!db.isOpen()) {
//        std::cerr << "Failed to open database." << std::endl;
//        return new Packet;;
//    }
//
//    // Prepare SQL query to update user
//    std::string query = "UPDATE users SET "
//        "email_address = '" + user.getEmailAddress() + "', "
//        "password = '" + user.getPassword() + "', "
//        "first_name = '" + user.getFirstName() + "', "
//        "last_name = '" + user.getLastName() + "', "
//        "profile_picture = '" + user.getProfilePicture() + "', "
//        "account_status = '" + user.getAccountStatus() + "' "
//        "WHERE id = " + user.getIdAsString();
//
//    // Execute query
//    if (!db.executeQuery(query.c_str())) {
//        std::cerr << "Failed to execute query." << std::endl;
//        return new Packet;
//    }
//
//    return new Packet;
//}
//
//
//Packet* UserRoutes::handleDeleteUser(User user) {
//    // Delete the user from the database
//    SQLiteDatabase db("database.db");
//    if (!db.isOpen()) {
//        std::cerr << "Failed to open database." << std::endl;
//        return new Packet;
//    }
//
//    // Prepare SQL query to delete user
//    std::string query = "DELETE FROM users WHERE id = " + user.getIdAsString();
//
//    // Execute query
//    if (!db.executeQuery(query.c_str())) {
//        std::cerr << "Failed to execute query." << std::endl;
//        return new Packet;
//    }
//
//    return new Packet;
//}