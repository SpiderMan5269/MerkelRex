#pragma once

#include <string>
#include <vector>

// Represents an authenticated profile with hashed credentials
class User
{
public:
    User();
    User(const std::string& id, const std::string& name, const std::string& email, 
         const std::string& passwordHash);

    // Accessors
    std::string getId() const { return userId; }
    std::string getName() const { return fullName; }
    std::string getEmail() const { return email; }
    std::string getPasswordHash() const { return passwordHash; }

    // Helpers for identity and credential handling
    static std::string generateUserId();
    static std::string hashPassword(const std::string& password);
    static bool verifyPassword(const std::string& plainPassword, const std::string& storedHash);

private:
    std::string userId;        // 10-digit unique ID
    std::string fullName;
    std::string email;
    std::string passwordHash;  // Hashed password
};
