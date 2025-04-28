// Menu.cpp

#include "Menu.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <set>
#include <limits>
#include <algorithm>

#include "DiscountManager.h"
#include "TextReview.h"
#include "StarReview.h"
#include "Utils.h"
#include "Booking.h"


#include "Payment.h"
#include "CreditCardPayment.h"
#include "EWalletPayment.h"
#include "CashPayment.h"

#include "MemberUser.h"
#include "LoyaltyDiscount.h"

void Menu::displayMainMenu() {
    std::cout << "============================================\n";
    std::cout << "============================================\n";
    std::cout << "===== Hotel Booking System Main Menu =====\n"
        << "1. Display Room Types\n"
        << "2. Book Room\n"
        << "3. View Booking History\n"
        << "4. Add Review\n"
        << "5. Exit\n"
        << "Please select an option: ";
}

bool Menu::getNewDates(std::string& checkIn, std::string& checkOut, const std::string& today) {
    while (true) {
        std::cout << "Enter check-in date (YYYY-MM-DD) or 'q' to return to previous menu: ";
        std::cin >> checkIn;
        if (checkIn == "q" || checkIn == "Q") {
            return false;
        }
        if (!validateDate(checkIn)) {
            std::cout << "Invalid date format or invalid date. Please try again.\n";
            continue;
        }
        if (!isDateNotInPast(checkIn, today)) {
            std::cout << "Check-in date must be today or a future date.\n";
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Enter check-out date (YYYY-MM-DD) or 'q' to return to previous menu: ";
        std::cin >> checkOut;
        if (checkOut == "q" || checkOut == "Q") {
            return false;
        }
        if (!validateDate(checkOut)) {
            std::cout << "Invalid date format or invalid date. Please try again.\n";
            continue;
        }
        if (!isCheckInBeforeCheckOut(checkIn, checkOut)) {
            std::cout << "Check-out date must be after check-in date.\n";
            continue;
        }
        break;
    }
    return true;
}

void Menu::handleDisplayRooms(const HotelManager& hotelManager) {
    // 1. Get all unique destinations
    std::vector<std::string> uniqueDestinations;
    for (const auto& hotel : hotelManager.getHotels()) {
        if (std::find(uniqueDestinations.begin(), uniqueDestinations.end(), hotel.getDestination()) == uniqueDestinations.end()) {
            uniqueDestinations.emplace_back(hotel.getDestination());
        }
    }

    if (uniqueDestinations.empty()) {
        std::cout << "There are no available destinations.\n";
        return;
    }

    // 2. Use helper function to select destination
    std::cout << "Please select a destination:\n";
    int destIndex = selectFromList(uniqueDestinations, "Enter the number corresponding to your destination: ");
    if (destIndex == -1) {
        return;
    }
    std::string selectedDestination = uniqueDestinations[destIndex];

    // 3. Get all hotels in the selected destination
    std::vector<std::string> hotelsInDestination;
    for (const auto& hotel : hotelManager.getHotels()) {
        if (hotel.getDestination() == selectedDestination) {
            hotelsInDestination.emplace_back(hotel.getHotelName());
        }
    }

    if (hotelsInDestination.empty()) {
        std::cout << "No hotels found in " << selectedDestination << ".\n";
        return;
    }

    // 4. Use helper function to select hotel
    std::cout << "Please select a hotel:\n";
    int hotelIndex = selectFromList(hotelsInDestination, "Enter the number corresponding to your hotel: ");
    if (hotelIndex == -1) {
        return;
    }
    std::string selectedHotelName = hotelsInDestination[hotelIndex];

    // 5. Display room type information for the selected hotel
    for (const auto& hotel : hotelManager.getHotels()) {
        if (hotel.getHotelName() == selectedHotelName && hotel.getDestination() == selectedDestination) {
            hotel.displayHotelInfo();
            break;
        }
    }
}


void Menu::handleBookRoom(
    HotelManager& hotelManager,
    BookingManager& bookingManager,
    DiscountManager& discountManager,
    std::shared_ptr<User> user
) {
    // 1) User Selects Destinations & Hotels
    std::string selectedDestination, selectedHotelName;
    if (!selectDestinationAndHotel(hotelManager, selectedDestination, selectedHotelName)) {
        return;
    }

    // 2) User enters and verifies check-in and check-out dates
    std::string checkIn, checkOut;
    if (!getCheckInOutDates(checkIn, checkOut)) {
        return;
    }

    // 3) User selects room type & enters number of rooms & guests
    std::string selectedRoomType;
    int numberOfRooms = 0;
    int numberOfGuests = 0;
    if (!selectRoomAndGuests(
        hotelManager,
        bookingManager,
        selectedDestination,
        selectedHotelName,
        checkIn,
        checkOut,
        selectedRoomType,
        numberOfRooms,
        numberOfGuests
    )) {
        return;
    }

    // -----------------
    // Here's the second half: Calculate Price -> Promo Code -> Member Discount -> Payment -> Generate Booking
    // -----------------

    // 1) get room price
    double roomPrice = 0.0;
    // get all rooms
    std::vector<Room> allRooms;
    for (const auto& h : hotelManager.getHotels()) {
        if (h.getHotelName() == selectedHotelName && h.getDestination() == selectedDestination) {
            allRooms = h.getRooms();
            break;
        }
    }
    // Find the price of the corresponding room type
    for (const auto& rm : allRooms) {
        if (rm.getRoomTypeString() == selectedRoomType) {
            roomPrice = rm.getPrice();
            break;
        }
    }
    if (roomPrice <= 0.0) {
        std::cout << "Error: Unable to retrieve room price.\n";
        return;
    }

    // 2) calculate the total price
    const double taxRate = 0.10;
    double subtotal = roomPrice * numberOfRooms;
    double taxes = subtotal * taxRate;
    double totalAmount = subtotal + taxes;

    // 3) Show Booking Summary
    std::cout << "\n===== Booking Summary =====\n"
        << "Destination: " << selectedDestination << "\n"
        << "Hotel: " << selectedHotelName << "\n"
        << "Room Type: " << selectedRoomType << "\n"
        << "Check-in Date: " << checkIn << "\n"
        << "Check-out Date: " << checkOut << "\n"
        << "Number of Rooms: " << numberOfRooms << "\n"
        << "Number of Guests: " << numberOfGuests << "\n"
        << "Subtotal: $" << std::fixed << std::setprecision(2) << subtotal << "\n"
        << "Taxes (10%): $" << taxes << "\n"
        << "Total Amount: $" << totalAmount << "\n\n";

    // 4) Apply Promocode
    std::string applyDiscountStr;
    std::cout << "Would you like to apply a promo code? (yes/no): ";
    std::cin >> applyDiscountStr;
    double discountedAmount = totalAmount;
    if (applyDiscountStr == "yes" || applyDiscountStr == "Yes") {
        std::cout << "Enter Promo Code: ";
        std::string promoCode;
        std::cin >> promoCode;
        bool success = discountManager.applyPromoCode(promoCode, discountedAmount, totalAmount);
        if (success) {
            std::cout << "Discount applied! New Total Amount: $"
                << std::fixed << std::setprecision(2) << discountedAmount << "\n";
            totalAmount = discountedAmount;
        }
        else {
            std::cout << "Invalid Promo Code.\n";
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 5) If MemberUser => Member Discounts
    {
        MemberUser* memberPtr = dynamic_cast<MemberUser*>(user.get());
        if (memberPtr) {
            int lvl = memberPtr->getLoyaltyLevel();
            double loyaltyDiscount = static_cast<double>(lvl * 10); // eg. LV3 => 30RM
            if (loyaltyDiscount > totalAmount) {
                loyaltyDiscount = totalAmount;
            }
            if (loyaltyDiscount > 0.0) {
                totalAmount -= loyaltyDiscount;
                std::cout << "Given your membership level " << lvl
                    << ", you get a RM" << loyaltyDiscount
                    << " discount!\n"
                    << "New total amount: $" << std::fixed << std::setprecision(2)
                    << totalAmount << "\n";
            }
        }
    }

    // 6) Selection of payment method (polymorphism)
    std::cout << "Select payment method:\n"
        << "1. Credit Card\n"
        << "2. E-wallet\n"
        << "3. Cash on Delivery\n"
        << "Please select an option: ";
    int paymentChoice = 0;
    std::cin >> paymentChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::unique_ptr<Payment> paymentMethodPtr;
    switch (paymentChoice) {
    case 1:
        paymentMethodPtr = std::make_unique<CreditCardPayment>();
        break;
    case 2:
        paymentMethodPtr = std::make_unique<EWalletPayment>();
        break;
    case 3:
        paymentMethodPtr = std::make_unique<CashPayment>();
        break;
    default:
        std::cout << "Invalid payment method selected. Defaulting to Credit Card.\n";
        paymentMethodPtr = std::make_unique<CreditCardPayment>();
        break;
    }

    if (!paymentMethodPtr->processPayment()) {
        std::cout << "Payment was not successful. Booking cancelled.\n";
        return;
    }
    std::string paymentDesc = paymentMethodPtr->getPaymentDescription();

    // 7) Final Generation Booking
    std::string bookingID = Booking::generateBookingID();
    Booking newBooking(
        bookingID,
        user->getUsername(),
        selectedDestination,
        selectedHotelName,
        selectedRoomType,
        checkIn,
        checkOut,
        numberOfRooms,
        numberOfGuests,
        paymentDesc,
        totalAmount
    );

    bookingManager.saveBooking(newBooking, "data/bookings.csv");
    bookingManager.addBooking(newBooking);

    std::cout << "\n===== Booking Confirmation =====\n"
        << "Booking ID: " << bookingID << "\n"
        << "Destination: " << selectedDestination << "\n"
        << "Hotel: " << selectedHotelName << "\n"
        << "Room Type: " << selectedRoomType << "\n"
        << "Check-in Date: " << checkIn << "\n"
        << "Check-out Date: " << checkOut << "\n"
        << "Number of Rooms: " << numberOfRooms << "\n"
        << "Number of Guests: " << numberOfGuests << "\n"
        << "Payment Method: " << paymentDesc << "\n"
        << "Total Amount: $" << std::fixed << std::setprecision(2) << totalAmount << "\n"
        << "Your booking has been confirmed. Thank you!\n";
}

void Menu::handleViewBookingHistory(
    BookingManager& bookingManager,
    HotelManager& hotelManager,
    DiscountManager& discountManager,
    std::shared_ptr<User> user
)
{
    auto userBookings = bookingManager.getUserBookings(user->getUsername());
    if (userBookings.empty()) {
        std::cout << "No booking history found.\n";
        return;
    }

    std::cout << "===== Booking History =====\n";
    for (size_t i = 0; i < userBookings.size(); ++i) {
        std::cout << i + 1 << ". Booking ID: " << userBookings[i].getBookingID()
            << ", Hotel: " << userBookings[i].getHotelName()
            << ", Room Type: " << userBookings[i].getRoomType()
            << ", Check-in: " << userBookings[i].getCheckInDate()
            << ", Check-out: " << userBookings[i].getCheckOutDate() << "\n";
    }

    std::cout << "Enter the number of the booking you want to rebook (or 0 to return): ";
    int bookingChoice = 0;
    if (!(std::cin >> bookingChoice)) {
        std::cout << "Invalid input. Returning.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    if (bookingChoice == 0) {
        return;
    }

    if (bookingChoice < 1 || bookingChoice > static_cast<int>(userBookings.size())) {
        std::cout << "Invalid selection.\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Booking selectedBooking = userBookings[bookingChoice - 1];
    std::cout << "You have selected the following booking to rebook:\n";
    selectedBooking.displayBookingSummary();

    // Withdrawal of subscribed information
    std::string oldDestination = selectedBooking.getDestination();
    std::string selectedHotelName = selectedBooking.getHotelName();
    std::string oldRoomType = selectedBooking.getRoomType();

    // Let the user enter a new check-in/check-out
    std::string newCheckIn, newCheckOut;
    if (!getCheckInOutDates(newCheckIn, newCheckOut)) {
        return;
    }

    // Select new room type & number of rooms & guests
    // Because users may want the same roomType, or they may want something else.

    std::string newRoomType;
    int numberOfRooms = 0;
    int numberOfGuests = 0;
    if (!selectRoomAndGuests(
        hotelManager,
        bookingManager,
        oldDestination,  // Destination
        selectedHotelName,
        newCheckIn,
        newCheckOut,
        newRoomType,
        numberOfRooms,
        numberOfGuests
    )) {
        return;
    }

    // Back Process => Price, Offer, Payment
    // 1) Searching for house prices
    double roomPrice = 0.0;
    std::vector<Room> allRooms;
    for (const auto& h : hotelManager.getHotels()) {
        if (h.getDestination() == oldDestination && h.getHotelName() == selectedHotelName) {
            allRooms = h.getRooms();
            break;
        }
    }
    for (auto& rm : allRooms) {
        if (rm.getRoomTypeString() == newRoomType) {
            roomPrice = rm.getPrice();
            break;
        }
    }
    if (roomPrice <= 0.0) {
        std::cout << "Error retrieving room price.\n";
        return;
    }

    // 2) calculate the price
    const double taxRate = 0.10;
    double subtotal = roomPrice * numberOfRooms;
    double taxes = subtotal * taxRate;
    double totalAmount = subtotal + taxes;

    std::cout << "\n===== Rebooking Summary =====\n"
        << "Old Destination: " << oldDestination << "\n"
        << "Hotel: " << selectedHotelName << "\n"
        << "Old Room Type: " << oldRoomType << "\n\n"
        << "New Check-in: " << newCheckIn << "\n"
        << "New Check-out: " << newCheckOut << "\n"
        << "New Room Type: " << newRoomType << "\n"
        << "Rooms: " << numberOfRooms << "\n"
        << "Guests: " << numberOfGuests << "\n"
        << "Subtotal: $" << std::fixed << std::setprecision(2) << subtotal << "\n"
        << "Taxes (10%): $" << taxes << "\n"
        << "Total: $" << totalAmount << "\n\n";

    // 3) PromoCode
    std::string applyDiscountStr;
    std::cout << "Would you like to apply a promo code? (yes/no): ";
    std::cin >> applyDiscountStr;
    double discountedAmount = totalAmount;
    if (applyDiscountStr == "yes" || applyDiscountStr == "Yes") {
        std::cout << "Enter Promo Code: ";
        std::string promoCode;
        std::cin >> promoCode;
        bool success = discountManager.applyPromoCode(promoCode, discountedAmount, totalAmount);
        if (success) {
            std::cout << "Promo applied! New total: $"
                << std::fixed << std::setprecision(2) << discountedAmount << "\n";
            totalAmount = discountedAmount;
        }
        else {
            std::cout << "Invalid Promo Code.\n";
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 4) MemberUser discount
    {
        MemberUser* memberPtr = dynamic_cast<MemberUser*>(user.get());
        if (memberPtr) {
            int lvl = memberPtr->getLoyaltyLevel();
            double loyaltyDiscount = static_cast<double>(lvl * 10);
            if (loyaltyDiscount > totalAmount) {
                loyaltyDiscount = totalAmount;
            }
            if (loyaltyDiscount > 0.0) {
                totalAmount -= loyaltyDiscount;
                std::cout << "Given your membership level " << lvl
                    << ", you get a RM" << loyaltyDiscount
                    << " discount!\n"
                    << "New total amount: $" << std::fixed << std::setprecision(2)
                    << totalAmount << "\n";
            }
        }
    }

    // 5) Selection of payment method (polymorphism)
    std::cout << "Select payment method:\n"
        << "1. Credit Card\n"
        << "2. E-wallet\n"
        << "3. Cash on Delivery\n"
        << "Please select an option: ";
    int paymentChoice = 0;
    std::cin >> paymentChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::unique_ptr<Payment> paymentMethodPtr;
    switch (paymentChoice) {
    case 1:
        paymentMethodPtr = std::make_unique<CreditCardPayment>();
        break;
    case 2:
        paymentMethodPtr = std::make_unique<EWalletPayment>();
        break;
    case 3:
        paymentMethodPtr = std::make_unique<CashPayment>();
        break;
    default:
        std::cout << "Invalid payment method selected. Defaulting to Credit Card.\n";
        paymentMethodPtr = std::make_unique<CreditCardPayment>();
        break;
    }
    if (!paymentMethodPtr->processPayment()) {
        std::cout << "Payment not successful. Rebooking cancelled.\n";
        return;
    }
    std::string paymentDesc = paymentMethodPtr->getPaymentDescription();

    // 6) Generate new booking
    std::string newBookingID = Booking::generateBookingID();
    Booking newBooking(
        newBookingID,
        user->getUsername(),
        oldDestination,
        selectedHotelName,
        newRoomType,
        newCheckIn,
        newCheckOut,
        numberOfRooms,
        numberOfGuests,
        paymentDesc,
        totalAmount
    );

    bookingManager.saveBooking(newBooking, "data/bookings.csv");
    bookingManager.addBooking(newBooking);

    std::cout << "\n===== Rebooking Confirmation =====\n"
        << "Booking ID: " << newBookingID << "\n"
        << "Destination: " << oldDestination << "\n"
        << "Hotel: " << selectedHotelName << "\n"
        << "Room Type: " << newRoomType << "\n"
        << "Check-in: " << newCheckIn << "\n"
        << "Check-out: " << newCheckOut << "\n"
        << "Rooms: " << numberOfRooms << "\n"
        << "Guests: " << numberOfGuests << "\n"
        << "Payment: " << paymentDesc << "\n"
        << "Total Amount: $" << std::fixed << std::setprecision(2) << totalAmount << "\n"
        << "Your rebooking has been confirmed. Thank you!\n";
}


void Menu::handleAddReview(const BookingManager& bookingManager, ReviewManager& reviewManager, std::shared_ptr<User> user) {
    auto userBookings = bookingManager.getUserBookings(user->getUsername());
    if (userBookings.empty()) {
        std::cout << "No bookings found. You need to make a booking before adding a review.\n";
        return;
    }

    std::cout << "===== Your Bookings =====\n";
    for (size_t i = 0; i < userBookings.size(); ++i) {
        std::cout << i + 1 << ". Booking ID: " << userBookings[i].getBookingID()
            << ", Hotel: " << userBookings[i].getHotelName()
            << ", Check-in: " << userBookings[i].getCheckInDate()
            << ", Check-out: " << userBookings[i].getCheckOutDate() << "\n";
    }

    std::cout << "Select a booking to review (enter number, or 0 to cancel): ";
    int bookingChoice = 0;
    if (!(std::cin >> bookingChoice)) {
        std::cout << "Invalid input. Returning to main menu.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    if (bookingChoice == 0) {
        return;
    }

    if (bookingChoice < 1 || bookingChoice > static_cast<int>(userBookings.size())) {
        std::cout << "Invalid selection. Returning to main menu.\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Booking selectedBooking = userBookings[bookingChoice - 1];
    std::cout << "You have selected the following booking to review:\n";
    selectedBooking.displayBookingSummary();

    // Extract hotelName and roomType
    std::string bookingID = selectedBooking.getBookingID();
    std::string hotelName = selectedBooking.getHotelName();

    // Select comment type
    std::string reviewType;
    std::cout << "Enter review type (Text/Star) or 'q' to cancel: ";
    std::cin >> reviewType;
    if (reviewType == "q" || reviewType == "Q") {
        std::cout << "Review addition cancelled.\n";
        return;
    }

    if (reviewType == "Star") {
        int rating;
        while (true) {
            std::cout << "Enter rating (1-5) or 0 to cancel: ";
            if (!(std::cin >> rating)) {
                std::cout << "Invalid input. Please enter a number between 1 and 5.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            if (rating == 0) {
                std::cout << "Review addition cancelled.\n";
                return;
            }
            if (rating < 1 || rating > 5) {
                std::cout << "Invalid rating. Must be between 1 and 5.\n";
                continue;
            }
            break;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string reviewID = "RV" + Booking::generateBookingID();
        auto starReview = std::make_shared<StarReview>(reviewID, bookingID, user->getUsername(), hotelName, rating);
        reviewManager.saveReview(starReview, "data/reviews.csv");
        std::cout << "Star review added successfully.\n";
    }
    else if (reviewType == "Text") {
        std::cin.ignore();
        std::string content;
        std::cout << "Enter your review or 'q' to cancel: ";
        std::getline(std::cin, content);
        if (content == "q" || content == "Q") {
            std::cout << "Review addition cancelled.\n";
            return;
        }
        std::string reviewID = "RV" + Booking::generateBookingID();
        auto textReview = std::make_shared<TextReview>(reviewID, bookingID, user->getUsername(), hotelName, content);
        reviewManager.saveReview(textReview, "data/reviews.csv");
        std::cout << "Text review added successfully.\n";
    }
    else {
        std::cout << "Invalid review type.\n";
    }
}

void Menu::handleApplyDiscount(DiscountManager& discountManager, double& totalAmount) {
    std::string promoCode;
    std::cout << "Enter Promo Code: ";
    std::cin >> promoCode;
    double discountedPrice;
    if (discountManager.applyPromoCode(promoCode, discountedPrice, totalAmount)) {
        std::cout << "Discount applied! New Total Amount: $" << std::fixed << std::setprecision(2) << discountedPrice << "\n";
        totalAmount = discountedPrice;
    }
    else {
        std::cout << "Invalid Promo Code.\n";
    }
}

int Menu::selectFromList(const std::vector<std::string>& options, const std::string& prompt) {
    if (options.empty()) {
        std::cout << "There are no available options.\n";
        return -1;
    }

    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ". " << options[i] << "\n";
    }

    int choice = 0;
    while (true) {
        std::cout << prompt;
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input, please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (choice < 1 || choice > static_cast<int>(options.size())) {
            std::cout << "Out of range, please select again.\n";
            continue;
        }
        break;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice - 1;
}


bool Menu::selectDestinationAndHotel(
    const HotelManager& hotelManager,
    std::string& selectedDestination,
    std::string& selectedHotelName
) {
    // 1)User selects destination
    std::vector<std::string> uniqueDestinations;
    for (const auto& h : hotelManager.getHotels()) {
        if (std::find(uniqueDestinations.begin(), uniqueDestinations.end(),
            h.getDestination()) == uniqueDestinations.end()) {
            uniqueDestinations.push_back(h.getDestination());
        }
    }
    if (uniqueDestinations.empty()) {
        std::cout << "No destinations available.\n";
        return false; // Returns directly to indicate that it can't be selected
    }

    std::cout << "Select a destination:\n";
    int destIdx = selectFromList(uniqueDestinations, "Enter number for your destination: ");
    if (destIdx == -1) {
        return false; // User cancellation
    }
    selectedDestination = uniqueDestinations[destIdx];

    // 2) User selects hotel
    std::vector<std::string> hotelsInDest;
    for (const auto& h : hotelManager.getHotels()) {
        if (h.getDestination() == selectedDestination) {
            hotelsInDest.push_back(h.getHotelName());
        }
    }
    if (hotelsInDest.empty()) {
        std::cout << "No hotels found in " << selectedDestination << ".\n";
        return false;
    }
    std::cout << "Select a hotel:\n";
    int hotelIdx = selectFromList(hotelsInDest, "Enter number for your hotel: ");
    if (hotelIdx == -1) {
        return false;
    }
    selectedHotelName = hotelsInDest[hotelIdx];

    return true; // all are successful
}


bool Menu::getCheckInOutDates(std::string& checkIn, std::string& checkOut)
{
    std::string today = getCurrentDate();

    while (true) {
        std::cout << "Enter check-in date (YYYY-MM-DD) or 'q' to cancel: ";
        std::cin >> checkIn;
        if (checkIn == "q" || checkIn == "Q") {
            return false;
        }
        if (!validateDate(checkIn)) {
            std::cout << "Invalid date format.\n";
            continue;
        }
        if (!isDateNotInPast(checkIn, today)) {
            std::cout << "Check-in cannot be in the past.\n";
            continue;
        }
        break;
    }

    while (true) {
        std::cout << "Enter check-out date (YYYY-MM-DD) or 'q' to cancel: ";
        std::cin >> checkOut;
        if (checkOut == "q" || checkOut == "Q") {
            return false;
        }
        if (!validateDate(checkOut)) {
            std::cout << "Invalid date format.\n";
            continue;
        }
        if (!isCheckInBeforeCheckOut(checkIn, checkOut)) {
            std::cout << "Check-out must be after check-in.\n";
            continue;
        }
        break;
    }

    return true;
}


bool Menu::selectRoomAndGuests(
    const HotelManager& hotelManager,
    const BookingManager& bookingManager,
    const std::string& destination,
    const std::string& hotelName,
    std::string& checkIn,
    std::string& checkOut,
    std::string& selectedRoomType,
    int& numberOfRooms,
    int& numberOfGuests
) {
    // Find all room types for this hotel
    std::vector<Room> allRooms;
    for (const auto& h : hotelManager.getHotels()) {
        if (h.getDestination() == destination && h.getHotelName() == hotelName) {
            allRooms = h.getRooms();
            break;
        }
    }
    if (allRooms.empty()) {
        std::cout << "No rooms found for " << hotelName << ".\n";
        return false;
    }

    // Calculate and display the number of available rooms
    std::vector<std::string> displayList;
    std::vector<std::string> rawRoomTypes;
    std::set<std::string> usedTypes;

    for (const auto& rm : allRooms) {
        std::string rType = rm.getRoomTypeString();
        if (usedTypes.find(rType) != usedTypes.end()) {
            continue;
        }
        usedTypes.insert(rType);

        int total = hotelManager.getTotalRooms(hotelName, rType);
        int booked = bookingManager.getBookedRooms(hotelName, rType, checkIn, checkOut);
        int available = total - booked;

        std::ostringstream oss;
        oss << rType << " (" << available << " available)";
        displayList.push_back(oss.str());
        rawRoomTypes.push_back(rType);
    }
    if (displayList.empty()) {
        std::cout << "No room types available in that date range.\n";
        return false;
    }

    // Let the user choose the room type
    std::cout << "Available room types:\n";
    for (size_t i = 0; i < displayList.size(); ++i) {
        std::cout << (i + 1) << ". " << displayList[i] << "\n";
    }
    int rtypeIdx;
    while (true) {
        std::cout << "Choose a room type (or 0 to cancel): ";
        if (!(std::cin >> rtypeIdx)) {
            std::cout << "Invalid input.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (rtypeIdx == 0) {
            return false;
        }
        if (rtypeIdx < 1 || rtypeIdx >(int)displayList.size()) {
            std::cout << "Out of range. Try again.\n";
            continue;
        }
        break;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    selectedRoomType = rawRoomTypes[rtypeIdx - 1];

    // Enter the number of rooms
    while (true) {
        std::cout << "Number of rooms (or 0 to cancel): ";
        if (!(std::cin >> numberOfRooms)) {
            std::cout << "Invalid input.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (numberOfRooms == 0) {
            return false;
        }
        if (numberOfRooms < 0) {
            std::cout << "Cannot be negative.\n";
            continue;
        }
        break;
    }
    // Enter the number of guests
    while (true) {
        std::cout << "Number of guests (or 0 to cancel): ";
        if (!(std::cin >> numberOfGuests)) {
            std::cout << "Invalid input.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (numberOfGuests == 0) {
            return false;
        }
        if (numberOfGuests < 0) {
            std::cout << "Cannot be negative.\n";
            continue;
        }
        break;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Double-check if it's enough
    {
        int total = hotelManager.getTotalRooms(hotelName, selectedRoomType);
        int booked = bookingManager.getBookedRooms(hotelName, selectedRoomType, checkIn, checkOut);
        int available = total - booked;
        if (available < numberOfRooms) {
            std::cout << "Only " << available << " rooms are left for that type.\n";
            return false;
        }

        // See if the number of people exceeds maxOccupancy.
        int maxOcc = 0;
        for (auto& rm : allRooms) {
            if (rm.getRoomTypeString() == selectedRoomType) {
                maxOcc = rm.getMaxOccupancy();
                break;
            }
        }
        if (numberOfGuests > (numberOfRooms * maxOcc)) {
            std::cout << "The number of guests exceeds the max occupancy.\n";
            return false;
        }
    }

    return true; // It's all good.
}
