// TextReview.cpp
#include "TextReview.h"
#include <iostream>
#include <sstream>
#include <fstream>

TextReview::TextReview(const std::string& rid, const std::string& bid, const std::string& user, const std::string& hotel, const std::string& cont)
    : Review(rid, bid, user, hotel), content(cont) {}

void TextReview::displayReview() const {
    std::cout << "Review ID: " << reviewID << "\n"
        << "Booking ID: " << bookingID << "\n"
        << "Username: " << username << "\n"
        << "Hotel: " << hotelName << "\n"
        << "Review: " << content << "\n";
}

std::string TextReview::getContent() const {
    return content;
}

void TextReview::saveToFile(std::ofstream& ofs) const {
    if (ofs.is_open()) {
        ofs << reviewID << "," << bookingID << "," << username << "," << hotelName << ",Text," << content << "\n";
    }
}