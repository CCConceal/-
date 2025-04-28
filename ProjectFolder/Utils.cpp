#define _CRT_SECURE_NO_WARNINGS  // disable MSVC secure warnings

/*reference£ºDuke56. (2016, September 1). Solution to the "_CRT_SECURE_NO_WARNINGS" error message. CSDN. Retrieved December 29, 2024, from https://blog.csdn.net/duke56/article/details/52403458
*/

// Utils.cpp
#include "Utils.h"
#include <sstream>
#include <regex>
#include <iomanip>
#include <ctime>
#include<iostream>

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(s);
    while (std::getline(ss, token, delimiter)) {
        tokens.emplace_back(token);
    }
    return tokens;
}

bool isValidDate(const std::string& date) {
    std::regex datePattern(R"(\d{4}-\d{2}-\d{2})");
    if (!std::regex_match(date, datePattern)) {
        return false;
    }
    // Further validation to check if the date is valid
    std::tm tm1 = {};
    std::istringstream ss1(date);
    ss1 >> std::get_time(&tm1, "%Y-%m-%d");
    if (ss1.fail()) {
        return false;
    }
    /*reference£º[5]	cppreference.com. (n.d.). std::ctime. Retrieved December 29, 2024, from https://en.cppreference.com/w/cpp/header/ctime
    [4]	cppreference.com. (n.d.). Regular expressions library (since C++11). Retrieved December 29, 2024, from https://en.cppreference.com/w/cpp/regex
    */


    // Check if the date is valid (e.g., month between 1-12, day appropriate for month)
    // This can be complex; for simplicity, mktime can be used to normalize the date
    tm1.tm_hour = 12; // Avoid issues with daylight saving time
    time_t time = mktime(&tm1);
    if (time == -1) {
        return false;
    }
    std::tm* normalized = localtime(&time);
    if (normalized->tm_year != tm1.tm_year || normalized->tm_mon != tm1.tm_mon || normalized->tm_mday != tm1.tm_mday) {
        return false;
    }
    return true;
}

bool validateDate(const std::string& date) {
    if (!isValidDate(date)) {
        return false;
    }

    std::tm tmDate = {};
    std::istringstream ssDate(date);
    ssDate >> std::get_time(&tmDate, "%Y-%m-%d");

    if (ssDate.fail()) {
        return false;
    }

    return true;
}



bool isCheckInBeforeCheckOut(const std::string& checkIn, const std::string& checkOut) {
    std::tm tmCheckIn = {};
    std::tm tmCheckOut = {};
    std::istringstream ssCheckIn(checkIn);
    std::istringstream ssCheckOut(checkOut);
    ssCheckIn >> std::get_time(&tmCheckIn, "%Y-%m-%d");
    ssCheckOut >> std::get_time(&tmCheckOut, "%Y-%m-%d");
    if (ssCheckIn.fail() || ssCheckOut.fail()) {
        return false;
    }
    time_t timeCheckIn = mktime(&tmCheckIn);
    time_t timeCheckOut = mktime(&tmCheckOut);
    return difftime(timeCheckOut, timeCheckIn) > 0;
}

std::string getCurrentDate() {
    std::time_t t = std::time(nullptr);
    std::tm tmCurrent = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tmCurrent, "%Y-%m-%d");
    return ss.str();
}

bool isDateNotInPast(const std::string& dateToCheck, const std::string& referenceDate) {
    std::tm tmCheck = {};
    std::tm tmReference = {};

    std::istringstream ssCheck(dateToCheck);
    ssCheck >> std::get_time(&tmCheck, "%Y-%m-%d");
    if (ssCheck.fail()) {
        return false; // Unable to resolve date
    }

    std::istringstream ssReference(referenceDate);
    ssReference >> std::get_time(&tmReference, "%Y-%m-%d");
    if (ssReference.fail()) {
        return false; // Unable to resolve reference date
    }

    
    time_t timeCheck = mktime(&tmCheck);
    time_t timeReference = mktime(&tmReference);

    if (timeCheck == -1 || timeReference == -1) {
        return false; // conversion failure
    }

    // Returns true if timeCheck is greater than or equal to timeReference.
    return difftime(timeCheck, timeReference) >= 0;
}

/*reference£º[6]	GitHub. (n.d.). an-halim/Hotel-BookingAPP: Simple app to manage hotel room booking system Features. Retrieved December 29, 2024, from https://github.com/an-halim/Hotel-BookingAPP
*/