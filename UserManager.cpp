#include "UserManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

UserManager::UserManager(const std::string& csvFilename)
    : csvFilename(csvFilename)
{
    loadUsersFromCSV();
}

// Register a new user, persist to CSV, and return the created object
User UserManager::registerUser(const std::string& fullName, const std::string& email, 
                               const std::string& password)
{
    // Check for duplicates
    if (userExists(email, fullName))
    {
        throw std::runtime_error("User with this email or name already exists!");
    }

    // Generate unique ID and hash password
    std::string userId = User::generateUserId();
    std::string passwordHash = User::hashPassword(password);

    // Create user
    User newUser(userId, fullName, email, passwordHash);
    
    // Save to file and memory
    users.push_back(newUser);
    saveUserToCSV(newUser);

    std::cout << "\n=== Registration Successful ===" << std::endl;
    std::cout << "Your User ID: " << userId << std::endl;
    std::cout << "Please save this ID for future login." << std::endl;

    return newUser;
}

// Authenticate by ID or email; return pointer to stored User on success
User* UserManager::loginUser(const std::string& emailOrId, const std::string& password)
{
    User* user = nullptr;

    // Try to find user by ID first, then by email
    user = findUserById(emailOrId);
    if (!user)
    {
        user = findUserByEmail(emailOrId);
    }

    if (!user)
    {
        throw std::runtime_error("User not found!");
    }

    // Verify password
    if (!User::verifyPassword(password, user->getPasswordHash()))
    {
        throw std::runtime_error("Invalid password!");
    }

    return user;
}

User* UserManager::findUserByEmail(const std::string& email)
{
    for (auto& user : users)
    {
        if (user.getEmail() == email)
        {
            return &user;
        }
    }
    return nullptr;
}

User* UserManager::findUserById(const std::string& id)
{
    for (auto& user : users)
    {
        if (user.getId() == id)
        {
            return &user;
        }
    }
    return nullptr;
}

bool UserManager::userExists(const std::string& email, const std::string& fullName)
{
    for (const auto& user : users)
    {
        if (user.getEmail() == email || user.getName() == fullName)
        {
            return true;
        }
    }
    return false;
}

// Append one user record to CSV store
void UserManager::saveUserToCSV(const User& user)
{
    std::ofstream file(csvFilename, std::ios::app);
    if (file.is_open())
    {
        // Format: userId,fullName,email,passwordHash
        file << user.getId() << ","
             << user.getName() << ","
             << user.getEmail() << ","
             << user.getPasswordHash() << std::endl;
        file.close();
    }
    else
    {
        std::cerr << "Error opening users.csv for writing!" << std::endl;
    }
}

// Eagerly load users from CSV; create empty file if absent
void UserManager::loadUsersFromCSV()
{
    std::ifstream file(csvFilename);
    if (!file.is_open())
    {
        // File doesn't exist yet, create empty one
        std::ofstream newFile(csvFilename);
        newFile.close();
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string id, name, email, hash;

        if (std::getline(ss, id, ',') && 
            std::getline(ss, name, ',') &&
            std::getline(ss, email, ',') &&
            std::getline(ss, hash, ','))
        {
            users.emplace_back(id, name, email, hash);
        }
    }
    file.close();
}
