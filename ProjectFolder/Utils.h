// Utils.h
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& s, char delimiter);
bool isValidDate(const std::string& date);
bool isCheckInBeforeCheckOut(const std::string& checkIn, const std::string& checkOut);
std::string getCurrentDate();
bool validateDate(const std::string& date);
bool isDateNotInPast(const std::string& dateToCheck, const std::string& referenceDate);



#endif // UTILS_H
