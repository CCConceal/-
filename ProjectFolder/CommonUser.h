// CommonUser.h
#ifndef COMMONUSER_H
#define COMMONUSER_H

#include "User.h"

class CommonUser : public User {
public:
    CommonUser(int id, const std::string& uname, const std::string& pwd, int a);

    void displayWelcomeMessage() const override;
};

#endif // COMMONUSER_H
