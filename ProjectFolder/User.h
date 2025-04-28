// User.h
#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    int userID;
    std::string username;
    std::string password;
    int age;
public:
    User(int id, const std::string& uname, const std::string& pwd, int a);
    virtual ~User() {}

    int getUserID() const;
    std::string getUsername() const;
    bool authenticate(const std::string& pwd) const;

    virtual void displayWelcomeMessage() const = 0;
};

#endif // USER_H
