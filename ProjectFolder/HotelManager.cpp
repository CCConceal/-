// HotelManager.cpp
#include "HotelManager.h"
#include "BookingManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include "Utils.h"

HotelManager::HotelManager() : bookingManager() {

}

void HotelManager::addHotel(const Hotel& hotel) {
    hotels.emplace_back(hotel);
}

void HotelManager::setBookingManager(std::shared_ptr<BookingManager> bm) {

    bookingManager = bm;
}

void HotelManager::loadHotels(const std::string& filename) {
    std::cerr << "[HotelManager::loadHotels] Starting to load hotels from: " << filename << "\n";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[HotelManager::loadHotels] Unable to open hotels file: " << filename << "\n";
        throw std::runtime_error("Failed to open hotels file.");
    }

    std::string line;
    int lineNumber = 0;
    // Read and skip header rows
    if (std::getline(file, line)) {
        lineNumber++;
        std::cerr << "[HotelManager::loadHotels] Skipping header line: " << line << "\n";

        // Check and remove the BOM
        if (line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF) {
            std::cerr << "[HotelManager::loadHotels] Detected and removing BOM.\n";
            line.erase(0, 3);
        }
    }

    while (std::getline(file, line)) {
        lineNumber++;
        std::cerr << "[HotelManager::loadHotels] Processing line " << lineNumber << ": " << line << "\n";
        try {
            std::istringstream iss(line);
            std::string destination, hotelName, roomTypeStr, priceStr, occupancyStr, amenitiesStr, totalRoomsStr;

            // Split fields by comma
            std::getline(iss, destination, ',');
            std::getline(iss, hotelName, ',');
            std::getline(iss, roomTypeStr, ',');
            std::getline(iss, priceStr, ',');
            std::getline(iss, occupancyStr, ',');
            std::getline(iss, amenitiesStr, ',');
            std::getline(iss, totalRoomsStr, ',');

            // Debug log: print parsed fields
            std::cerr << "[HotelManager::loadHotels] Parsed fields - destination: " << destination
                << ", hotelName: " << hotelName
                << ", roomTypeStr: " << roomTypeStr
                << ", priceStr: " << priceStr
                << ", occupancyStr: " << occupancyStr
                << ", amenitiesStr: " << amenitiesStr
                << ", totalRoomsStr: " << totalRoomsStr << "\n";


            double price = std::stod(priceStr);
            int maxOccupancy = std::stoi(occupancyStr);
            int totalRooms = std::stoi(totalRoomsStr);

            // Creation of the RoomType enumeration
            RoomType roomType;
            if (roomTypeStr == "Standard") roomType = RoomType::Standard;
            else if (roomTypeStr == "Deluxe") roomType = RoomType::Deluxe;
            else if (roomTypeStr == "Couple") roomType = RoomType::Couple;
            else if (roomTypeStr == "Suite") roomType = RoomType::Suite;
            else {
                std::cerr << "[HotelManager::loadHotels] Unknown room type '" << roomTypeStr
                    << "' in line " << lineNumber << ". Skipping.\n";
                continue;
            }

            // Analyse amenities
            std::vector<std::string> amenities;
            std::istringstream amensStream(amenitiesStr);
            std::string amenity;
            while (std::getline(amensStream, amenity, '|')) { //   amenities separated by ¡®|¡¯
                amenities.push_back(amenity);
            }

            // Create a Room object.
            Room room(roomType, price, maxOccupancy, amenities, totalRooms);

            // Check if the hotel already exists
            auto it = std::find_if(hotels.begin(), hotels.end(), [&](const Hotel& h) {
                return h.getHotelName() == hotelName && h.getDestination() == destination;
                });

            if (it != hotels.end()) {
                // If the hotel already exists, add the room type
                it->addRoom(room);
                std::cerr << "[HotelManager::loadHotels] Added room type: " << roomTypeStr
                    << " to existing hotel: " << hotelName << "\n";
            }
            else {
                // If the hotel does not exist, create a new hotel and add a room type
                std::vector<Room> roomList = { room };
                Hotel newHotel(destination, hotelName, roomList);
                addHotel(newHotel);
                std::cerr << "[HotelManager::loadHotels] Added new hotel: " << hotelName
                    << " in " << destination << "\n";
            }
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "[HotelManager::loadHotels] Invalid numeric argument in line " << lineNumber << ":\n"
                << "  " << line << "\n"
                << "  Reason: " << e.what() << "\n";
            continue;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "[HotelManager::loadHotels] Out of range error in line " << lineNumber << ":\n"
                << "  " << line << "\n"
                << "  Reason: " << e.what() << "\n";
            continue;
        }
        catch (...) {
            std::cerr << "[HotelManager::loadHotels] Unknown exception while parsing line " << lineNumber << ":\n"
                << "  " << line << "\n";
            continue;
        }
    }

    file.close();
    std::cerr << "[HotelManager::loadHotels] Finished loading: " << filename << "\n";
    std::cerr << "========== The above is [HotelManager] debugging information (the actual system will not show, here for the convenience of showing the error handling function of this project) ==========\n\n";
}

