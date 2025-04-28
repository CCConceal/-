// MemberUser.cpp
#include "MemberUser.h"
#include <iostream>

MemberUser::MemberUser(int id, const std::string& uname, const std::string& pwd, int a, int level)
    : User(id, uname, pwd, a), loyaltyLevel(level) {}

void MemberUser::displayWelcomeMessage() const {
    std::cout << "Welcome, " << username << "! Thank you for being a loyal member of our hotel booking system.\n";
}

int MemberUser::getLoyaltyLevel() const {
    return loyaltyLevel;
}
