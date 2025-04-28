// Hotel.h
#ifndef HOTEL_H
#define HOTEL_H

#include <string>
#include <vector>
#include "Room.h"

class BookingManager;


class Hotel {
private:
    std::string destination;
    std::string hotelName;
    std::vector<Room> rooms;
public:
    Hotel(const std::string& dest, const std::string& name, const std::vector<Room>& roomList);

    std::string getDestination() const;
    std::string getHotelName() const;
    const std::vector<Room>& getRooms() const;
    std::vector<Room>& getRooms();

    void displayHotelInfo() const;
    void displayAvailableRooms(const std::string& checkIn, const std::string& checkOut, const BookingManager* bookingManager) const;

    void addRoom(const Room& room);
};

#endif // HOTEL_H
