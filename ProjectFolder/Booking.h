// Booking.h
#ifndef BOOKING_H
#define BOOKING_H
#include <string>

class Booking {
private:
    std::string bookingID;
    std::string username;
    std::string destination;
    std::string hotelName;
    std::string roomType;
    std::string checkInDate;
    std::string checkOutDate;
    int numberOfRooms;
    int numberOfGuests;
    std::string paymentMethod;
    double totalAmount;

public:
    Booking(const std::string& id, const std::string& user, const std::string& dest,
        const std::string& hotel, const std::string& room, const std::string& checkIn,
        const std::string& checkOut, int numRooms, int numGuests,
        const std::string& payment, double total);

    // Getters
    std::string getBookingID() const;
    std::string getUsername() const;
    std::string getDestination() const;
    std::string getHotelName() const;
    std::string getRoomType() const;
    std::string getCheckInDate() const;
    std::string getCheckOutDate() const;
    int getNumberOfRooms() const;
    int getNumberOfGuests() const;
    std::string getPaymentMethod() const;
    double getTotalAmount() const;

    void displayBookingSummary() const;

    // Static function for generating BookingID
    static std::string generateBookingID();
    static void setBookingCounter(int counter);

private:
    static int bookingCounter; // Added static member
};

#endif // BOOKING_H
