// BookingManager.cpp
#include "BookingManager.h"
#include "Booking.h"       
#include "HotelManager.h"   
#include <fstream>
#include <sstream>
#include <iostream>
#include "Utils.h"
#include <stdexcept>
#include <iomanip>
#include <ctime>

// Constructor
BookingManager::BookingManager(std::shared_ptr<HotelManager> hm) : hotelManager(hm) {

}

// Add a booking to the list
void BookingManager::addBooking(const Booking& booking) {
    bookings.emplace_back(std::make_shared<Booking>(booking));
}

// Load bookings from CSV
void BookingManager::loadBookings(const std::string& filename) {
    std::cerr << "[BookingManager::loadBookings] Starting to load bookings from: " << filename << "\n";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[BookingManager::loadBookings] Unable to open bookings file: " << filename << "\n";
        throw std::runtime_error("Failed to open bookings file.");
    }

    std::string line;
    int lineNumber = 0;

    // Read and skip header line
    if (std::getline(file, line)) {
        lineNumber++;
        std::cerr << "[BookingManager::loadBookings] Skipping header line: " << line << "\n";

        // Check and remove BOM if present
        if (line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF) {
            std::cerr << "[BookingManager::loadBookings] Detected and removing BOM.\n";
            line.erase(0, 3);
        }
    }

    int maxNumericID = 0;  // Used to record the largest value of the numeric portion following the BK

    while (std::getline(file, line)) {
        lineNumber++;
        std::cerr << "[BookingManager::loadBookings] Processing line " << lineNumber << ": " << line << "\n";
        try {
            std::istringstream iss(line);
            std::string bookingID, username, destination, hotelName, roomTypeStr, checkIn, checkOut;
            std::string numRoomsStr, numGuestsStr, paymentMethod, totalAmountStr;

            // Split fields by comma
            std::getline(iss, bookingID, ',');
            std::getline(iss, username, ',');
            std::getline(iss, destination, ',');
            std::getline(iss, hotelName, ',');
            std::getline(iss, roomTypeStr, ',');
            std::getline(iss, checkIn, ',');
            std::getline(iss, checkOut, ',');
            std::getline(iss, numRoomsStr, ',');
            std::getline(iss, numGuestsStr, ',');
            std::getline(iss, paymentMethod, ',');
            std::getline(iss, totalAmountStr, ',');

            // Debug log: print parsed fields
            std::cerr << "[BookingManager::loadBookings] Parsed fields - bookingID: " << bookingID
                << ", username: " << username
                << ", destination: " << destination
                << ", hotelName: " << hotelName
                << ", roomType: " << roomTypeStr
                << ", checkIn: " << checkIn
                << ", checkOut: " << checkOut
                << ", numRoomsStr: '" << numRoomsStr
                << "', numGuestsStr: '" << numGuestsStr
                << "', paymentMethod: " << paymentMethod
                << ", totalAmountStr: '" << totalAmountStr << "'\n";

            // Convert strings to appropriate data types
            int numRooms = std::stoi(numRoomsStr);
            int numGuests = std::stoi(numGuestsStr);
            double totalAmount = std::stod(totalAmountStr);

            // Create Booking object
            Booking booking(bookingID, username, destination, hotelName, roomTypeStr,
                checkIn, checkOut, numRooms, numGuests, paymentMethod, totalAmount);

            // Add to bookings list
            addBooking(booking);
            std::cerr << "[BookingManager::loadBookings] Added new booking: " << bookingID << "\n";

            // Parse the bookingID as ¡®BK001¡¯.
// If it matches, extract the numeric part and compare it to maxNumericID.
            if (bookingID.size() > 2 && bookingID.rfind("BK", 0) == 0) {
                // rfind("BK", 0) == 0 Indicates that the prefix is ¡®BK¡¯.
                std::string numericPart = bookingID.substr(2); //Cut off ¡®BK¡¯

                int currentID = std::stoi(numericPart);
                if (currentID > maxNumericID) {
                    maxNumericID = currentID;
                }
            }

        }
        catch (const std::invalid_argument& e) {
            std::cerr << "[BookingManager::loadBookings] Invalid numeric argument in line " << lineNumber << ":\n"
                << "  " << line << "\n"
                << "  Reason: " << e.what() << "\n";
            continue;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "[BookingManager::loadBookings] Out of range error in line " << lineNumber << ":\n"
                << "  " << line << "\n"
                << "  Reason: " << e.what() << "\n";
            continue;
        }
        catch (...) {
            std::cerr << "[BookingManager::loadBookings] Unknown exception while parsing line "
                << lineNumber << ":\n" << "  " << line << "\n";
            continue;
        }
    }

    file.close();
    std::cerr << "[BookingManager::loadBookings] Finished loading: " << filename << "\n";
    // Update the static counter here
    if (maxNumericID > 0) {
        Booking::setBookingCounter(maxNumericID + 1);
        std::cerr << "[BookingManager::loadBookings] Set Booking::bookingCounter to " << (maxNumericID + 1) << "\n";
    }

    std::cerr << "========== The above is [BookingManager] debugging information (the actual system will not show, here for the convenience of showing the error handling function of this project) ==========\n\n";
}


