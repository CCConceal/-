// Room.h
#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>

enum class RoomType {
    Standard,
    Deluxe,
    Couple,
    Suite
};

class Room {
private:
    RoomType roomType;
    double price;
    int maxOccupancy;
    std::vector<std::string> amenities;
    int totalRooms;        // Total number of rooms available in the hotel

public:
    // Constructor without nextAvailableDate
    Room(RoomType type, double pr, int occupancy, const std::vector<std::string>& amens, int total);

    // Getters
    RoomType getRoomType() const;
    std::string getRoomTypeString() const;
    double getPrice() const;
    int getMaxOccupancy() const;
    int getTotalRooms() const;
    const std::vector<std::string>& getAmenities() const;

    // Display room details without nextAvailableDate
    void displayDetails(int availableRooms) const;
};

#endif // ROOM_H
