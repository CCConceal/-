// main.cpp
#include <iostream>
#include <memory>
#include <string>
#include <filesystem>  
#include "UserManager.h"
#include "HotelManager.h"
#include "BookingManager.h"
#include "ReviewManager.h"
#include "DiscountManager.h"
#include "Menu.h"

int main() {
    // 1. Check and create data directory
    if (!std::filesystem::exists("data")) {
        if (!std::filesystem::create_directory("data")) {
            std::cerr << "[MAIN] Failed to create 'data' directory.\n";
            return -1;
        }
    }

    // 2. Initialize UserManager
    std::shared_ptr<UserManager> userManager = std::make_shared<UserManager>();
    try {
        userManager->loadUsers("data/users.csv");
        std::cout << "[MAIN] Users loaded successfully.\n";
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "[MAIN] Caught std::invalid_argument while loading users: " << e.what() << std::endl;
        return -1;
    }
    catch (const std::exception& e) {
        std::cerr << "[MAIN] Caught std::exception while loading users: " << e.what() << std::endl;
        return -1;
    }

    // 3. Initialize HotelManager
    std::shared_ptr<HotelManager> hotelManager = std::make_shared<HotelManager>();
    try {
        hotelManager->loadHotels("data/hotels.csv");
        std::cout << "[MAIN] Hotels loaded successfully.\n";
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "[MAIN] Caught std::invalid_argument while loading hotels: " << e.what() << std::endl;
        return -1;
    }
    catch (const std::exception& e) {
        std::cerr << "[MAIN] Caught std::exception while loading hotels: " << e.what() << std::endl;
        return -1;
    }

    // 4. Initialize BookingManager
    std::shared_ptr<BookingManager> bookingManager = std::make_shared<BookingManager>(hotelManager);
 
    try {
        bookingManager->loadBookings("data/bookings.csv");
        std::cout << "[MAIN] Bookings loaded successfully.\n";
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "[MAIN] Caught std::invalid_argument while loading bookings: " << e.what() << std::endl;
        return -1;
    }
    catch (const std::exception& e) {
        std::cerr << "[MAIN] Caught std::exception while loading bookings: " << e.what() << std::endl;
        return -1;
    }

    // 5. Initialize ReviewManager
    ReviewManager reviewManager;
    try {
        reviewManager.loadReviews("data/reviews.csv");
        std::cout << "[MAIN] Reviews loaded successfully.\n";
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "[MAIN] Caught std::invalid_argument while loading reviews: " << e.what() << std::endl;
        return -1;
    }
    catch (const std::exception& e) {
        std::cerr << "[MAIN] Caught std::exception while loading reviews: " << e.what() << std::endl;
        return -1;
    }

    // 6. Initialize DiscountManager
    DiscountManager discountManager;
    try {
        discountManager.loadDiscounts("data/discounts.csv");
        std::cout << "[MAIN] Discounts loaded successfully.\n";
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "[MAIN] Caught std::invalid_argument while loading discounts: " << e.what() << std::endl;
        return -1;
    }
    catch (const std::exception& e) {
        std::cerr << "[MAIN] Caught std::exception while loading discounts: " << e.what() << std::endl;
        return -1;
    }

    // ========== User Login/Registration Logic ==========
    std::shared_ptr<User> currentUser = nullptr;
    std::string username, password;
    int initialChoice;
    std::cout << std::endl;
    std::cout << "\n\n\n\n\n";
    std::cout << "============================================\n";
    std::cout << "============================================\n";
    std::cout << "===== Welcome to the Hotel Booking System =====\n";

    while (true) {
        std::cout << "1. Login\n2. Register\n3. Exit\nPlease select an option: ";
        if (!(std::cin >> initialChoice)) {
            // Invalid input, clear buffer and prompt
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (initialChoice == 1) {
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            try {
                if (userManager->authenticateUser(username, password, currentUser)) {
                    currentUser->displayWelcomeMessage();
                    break;  // Enter main menu
                }
                else {
                    std::cout << "Authentication failed. Invalid username or password.\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "[MAIN] Caught exception during authentication: " << e.what() << std::endl;
            }
        }
        else if (initialChoice == 2) {
            std::cout << "Enter new username: ";
            std::cin >> username;
            std::cout << "Enter new password: ";
            std::cin >> password;
            try {
                if (userManager->registerUser(username, password)) {
                    std::cout << "Registration successful. You can now login.\n";
                }
                else {
                    std::cout << "Registration failed. Please check the error message above.\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "[MAIN] Caught exception during registration: " << e.what() << std::endl;
            }
        }
        else if (initialChoice == 3) {
            std::cout << "Exiting the system. Goodbye!\n";
            return 0;
        }
        else {
            std::cout << "Invalid option. Please try again.\n";
        }
    }

    // ========== Main Menu Loop ==========
    bool running = true;
    while (running) {
        Menu::displayMainMenu();
        int choice;
        if (!(std::cin >> choice)) {
            // Invalid input, clear buffer and prompt
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        try {
            switch (choice) {
            case 1:
                // Call the updated handleDisplayRooms function
                Menu::handleDisplayRooms(*hotelManager);
                break;
            case 2:
                Menu::handleBookRoom(*hotelManager, *bookingManager, discountManager, currentUser);
                break;
            case 3:
                Menu::handleViewBookingHistory(*bookingManager, *hotelManager, discountManager, currentUser);
                break;
            case 4:
                Menu::handleAddReview(*bookingManager, reviewManager, currentUser);
                break;
            case 5:
                running = false;
                std::cout << "Thank you for using the system. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
            }
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "[MAIN] Caught std::invalid_argument during menu operation: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "[MAIN] Caught std::exception during menu operation: " << e.what() << std::endl;
        }
    }

    return 0;
}
