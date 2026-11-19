#pragma once

#include <string>

// Centralized input validation helpers for user-facing console prompts
class InputValidator
{
public:
    // Validate and parse numeric input (double)
    static bool parseDouble(const std::string& input, double& result);
    
    // Validate and parse integer input
    static bool parseInt(const std::string& input, int& result);
    
    // Validate email format (basic check)
    static bool isValidEmail(const std::string& email);
    
    // Validate non-empty string
    static bool isValidString(const std::string& str, int minLength = 1);
    
    // Validate product format (e.g., "ETH/BTC")
    static bool isValidProduct(const std::string& product);
};
