// UserManager.h
#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <vector>
#include <string>
#include <memory>
#include "User.h"

class UserManager {
private:
    std::vector<std::shared_ptr<User>> users;
public:
    void loadUsers(const std::string& filename);
    bool authenticateUser(const std::string& username, const std::string& password, std::shared_ptr<User>& currentUser);
    bool registerUser(const std::string& username, const std::string& password);
};

#endif // USERMANAGER_H
