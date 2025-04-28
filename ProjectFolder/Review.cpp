// Review.cpp
#include "Review.h"
#include<iomanip>
#include<iostream>
#include<sstream>

int Review::reviewCounter = 1; // Initialize static member

Review::Review(const std::string& rid, const std::string& bid, const std::string& user, const std::string& hotel)
    : reviewID(rid), bookingID(bid), username(user), hotelName(hotel) {}

std::string Review::getReviewID() const {
    return reviewID;
}

std::string Review::getBookingID() const {
    return bookingID;
}

std::string Review::getUsername() const {
    return username;
}

std::string Review::getHotelName() const {
    return hotelName;
}

std::string Review::generateReviewID() {
    std::ostringstream oss;
    oss << "RV" << std::setw(3) << std::setfill('0') << reviewCounter++;
    return oss.str();
}

void Review::setReviewCounter(int counter) {
    reviewCounter = counter;
}