// BookingManager.h
#ifndef BOOKINGMANAGER_H
#define BOOKINGMANAGER_H

#include <vector>
#include <string>
#include <memory>
#include "Booking.h"
#include "HotelManager.h"

class BookingManager {
private:
    std::vector<std::shared_ptr<Booking>> bookings;
    std::shared_ptr<HotelManager> hotelManager;
public:
    BookingManager(std::shared_ptr<HotelManager> hm);
    void loadBookings(const std::string& filename);
    bool isRoomAvailable(const std::string& hotelName, const std::string& roomType, const std::string& checkIn, const std::string& checkOut, int requestedRooms) const;
    void saveBooking(const Booking& booking, const std::string& filename);
    std::vector<Booking> getUserBookings(const std::string& username) const;
    void addBooking(const Booking& booking);
    int getTotalRooms(const std::string& hotelName, const std::string& roomType) const;


    int getBookedRooms(const std::string& hotelName,
        const std::string& roomType,
        const std::string& checkIn,
        const std::string& checkOut) const;

};

#endif // BOOKINGMANAGER_H
