// Room.cpp
#include "Room.h"
#include <iostream>

// Constructor implementation
Room::Room(RoomType type, double pr, int occupancy, const std::vector<std::string>& amens, int total)
    : roomType(type), price(pr), maxOccupancy(occupancy), amenities(amens), totalRooms(total) {}

// Get the room type
RoomType Room::getRoomType() const {
    return roomType;
}

// Convert room type to string
std::string Room::getRoomTypeString() const {
    switch (roomType) {
    case RoomType::Standard: return "Standard";
    case RoomType::Deluxe: return "Deluxe";
    case RoomType::Couple: return "Couple";
    case RoomType::Suite: return "Suite";
    default: return "Unknown";
    }
}

// Get room price
double Room::getPrice() const {
    return price;
}

// Get maximum occupancy
int Room::getMaxOccupancy() const {
    return maxOccupancy;
}

// Get total number of rooms
int Room::getTotalRooms() const {
    return totalRooms;
}

// Get amenities
const std::vector<std::string>& Room::getAmenities() const {
    return amenities;
}

// Display room details without nextAvailableDate
void Room::displayDetails(int availableRooms) const {
    std::cout << "Room Type: " << getRoomTypeString() << "\n"
        << "Price: $" << price << "\n"
        << "Max Occupancy: " << maxOccupancy << "\n"
        << "Amenities: ";
    for (const auto& amenity : amenities) {
        std::cout << amenity << " ";
    }
    std::cout << "\n"
        << "Total Rooms: " << totalRooms << "\n"
        << "Available Rooms: " << availableRooms << "\n\n";
}