// Check if room is available
bool BookingManager::isRoomAvailable(const std::string& hotelName, const std::string& roomType,
    const std::string& checkIn, const std::string& checkOut, int requestedRooms) const {
    // Convert checkIn and checkOut to time_t
    std::tm tmCheckIn = {};
    std::tm tmCheckOut = {};
    std::istringstream ssCheckIn(checkIn);
    std::istringstream ssCheckOut(checkOut);
    ssCheckIn >> std::get_time(&tmCheckIn, "%Y-%m-%d");
    ssCheckOut >> std::get_time(&tmCheckOut, "%Y-%m-%d");
    time_t timeCheckIn = mktime(&tmCheckIn);
    time_t timeCheckOut = mktime(&tmCheckOut);

    int bookedRooms = 0;
    for (const auto& booking : bookings) { // booking is std::shared_ptr<Booking>
        if (booking->getHotelName() == hotelName && booking->getRoomType() == roomType) {
            // Convert booking dates
            std::tm tmBookingIn = {};
            std::tm tmBookingOut = {};
            std::istringstream ssBookingIn(booking->getCheckInDate());
            std::istringstream ssBookingOut(booking->getCheckOutDate());
            ssBookingIn >> std::get_time(&tmBookingIn, "%Y-%m-%d");
            ssBookingOut >> std::get_time(&tmBookingOut, "%Y-%m-%d");
            time_t timeBookingIn = mktime(&tmBookingIn);
            time_t timeBookingOut = mktime(&tmBookingOut);

            // Check date overlap
            if (!(difftime(timeCheckOut, timeBookingIn) <= 0 || difftime(timeCheckIn, timeBookingOut) >= 0)) {
                bookedRooms += booking->getNumberOfRooms();
            }
        }
    }
    int totalRooms = hotelManager->getTotalRooms(hotelName, roomType);
    return (bookedRooms + requestedRooms) <= totalRooms;
}

int BookingManager::getBookedRooms(const std::string& hotelName,
    const std::string& roomType,
    const std::string& checkIn,
    const std::string& checkOut) const
{
    // Convert a string date to time_t
    std::tm tmCheckIn = {};
    std::tm tmCheckOut = {};
    {
        std::istringstream ssIn(checkIn);
        ssIn >> std::get_time(&tmCheckIn, "%Y-%m-%d");
        std::istringstream ssOut(checkOut);
        ssOut >> std::get_time(&tmCheckOut, "%Y-%m-%d");
    }
    time_t timeCheckIn = mktime(&tmCheckIn);
    time_t timeCheckOut = mktime(&tmCheckOut);

    int bookedRooms = 0;
    for (const auto& b : bookings) { // b is std::shared_ptr<Booking>
        // Match the same hotel, same type
        if (b->getHotelName() == hotelName && b->getRoomType() == roomType) {
            // Take the check-in/check-out for each booking and do a date overlap judgement.
            std::tm tmBIn = {};
            std::tm tmBOut = {};
            {
                std::istringstream ssBIn(b->getCheckInDate());
                ssBIn >> std::get_time(&tmBIn, "%Y-%m-%d");
                std::istringstream ssBOut(b->getCheckOutDate());
                ssBOut >> std::get_time(&tmBOut, "%Y-%m-%d");
            }
            time_t tBIn = mktime(&tmBIn);
            time_t tBOut = mktime(&tmBOut);

            bool overlap = !(difftime(timeCheckOut, tBIn) <= 0 || difftime(timeCheckIn, tBOut) >= 0);
            if (overlap) {
                bookedRooms += b->getNumberOfRooms();
            }
        }
    }
    return bookedRooms;
}



// Save a booking to CSV
void BookingManager::saveBooking(const Booking& booking, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Unable to open bookings file: " << filename << "\n";
        return;
    }
    file << booking.getBookingID() << ","
        << booking.getUsername() << ","
        << booking.getDestination() << ","
        << booking.getHotelName() << ","
        << booking.getRoomType() << ","
        << booking.getCheckInDate() << ","
        << booking.getCheckOutDate() << ","
        << booking.getNumberOfRooms() << ","
        << booking.getNumberOfGuests() << ","
        << booking.getPaymentMethod() << ","
        << booking.getTotalAmount() << "\n";
    file.close();
}

// Get user bookings
std::vector<Booking> BookingManager::getUserBookings(const std::string& username) const {
    std::vector<Booking> userBookings;
    for (const auto& booking : bookings) { // booking is std::shared_ptr<Booking>
        if (booking->getUsername() == username) {
            userBookings.emplace_back(*booking);
        }
    }
    return userBookings;
}

// Get total rooms for a specific hotel and room type
int BookingManager::getTotalRooms(const std::string& hotelName, const std::string& roomType) const {
    // Assuming HotelManager provides a method to get total rooms
    // Since HotelManager is a shared_ptr, we need to access it
    if (hotelManager) {
        return hotelManager->getTotalRooms(hotelName, roomType);
    }
    return 0;
}
