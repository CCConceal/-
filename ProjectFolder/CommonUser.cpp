// CommonUser.cpp
#include "CommonUser.h"
#include <iostream>

CommonUser::CommonUser(int id, const std::string& uname, const std::string& pwd, int a)
    : User(id, uname, pwd, a) {}

void CommonUser::displayWelcomeMessage() const {
    std::cout << "Welcome, " << username << "! Thank you for using our hotel booking system.\n";
}
