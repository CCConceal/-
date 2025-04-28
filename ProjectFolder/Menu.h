// Menu.h
#ifndef MENU_H
#define MENU_H

#include <string>
#include <memory>
#include "UserManager.h"
#include "HotelManager.h"
#include "BookingManager.h"
#include "ReviewManager.h"
#include "DiscountManager.h"

class Menu {
public:
    static void displayMainMenu();

    static void handleDisplayRooms(const HotelManager& hotelManager);
    static void handleBookRoom(HotelManager& hotelManager, BookingManager& bookingManager, DiscountManager& discountManager, std::shared_ptr<User> user);
    static void handleViewBookingHistory(BookingManager& bookingManager, HotelManager& hotelManager, DiscountManager& discountManager, std::shared_ptr<User> user);
    static void handleAddReview(const BookingManager& bookingManager, ReviewManager& reviewManager, std::shared_ptr<User> user);
    static void handleApplyDiscount(DiscountManager& discountManager, double& totalAmount);
private:

    static bool getNewDates(std::string& checkIn, std::string& checkOut, const std::string& today);
    static int selectFromList(const std::vector<std::string>& options, const std::string& prompt);
    // 1) select destination and hotels
    static bool selectDestinationAndHotel(
        const HotelManager& hotelManager,
        std::string& selectedDestination,
        std::string& selectedHotelName
    );

    // 2) get and validate the date
    static bool getCheckInOutDates(std::string& checkIn, std::string& checkOut);

    // 3) select type and enter number of people and rooms
    static bool selectRoomAndGuests(
        const HotelManager& hotelManager,
        const BookingManager& bookingManager,
        const std::string& destination,
        const std::string& hotelName,
        std::string& checkIn,       
        std::string& checkOut,
        std::string& selectedRoomType,
        int& numberOfRooms,
        int& numberOfGuests
    );
};

#endif // MENU_H
