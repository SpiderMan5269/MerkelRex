#pragma once

#include "User.h"
#include <vector>
#include <string>

// Handles user lifecycle (register/login) and CSV persistence
class UserManager
{
public:
    UserManager(const std::string& csvFilename = "users.csv");
    
    // Registration
    User registerUser(const std::string& fullName, const std::string& email, 
                     const std::string& password);
    
    // Login (by ID or email)
    User* loginUser(const std::string& emailOrId, const std::string& password);
    
    // Lookups
    User* findUserByEmail(const std::string& email);
    User* findUserById(const std::string& id);
    bool userExists(const std::string& email, const std::string& fullName);
    
    // CSV operations
    void saveUserToCSV(const User& user);
    void loadUsersFromCSV();

private:
    std::vector<User> users;
    std::string csvFilename;
};
