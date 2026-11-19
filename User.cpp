#include "User.h"
#include <random>
#include <functional>
#include <sstream>
#include <iomanip>

User::User()
    : userId(""), fullName(""), email(""), passwordHash("")
{
}

User::User(const std::string& id, const std::string& name, const std::string& email, 
           const std::string& passwordHash)
    : userId(id), fullName(name), email(email), passwordHash(passwordHash)
{
}

// Produce a pseudo-random 10-digit identifier as string
std::string User::generateUserId()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(1000000000LL, 9999999999LL);
    return std::to_string(dis(gen));
}

// Deterministic hash (hex) of a plain-text password for storage
std::string User::hashPassword(const std::string& password)
{
    std::hash<std::string> hasher;
    size_t hashValue = hasher(password);
    
    // Convert hash to hexadecimal string for readability
    std::stringstream ss;
    ss << std::hex << hashValue;
    return ss.str();
}

// Compare a candidate password with the stored hash
bool User::verifyPassword(const std::string& plainPassword, const std::string& storedHash)
{
    std::string hash = hashPassword(plainPassword);
    return hash == storedHash;
}
