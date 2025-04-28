// Hotel.cpp
#include "Hotel.h"
#include "BookingManager.h" 
#include <iostream>

Hotel::Hotel(const std::string& dest, const std::string& name, const std::vector<Room>& roomList)
    : destination(dest), hotelName(name), rooms(roomList) {}

std::string Hotel::getDestination() const {
    return destination;
}

std::string Hotel::getHotelName() const {
    return hotelName;
}

std::vector<Room>& Hotel::getRooms() {
    return rooms;
}

const std::vector<Room>& Hotel::getRooms() const {
    return rooms;
}

void Hotel::addRoom(const Room& room) {
    rooms.emplace_back(room);
}

void Hotel::displayHotelInfo() const {
    std::cout << "Hotel: " << hotelName << " | Destination: " << destination << "\n";
    for (const auto& room : rooms) {
      
        std::cout << "Room Type: " << room.getRoomTypeString() << "\n"
            << "Price: $" << room.getPrice() << "\n"
            << "Max Occupancy: " << room.getMaxOccupancy() << "\n"
            << "Amenities: ";
        for (const auto& amenity : room.getAmenities()) {
            std::cout << amenity << " ";
        }
        std::cout << "\n"
            << "Total Rooms: " << room.getTotalRooms() << "\n\n";
        std::cout << "---------------------------\n";
    }
}

void Hotel::displayAvailableRooms(const std::string& checkIn, const std::string& checkOut, const BookingManager* bookingManager) const {
    if (!bookingManager) {
        std::cerr << "[Hotel::displayAvailableRooms] BookingManager is null.\n";
        return;
    }
    std::cout << "Available rooms from " << checkIn << " to " << checkOut << ":\n";
    for (const auto& room : rooms) {
        // get totalRoom
        int total = room.getTotalRooms();

        // get the room that booked
        int booked = bookingManager->getBookedRooms(
            hotelName,
            room.getRoomTypeString(),
            checkIn,
            checkOut
        );

        // calculate the number of available rooms
        int available = total - booked;

        if (available > 0) {
            room.displayDetails(available);
            std::cout << "---------------------------\n";
        }
    }
}