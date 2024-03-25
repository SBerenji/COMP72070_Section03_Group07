#ifndef LISTING_H
#define LISTING_H

#include "SQLiteDatabase.h"
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include "file_utils.h"

class Listing {
public:
    Listing() {};
    Listing(int id, const std::string& title, const std::string& description,
        const std::string& category, double price, const std::string& condition,
        const std::string& location, const std::string& seller, const std::string& status,
        const std::vector<unsigned char> image);

    Listing(int id, const std::string& title, const std::string& description,
        const std::string& category, double price, const std::string& condition,
        const std::string& location, const std::string& seller,
        const std::string& status, const std::vector<unsigned char>& image,
        const std::string& listingDate)
        : _id(id), _title(title), _description(description),
        _category(category), _price(price), _condition(condition),
        _location(location), _seller(seller), _status(status),
        _image(image), _listingDate(listingDate) {}

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
    const std::vector<unsigned char> getImage() const { return _image; }
    std::string getListingDate() const { return _listingDate; }

    Listing parseListing(const std::string& listingString) {
        std::istringstream iss(listingString);
        std::string token;

        // Split the string by comma and extract values
        int id;
        std::string title, description, category, condition, location, seller, status, listingDate;
        double price;
        std::vector<unsigned char> images;

        std::getline(iss, token, ','); // id
        id = std::stoi(token);
        std::getline(iss, title, ',');
        std::getline(iss, description, ',');
        std::getline(iss, category, ',');
        std::getline(iss, token, ','); // price
        price = std::stod(token);
        std::getline(iss, condition, ',');
        std::getline(iss, location, ',');
        std::getline(iss, seller, ',');
        std::getline(iss, status, ',');
        std::getline(iss, token, ','); // images (comma-separated)
        images = base64Decode(token);
        std::getline(iss, listingDate, ',');

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
    std::vector<unsigned char> _image;
    std::string _listingDate;
   

};

// Constructor



#endif // LISTING_H