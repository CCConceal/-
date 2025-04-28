// MemberUser.h
#ifndef MEMBERUSER_H
#define MEMBERUSER_H

#include "User.h"

class MemberUser : public User {
private:
    int loyaltyLevel;
public:
    MemberUser(int id, const std::string& uname, const std::string& pwd, int a, int level);

    void displayWelcomeMessage() const override;
    int getLoyaltyLevel() const;
};

#endif // MEMBERUSER_H
