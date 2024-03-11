#ifndef LISTING_H
#define LISTING_H

#include "SQLiteDatabase.h"
#include <string>
#include <vector>
#include <ctime>
#include <sstream>

class Listing {
public:
    Listing() {};
    Listing(int id, const std::string& title, const std::string& description,
        const std::string& category, double price, const std::string& condition,
        const std::string& location, const std::string& seller, const std::string& status,
        const std::vector<std::string>& images);

    Listing(int id, const std::string& title, const std::string& description,
        const std::string& category, double price, const std::string& condition,
        const std::string& location, const std::string& seller,
        const std::string& status, const std::vector<std::string>& images,
        const std::string& listingDate)
        : _id(id), _title(title), _description(description),
        _category(category), _price(price), _condition(condition),
        _location(location), _seller(seller), _status(status),
        _images(images), _listingDate(listingDate) {}

    // Getters
    int getId() const { return _id; }
    std::string getTitle() const { return _title; }
    std::string getDescription() const { return _description; }
    std::string getCategory() const { return _category; }
    double getPrice() const { return _price; }
    std::string getCondition() const { return _condition; }
    std::string getLocation() const { return _location; }
    std::string getSeller() const { return _seller; }
    std::string getStatus() const { return _status; }
    std::vector<std::string> getImages() const { return _images; }
    std::string getImage() const {
        if (!_images.empty()) {
            return _images[0]; // Return the first image if vector is not empty
        }
        else {
            return ""; // Return an empty string if vector is empty
        }
    }
    std::string getListingDate() const { return _listingDate; }

    Listing parseListing(const std::string& listingString) {
        std::istringstream iss(listingString);
        std::string token;

        // Split the string by comma and extract values
        int id;
        std::string title, description, category, condition, location, seller, status, listingDate;
        double price;
        std::vector<std::string> images;

        getline(iss, token, ','); // id
        id = std::stoi(token);
        getline(iss, title, ',');
        getline(iss, description, ',');
        getline(iss, category, ',');
        getline(iss, token, ','); // price
        price = std::stod(token);
        getline(iss, condition, ',');
        getline(iss, location, ',');
        getline(iss, seller, ',');
        getline(iss, status, ',');
        getline(iss, token, ','); // images (comma-separated)
        std::istringstream imgStream(token);
        while (getline(imgStream, token, ';')) {
            images.push_back(token);
        }
        getline(iss, listingDate, ',');

        // Create and return a Listing object
        return Listing(id, title, description, category, price, condition, location, seller, status, images, listingDate);
    }

private:
    int _id;
    std::string _title;
    std::string _description;
    std::string _category;
    double _price;
    std::string _condition;
    std::string _location;
    std::string _seller;
    std::string _status;
    std::vector<std::string> _images;
    std::string _listingDate;
   

};

// Constructor



#endif // LISTING_H