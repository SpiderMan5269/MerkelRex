#include "InputValidator.h"
#include <sstream>
#include <cctype>

// Defensive parsers used to keep menu and form inputs resilient

bool InputValidator::parseDouble(const std::string& input, double& result)
{
    if (input.empty())
        return false;
    
    try
    {
        size_t idx = 0;
        result = std::stod(input, &idx);
        // Check if entire string was consumed
        return idx == input.length();
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool InputValidator::parseInt(const std::string& input, int& result)
{
    if (input.empty())
        return false;
    
    try
    {
        size_t idx = 0;
        long val = std::stol(input, &idx);
        if (idx != input.length())
            return false;
        
        if (val > INT_MAX || val < INT_MIN)
            return false;
        
        result = (int)val;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

bool InputValidator::isValidEmail(const std::string& email)
{
    if (email.empty() || email.length() < 5)
        return false;
    
    size_t atPos = email.find('@');
    if (atPos == std::string::npos || atPos == 0 || atPos == email.length() - 1)
        return false;
    
    size_t dotPos = email.find('.', atPos);
    if (dotPos == std::string::npos || dotPos == email.length() - 1)
        return false;
    
    return true;
}

bool InputValidator::isValidString(const std::string& str, int minLength)
{
    if ((int)str.length() < minLength)
        return false;
    
    // Check for non-whitespace characters
    for (char c : str)
    {
        if (!std::isspace(c))
            return true;
    }
    return false;
}

bool InputValidator::isValidProduct(const std::string& product)
{
    if (product.empty() || product.length() < 3)
        return false;
    
    size_t slashPos = product.find('/');
    if (slashPos == std::string::npos || slashPos == 0 || slashPos == product.length() - 1)
        return false;
    
    // Check format is uppercase letters/numbers
    for (char c : product)
    {
        if (c != '/' && !std::isupper(c) && !std::isdigit(c))
            return false;
    }
    
    return true;
}
