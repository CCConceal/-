// StarReview.cpp
#include "StarReview.h"
#include <iostream>
#include <fstream>

StarReview::StarReview(const std::string& rid, const std::string& bid, const std::string& user, const std::string& hotel, int rate)
    : Review(rid, bid, user, hotel), rating(rate) {}

void StarReview::displayReview() const {
    std::cout << "Review ID: " << reviewID << "\n"
        << "Booking ID: " << bookingID << "\n"
        << "Username: " << username << "\n"
        << "Hotel: " << hotelName << "\n"
        << "Rating: ";
    for (int i = 0; i < rating; ++i) {
        std::cout << "¡ï"; 
    }
    for (int i = rating; i < 5; ++i) {
        std::cout << "¡î"; 
    }
    std::cout << "\n";
}

int StarReview::getRating() const {
    return rating;
}

void StarReview::saveToFile(std::ofstream& ofs) const {
    if (ofs.is_open()) {
        ofs << reviewID << "," << bookingID << "," << username << "," << hotelName << ",Star," << rating << "\n";
    }
}