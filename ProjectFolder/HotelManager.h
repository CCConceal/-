// HotelManager.h
#ifndef HOTELMANAGER_H
#define HOTELMANAGER_H

#include <vector>
#include <string>
#include "Hotel.h"
#include <memory>

class BookingManager; // forward statement

class HotelManager {
private:
    std::vector<Hotel> hotels;
    std::weak_ptr<BookingManager> bookingManager; // Use weak_ptr to avoid circular references
    int selectFromList(const std::vector<std::string>& options, const std::string& prompt) const;
public:
    HotelManager();

    void setBookingManager(std::shared_ptr<BookingManager> bm);
    void loadHotels(const std::string& filename);
    void displayHotelsByDestination(const std::string& destination) const;
    void displayAvailableRooms(const std::string& checkIn, const std::string& checkOut) const;
    const std::vector<Hotel>& getHotels() const;
    void addHotel(const Hotel& hotel);

    int getTotalRooms(const std::string& hotelName, const std::string& roomType) const;
    
};

#endif // HOTELMANAGER_H
