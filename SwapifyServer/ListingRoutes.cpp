//#include "ListingRoutes.h"
//#include <iostream>
//#include <string>
//
//Packet* ListingRoutes::handleListingRequest(Packet& packet, SOCKET clientSocket, std::string action) {
//	//parse string to user object
//	Listing listing;
//	listing.parseListing(packet.GetBody()->Listing);
//	//userString parse into user
//	Packet* resultPacket = CreatePacket();
//	ListingRoutes listingRoutes;
//
//	if (action == "create") {
//		resultPacket = listingRoutes.handleCreateListing(listing);
//	}
//	else if (action == "get") {
//		resultPacket = listingRoutes.handleGetListing(listing);
//	}
//	else if (action == "update") {
//		resultPacket = listingRoutes.handleUpdateListing(listing);
//	}
//	else if (action == "delete") {
//		resultPacket = listingRoutes.handleDeleteListing(listing);
//	}
//	else {
//		std::cout << "Invalid route" << std::endl;
//	}
//	return resultPacket;
//}
//
//Packet* ListingRoutes::handleCreateListing(Listing listing) {
//	// Check if user exists
//	SQLiteDatabase db("database.db");
//	if (!db.isOpen()) {
//		std::cerr << "Failed to open database." << std::endl;
//		return nullptr; // Return nullptr to indicate failure
//	}
//
//	// Prepare SQL query to select user by username
//	std::string query = "INSERT INTO listings (title, description, price, category, location, image) VALUES ('"
//		+ listing.getTitle() + "', '"
//		+ listing.getDescription() + "', '"
//		+ std::to_string(listing.getPrice()) + "', '"
//		+ listing.getCategory() + "', '"
//		+ listing.getLocation() + "', '"
//		+ listing.getImage() + "')";
//
//	// Execute query
//	sqlite3_stmt* statement;
//	if (sqlite3_prepare_v2(db.getDB(), query.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
//		std::cerr << "Error preparing SQLite statement." << std::endl;
//		return nullptr;
//	}
//
//	// Check if a row was returned (i.e., if the user exists)
//	int result = sqlite3_step(statement);
//	if (result == SQLITE_DONE) {
//		// User exists, validate password
//		return new Packet; // Return a success packet
//	}
//	else {
//		std::cerr << "Error creating listing." << std::endl;
//		return nullptr;
//	}
//}	
//
//Packet* ListingRoutes::handleGetListing(Listing listing) {
//	// Check if user exists
//	SQLiteDatabase db("database.db");
//	if (!db.isOpen()) {
//		std::cerr << "Failed to open database." << std::endl;
//		return nullptr; // Return nullptr to indicate failure
//	}
//
//	// Prepare SQL query to select user by username
//	std::string query = "SELECT * FROM listings WHERE id = '" + std::to_string(listing.getId()) + "'";
//
//	// Execute query
//	sqlite3_stmt* statement;
//	if (sqlite3_prepare_v2(db.getDB(), query.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
//		std::cerr << "Error preparing SQLite statement." << std::endl;
//		return nullptr;
//	}
//
//	// Check if a row was returned (i.e., if the user exists)
//	int result = sqlite3_step(statement);
//	if (result == SQLITE_ROW) {
//		// User exists, validate password
//		return new Packet; // Return a success packet
//	}
//	else {
//		std::cerr << "Error getting listing." << std::endl;
//		return nullptr;
//	}
//}	
//
//Packet* ListingRoutes::handleUpdateListing(Listing listing) {
//	// Check if user exists
//	SQLiteDatabase db("database.db");
//	if (!db.isOpen()) {
//		std::cerr << "Failed to open database." << std::endl;
//		return nullptr; // Return nullptr to indicate failure
//	}
//
//	// Prepare SQL query to select user by username
//	std::string query = "UPDATE listings SET title = '" + listing.getTitle() + "', description = '" + listing.getDescription() + "', price = '" + std::to_string(listing.getPrice()) + "', category = '" + listing.getCategory() + "', location = '" + listing.getLocation() + "', image = '" + listing.getImage() + "' WHERE id = '" + std::to_string(listing.getId()) + "'";
//
//	// Execute query
//	sqlite3_stmt* statement;
//	if (sqlite3_prepare_v2(db.getDB(), query.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
//		std::cerr << "Error preparing SQLite statement." << std::endl;
//		return nullptr;
//	}
//
//	// Check if a row was returned (i.e., if the user exists)
//	int result = sqlite3_step(statement);
//	if (result == SQLITE_DONE) {
//		// User exists, validate password
//		return new Packet; // Return a success packet
//	}
//	else {
//		std::cerr << "Error updating listing." << std::endl;
//		return nullptr;
//	}
//}
//
//Packet* ListingRoutes::handleDeleteListing(Listing listing) {
//	// Check if user exists
//	SQLiteDatabase db("database.db");
//	if (!db.isOpen()) {
//		std::cerr << "Failed to open database." << std::endl;
//		return nullptr; // Return nullptr to indicate failure
//	}
//
//	// Prepare SQL query to select user by username
//	std::string query = "DELETE FROM listings WHERE id = '" + std::to_string(listing.getId()) + "'";
//
//	// Execute query
//	sqlite3_stmt* statement;
//	if (sqlite3_prepare_v2(db.getDB(), query.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
//		std::cerr << "Error preparing SQLite statement." << std::endl;
//		return nullptr;
//	}
//
//	// Check if a row was returned (i.e., if the user exists)
//	int result = sqlite3_step(statement);
//	if (result == SQLITE_DONE) {
//		// User exists, validate password
//		return new Packet; // Return a success packet
//	}
//	else {
//		std::cerr << "Error deleting listing." << std::endl;
//		return nullptr;
//	}
//}

