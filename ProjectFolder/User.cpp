// User.cpp
#include "User.h"
#include <iostream>

User::User(int id, const std::string& uname, const std::string& pwd, int a)
    : userID(id), username(uname), password(pwd), age(a) {}

int User::getUserID() const {
    return userID;
}

std::string User::getUsername() const {
    return username;
}

bool User::authenticate(const std::string& pwd) const {
    return password == pwd;
}
