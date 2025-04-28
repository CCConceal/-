// Booking.cpp
#include "Booking.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Initialize static member
int Booking::bookingCounter = 1;

Booking::Booking(const std::string& id, const std::string& user, const std::string& dest,
    const std::string& hotel, const std::string& room, const std::string& checkIn,
    const std::string& checkOut, int numRooms, int numGuests,
    const std::string& payment, double total)
    : bookingID(id), username(user), destination(dest), hotelName(hotel),
    roomType(room), checkInDate(checkIn), checkOutDate(checkOut),
    numberOfRooms(numRooms), numberOfGuests(numGuests),
    paymentMethod(payment), totalAmount(total) {}

std::string Booking::getBookingID() const {
    return bookingID;
}

std::string Booking::getUsername() const {
    return username;
}

std::string Booking::getDestination() const {
    return destination;
}

std::string Booking::getHotelName() const {
    return hotelName;
}

std::string Booking::getRoomType() const {
    return roomType;
}

std::string Booking::getCheckInDate() const {
    return checkInDate;
}

std::string Booking::getCheckOutDate() const {
    return checkOutDate;
}

int Booking::getNumberOfRooms() const {
    return numberOfRooms;
}

int Booking::getNumberOfGuests() const {
    return numberOfGuests;
}

std::string Booking::getPaymentMethod() const {
    return paymentMethod;
}

double Booking::getTotalAmount() const {
    return totalAmount;
}

void Booking::displayBookingSummary() const {
    std::cout << "Booking ID: " << bookingID << "\n"
        << "Username: " << username << "\n"
        << "Destination: " << destination << "\n"
        << "Hotel: " << hotelName << "\n"
        << "Room Type: " << roomType << "\n"
        << "Check-in Date: " << checkInDate << "\n"
        << "Check-out Date: " << checkOutDate << "\n"
        << "Number of Rooms: " << numberOfRooms << "\n"
        << "Number of Guests: " << numberOfGuests << "\n"
        << "Payment Method: " << paymentMethod << "\n"
        << "Total Amount: $" << totalAmount << "\n";
}

std::string Booking::generateBookingID() {
    std::ostringstream oss;
    oss << "BK" << std::setw(3) << std::setfill('0') << bookingCounter++;
    return oss.str();
}

void Booking::setBookingCounter(int counter) {
    bookingCounter = counter;
}