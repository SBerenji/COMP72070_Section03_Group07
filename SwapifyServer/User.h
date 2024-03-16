#ifndef USER_H
#define USER_H
#pragma once
#include "file_utils.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include "bcrypt.h"
#include "SQLiteDatabase.h"
class User {
private:
	int id_;
	std::string emailAddress_;
	std::string password_;
	std::string firstName_;
	std::string lastName_;
	std::vector<unsigned char> profilePicture_;
	time_t dateCreated_;
	std::string accountStatus_;

public:
	User(int id, const std::string& emailAddress, const std::string& password,
		const std::string& firstName, const std::string& lastName,
		const std::vector<unsigned char>& profilePicture, const std::string& dateCreated,
		const std::string& accountStatus)
		: id_(id), emailAddress_(emailAddress), password_(password),
		firstName_(firstName), lastName_(lastName),
		profilePicture_(profilePicture), accountStatus_(accountStatus) {
		// Convert date string to time_t
		std::istringstream iss(dateCreated);
		std::tm tm = {};
		iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
		dateCreated_ = mktime(&tm);
	}

	User() = default;

	char* serializeUser(const User& user) {
		// Serialize user fields into a string
		std::ostringstream oss;
		oss << user.id_ << ','
			<< user.emailAddress_ << ','
			<< user.password_ << ','
			<< user.firstName_ << ','
			<< user.lastName_ << ','
			<< base64Encode(user.profilePicture_) << ','
			<< user.dateCreated_ << ','
			<< user.accountStatus_;

		std::string serializedString = oss.str();

		// Allocate memory for a character array to store the serialized data
		char* serializedData = new char[serializedString.length() + 1]; // +1 for null terminator

		// Copy the serialized string characters to the character array
		std::copy(serializedString.begin(), serializedString.end(), serializedData);

		// Add null terminator
		serializedData[serializedString.length()] = '\0';

		return serializedData;
	}

	User parseUser(const std::string& userString) {
		std::istringstream iss(userString);
		std::string token;

		// Split the string by comma and extract values
		std::string emailAddress, password, firstName, lastName, dateCreated, accountStatus, profilePictureBase64;
		std::getline(iss, emailAddress, ',');
		std::getline(iss, password, ',');
		std::getline(iss, firstName, ',');
		std::getline(iss, lastName, ',');
		std::getline(iss, profilePictureBase64, ',');
		std::getline(iss, dateCreated, ',');
		std::getline(iss, accountStatus, ',');

		// Decode base64 string into byte array
		std::vector<unsigned char> profilePicture = base64Decode(profilePictureBase64);

		// Create and return a User object
		return User(0, emailAddress, password, firstName, lastName, profilePicture, dateCreated, accountStatus);
	}


	std::string getIdAsString() const {
		// Convert id_ to string representation
		return std::to_string(id_);
	}

	time_t getDateCreated() const {
		return dateCreated_;
	}
	std::string getDateCreatedAsString() const {
		std::tm timeinfo;
		localtime_s(&timeinfo, &dateCreated_);
		std::stringstream ss;
		ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
		return ss.str();
	}
	const std::string& getPassword() const {
		return password_;
	}

	const std::string& getFirstName() const {
		return firstName_;
	}

	const std::string& getLastName() const {
		return lastName_;
	}

	const std::string& getEmailAddress() const {
		return emailAddress_;
	}

	const std::string& getProfilePicture() const {
		return base64Encode(profilePicture_);
	}


	const std::string& getAccountStatus() const {
		return accountStatus_;
	}

	void setEmailAddress(const std::string& emailAddress) {
		emailAddress_ = emailAddress;
	}

	void setPassword(const std::string& password) {
		password_ = password;
	}

	void setFirstName(const std::string& firstName) {
		firstName_ = firstName;
	}

	void setLastName(const std::string& lastName) {
		lastName_ = lastName;
	}

	void setProfilePicture(const std::string& profilePicture) {
		profilePicture_ = base64Decode(profilePicture);
	}

	void setAccountStatus(const std::string& accountStatus) {
		accountStatus_ = accountStatus;
	}

	User getUserFromDatabase(sqlite3* db, int userId) {
		User user; // Create a default user object

		// Define the SQL query to fetch user data
		std::string query = "SELECT id, emailAddress, password, firstName, lastName, profilePicture, dateCreated, accountStatus FROM Users WHERE id = ?";

		// Prepare the SQL statement
		sqlite3_stmt* stmt;
		int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
		if (rc != SQLITE_OK) {
			std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
			return user; // Return default user
		}

		// Bind the user ID parameter to the SQL statement
		sqlite3_bind_int(stmt, 1, userId);

		// Execute the SQL statement
		rc = sqlite3_step(stmt);
		if (rc == SQLITE_ROW) {
			// Extract data from the query result and populate the user object
			user.id_ = sqlite3_column_int(stmt, 0);
			user.emailAddress_ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
			user.password_ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
			user.firstName_ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
			user.lastName_ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

			// Extract profile picture data as a binary blob
			const void* profilePictureBlob = sqlite3_column_blob(stmt, 5);
			int profilePictureSize = sqlite3_column_bytes(stmt, 5);
			user.profilePicture_.assign(static_cast<const unsigned char*>(profilePictureBlob), static_cast<const unsigned char*>(profilePictureBlob) + profilePictureSize);

			user.dateCreated_ = sqlite3_column_int(stmt, 6); // Assuming dateCreated is stored as a UNIX timestamp
			user.accountStatus_ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
		}
		else if (rc != SQLITE_DONE) {
			std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
		}

		// Finalize the SQL statement
		sqlite3_finalize(stmt);

		return user;
	}

	bool validatePassword(const std::string& inputPassword, const std::string& storedPassword) {
		// Placeholder function for password validation
		// Compare the input password directly with the stored password (plaintext comparison)
		return inputPassword == storedPassword;
	}

	std::string generateHash(const std::string& input) {
		// Placeholder function for generating a hash (returns the input string itself)
		return input;
	}


};

#endif