void HotelManager::displayHotelsByDestination(const std::string& destination) const {
    // 1. List all unique destinations
    std::vector<std::string> uniqueDestinations;
    for (const auto& hotel : hotels) {
        if (std::find(uniqueDestinations.begin(), uniqueDestinations.end(), hotel.getDestination()) == uniqueDestinations.end()) {
            uniqueDestinations.emplace_back(hotel.getDestination());
        }
    }

    if (uniqueDestinations.empty()) {
        std::cout << "There is no available destination\n";
        return;
    }

    // 2. Use auxiliary functions to select destinations
    std::cout << "Please select a destination:\n";
    int destIndex = selectFromList(uniqueDestinations, "enter a number to select destination: ");
    if (destIndex == -1) {
        return;
    }
    std::string selectedDestination = uniqueDestinations[destIndex];

    // 3. List all hotels under the selected destination
    std::vector<std::string> hotelsInDestination;
    for (const auto& hotel : hotels) {
        if (hotel.getDestination() == selectedDestination) {
            hotelsInDestination.emplace_back(hotel.getHotelName());
        }
    }

    if (hotelsInDestination.empty()) {
        std::cout << "In " << selectedDestination << " ,not found\n";
        return;
    }

    // 4. Selecting a hotel using auxiliary functions
    std::cout << "please select a hotel:\n";
    int hotelIndex = selectFromList(hotelsInDestination, "enter a number to select hotel: ");
    if (hotelIndex == -1) {
        return;
    }
    std::string selectedHotelName = hotelsInDestination[hotelIndex];

    // 5. shows all room types for the selected hotel
    for (const auto& hotel : hotels) {
        if (hotel.getHotelName() == selectedHotelName && hotel.getDestination() == selectedDestination) {
            hotel.displayHotelInfo();
            break;
        }
    }
}

void HotelManager::displayAvailableRooms(const std::string& checkIn,
    const std::string& checkOut) const
{
    // 1. Get all unique destinations
    std::vector<std::string> uniqueDestinations;
    for (const auto& hotel : hotels) {
        if (std::find(uniqueDestinations.begin(), uniqueDestinations.end(), hotel.getDestination()) == uniqueDestinations.end()) {
            uniqueDestinations.emplace_back(hotel.getDestination());
        }
    }

    if (uniqueDestinations.empty()) {
        std::cout << "There are no available destinations.\n";
        return;
    }

    // 2. displays a list of destinations and lets the user select them
    std::cout << "Please select a destination:\n";
    for (size_t i = 0; i < uniqueDestinations.size(); ++i) {
        std::cout << i + 1 << ". " << uniqueDestinations[i] << "\n";
    }

    int destChoice = 0;
    while (true) {
        std::cout << "Enter the number corresponding to your destination: ";
        if (!(std::cin >> destChoice)) {
            std::cout << "Invalid input, please enter a number.\n";
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
            continue;
        }
        if (destChoice < 1 || destChoice > static_cast<int>(uniqueDestinations.size())) {
            std::cout << "Out of range, please select again.\n";
            continue;
        }
        break;
    }
    std::string selectedDestination = uniqueDestinations[destChoice - 1];
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear Input Buffer

    // 3. Get all hotels under the selected destination
    std::vector<std::string> hotelsInDestination;
    for (const auto& hotel : hotels) {
        if (hotel.getDestination() == selectedDestination) {
            hotelsInDestination.emplace_back(hotel.getHotelName());
        }
    }

    if (hotelsInDestination.empty()) {
        std::cout << "No hotels found in " << selectedDestination << ".\n";
        return;
    }

    // 4. Display a list of hotels and let the user choose
    std::cout << "Please select a hotel:\n";
    for (size_t i = 0; i < hotelsInDestination.size(); ++i) {
        std::cout << i + 1 << ". " << hotelsInDestination[i] << "\n";
    }

    int hotelChoice = 0;
    while (true) {
        std::cout << "Enter the number corresponding to your hotel: ";
        if (!(std::cin >> hotelChoice)) {
            std::cout << "Invalid input, please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (hotelChoice < 1 || hotelChoice > static_cast<int>(hotelsInDestination.size())) {
            std::cout << "Out of range, please select again.\n";
            continue;
        }
        break;
    }
    std::string selectedHotelName = hotelsInDestination[hotelChoice - 1];
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 5. Show available rooms in the selected hotel
    for (const auto& hotel : hotels) {
        if (hotel.getHotelName() == selectedHotelName && hotel.getDestination() == selectedDestination) {
            std::cerr << "[HotelManager] Displaying available rooms for hotel: " << hotel.getHotelName() << "\n";
            if (auto bm = bookingManager.lock()) {
                hotel.displayAvailableRooms(checkIn, checkOut, bm.get());
            }
            else {
                std::cerr << "[HotelManager] BookingManager is not set.\n";
            }
            break;
        }
    }
}

const std::vector<Hotel>& HotelManager::getHotels() const {
    return hotels;
}

int HotelManager::getTotalRooms(const std::string& hotelName, const std::string& roomType) const {
    for (const auto& hotel : hotels) {
        if (hotel.getHotelName() == hotelName) {
            for (const auto& room : hotel.getRooms()) {
                if (room.getRoomTypeString() == roomType) {
                    return room.getTotalRooms();
                }
            }
        }
    }
    std::cerr << "[HotelManager::getTotalRooms] Hotel or room type not found: "
        << hotelName << ", " << roomType << "\n";
    return 0;
}


int HotelManager::selectFromList(const std::vector<std::string>& options, const std::string& prompt) const {
    if (options.empty()) {
        std::cout << "There is no available option\n";
        return -1;
    }

    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ". " << options[i] << "\n";
    }

    int choice = 0;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> choice)) {
            std::cout << "invalid input, please enter a number\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (choice < 1 || choice > static_cast<int>(options.size())) {
            std::cout << "Out of range, please select again\n";
            continue;
        }
        break;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
    return choice - 1; // Return index
}
