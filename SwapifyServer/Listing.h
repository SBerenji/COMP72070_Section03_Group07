#ifndef LISTING_H
#define LISTING_H

#include <string>
#include <vector>
#include <ctime>

class Listing {
public:
    Listing();
    Listing(int id, const std::string& title, const std::string& description,
        const std::string& category, double price, const std::string& condition,
        const std::string& location, const std::string& seller, const std::string& status,
        const std::vector<std::string>& images);

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
    std::string getListingDate() const { return _listingDate; }

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
Listing::Listing() {}

Listing::Listing(int id, const std::string& title, const std::string& description,
    const std::string& category, double price, const std::string& condition,
    const std::string& location, const std::string& seller, const std::string& status,
    const std::vector<std::string>& images)
    : _id(id), _title(title), _description(description), _category(category), _price(price),
    _condition(condition), _location(location), _seller(seller), _status(status), _images(images) {
    // Set current date as the listing date
    std::time_t currentTime = std::time(nullptr);
    _listingDate = std::asctime(std::localtime(&currentTime));
}

#endif // LISTING_H