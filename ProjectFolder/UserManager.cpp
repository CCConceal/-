// UserManager.cpp
#include "UserManager.h"
#include "MemberUser.h"
#include "CommonUser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <limits>

void UserManager::loadUsers(const std::string& filename) {
    std::cerr << "[UserManager::loadUsers] Starting to load users from: " << filename << "\n";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[UserManager::loadUsers] Unable to open users file: " << filename << "\n";
        throw std::runtime_error("Failed to open users file.");
    }

    std::string line;
    int lineNumber = 0;

    // Read and skip table header rows
    if (std::getline(file, line)) {
        lineNumber++;
        std::cerr << "[UserManager::loadUsers] Skipping header line: " << line << "\n";

        /*reference:utf8-bom-strip. (n.d.). GitHub Repository. Retrieved December 29, 2024, from https://github.com/zer4tul/utf8-bom-strip
        */
        if (line.size() >= 3 &&
            static_cast<unsigned char>(line[0]) == 0xEF &&
            static_cast<unsigned char>(line[1]) == 0xBB &&
            static_cast<unsigned char>(line[2]) == 0xBF) {
            std::cerr << "[UserManager::loadUsers] Detected and removing BOM.\n";
            line.erase(0, 3);
        }
    }

    while (std::getline(file, line)) {
        lineNumber++;
        std::cerr << "[UserManager::loadUsers] Processing line " << lineNumber << ": " << line << "\n";
        try {
            std::istringstream iss(line);
            std::string userIDStr, username, password, ageStr, userType, loyaltyLevelStr;

            
            std::getline(iss, userIDStr, ',');
            std::getline(iss, username, ',');
            std::getline(iss, password, ',');
            std::getline(iss, ageStr, ',');
            std::getline(iss, userType, ',');
            std::getline(iss, loyaltyLevelStr, ',');


            std::cerr << "DEBUG: Converting userIDStr = '" << userIDStr
                << "', ageStr = '" << ageStr
                << "', userType = '" << userType
                << "', loyaltyLevelStr = '" << loyaltyLevelStr << "'\n";

            // Convert a string to an integer
            int userID = std::stoi(userIDStr);
            int age = std::stoi(ageStr);

            // Select a derived class based on userType
            if (userType == "Member") {

                int loyaltyLevel = 0;
                if (!loyaltyLevelStr.empty()) {
                    loyaltyLevel = std::stoi(loyaltyLevelStr);
                }

                std::shared_ptr<User> user = std::make_shared<MemberUser>(userID, username, password, age, loyaltyLevel);
                users.emplace_back(user);
                std::cerr << "[UserManager::loadUsers] Added MemberUser: " << username << "\n";
            }
            else if (userType == "Common") {

                std::shared_ptr<User> user = std::make_shared<CommonUser>(userID, username, password, age);
                users.emplace_back(user);
                std::cerr << "[UserManager::loadUsers] Added CommonUser: " << username << "\n";
            }
            else {
                std::cerr << "[UserManager::loadUsers] Unknown user type '" << userType
                    << "' in line " << lineNumber << ". Skipping.\n";
                continue;
            }
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "[UserManager::loadUsers] Invalid numeric argument in line " << lineNumber << ":\n"
                << "  " << line << "\n"
                << "  Reason: " << e.what() << "\n";
            // Skip this line and continue with the next line
            continue;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "[UserManager::loadUsers] Out of range error in line " << lineNumber << ":\n"
                << "  " << line << "\n"
                << "  Reason: " << e.what() << "\n";
            continue;
        }
        catch (...) {
            std::cerr << "[UserManager::loadUsers] Unknown exception while parsing line " << lineNumber << ":\n"
                << "  " << line << "\n";
            continue;
        }
    }

    file.close();
    std::cerr << "[UserManager::loadUsers] Finished loading users.\n";
}

bool UserManager::authenticateUser(const std::string& username, const std::string& password, std::shared_ptr<User>& currentUser) {
    for (const auto& user : users) {
        if (user->getUsername() == username && user->authenticate(password)) {
            currentUser = user;
            return true;
        }
    }
    return false;
}



bool UserManager::registerUser(const std::string& username, const std::string& password)
{
    // 1) Check if the username already exists
    for (const auto& user : users) {
        if (user->getUsername() == username) {
            std::cerr << "Registration failed. The username '" << username
                << "' is already taken.\n";
            return false;
        }
    }

    // 2) Generate a new userID (simple approach: find max existing userID + 1)
    int newUserID = 1;
    for (const auto& user : users) {
        newUserID = std::max(newUserID, user->getUserID() + 1);
    }

    // 3) Ask the user type (must be either "Common" or "Member")
    std::string userType;
    std::cout << "Enter user type (Common/Member): ";
    std::cin >> userType;
    if (userType != "Common" && userType != "Member") {
        std::cerr << "Registration failed. Invalid user type: '" << userType
            << "'. Please enter 'Common' or 'Member'.\n";
        return false;
    }

    // 4) Ask for user age, with validation
    int age;
    std::cout << "Enter age for new user: ";
    while (!(std::cin >> age) || age <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Invalid age. Please enter a positive integer for age: ";
    }
    // Clear any leftover input
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // 5) If it's a Member user, ask for loyalty level with validation
    std::shared_ptr<User> newUser;
    if (userType == "Member") {
        int loyaltyLevel;
        std::cout << "Enter loyalty level for this member (e.g. 3)(this step will be done by a staff): ";
        while (!(std::cin >> loyaltyLevel) || loyaltyLevel < 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid loyalty level. Please enter a non-negative integer: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        newUser = std::make_shared<MemberUser>(newUserID, username, password, age, loyaltyLevel);
    }
    else {
        // For a Common user, we just construct directly
        newUser = std::make_shared<CommonUser>(newUserID, username, password, age);
    }

    // 6) Insert the new user into our in-memory vector
    users.emplace_back(newUser);

    // 7) Append the new user to data/users.csv
    std::ofstream file("data/users.csv", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "[UserManager::registerUser] Unable to open users file for appending: data/users.csv\n";
        return false;
    }

    file << newUserID << "," << username << "," << password << ","
        << age << "," << userType;
    if (userType == "Member") {
        // Retrieve loyaltyLevel from the newly created user
        int loyaltyLevel = std::dynamic_pointer_cast<MemberUser>(newUser)->getLoyaltyLevel();
        file << "," << loyaltyLevel;
    }
    else {
        // For Common users, leave the loyaltyLevel column empty
        file << ",";
    }
    file << "\n";

    file.close();

    std::cout << "Registration successful. You can now log in with username: " << username << "\n";
    return true;
}
