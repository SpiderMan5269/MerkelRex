#pragma once

#include <string>
#include <iomanip>
#include <sstream>

// Immutable record describing a user-facing balance event
class Transaction
{
public:
    std::string timestamp;
    std::string userId;
    std::string type;        // "buy", "sell", "deposit", "withdraw"
    std::string product;     // e.g., "ETH/BTC"
    double amount;           // quantity of product
    double price;            // price per unit
    double total;            // amount * price
    double walletBalanceAfter; // USDT balance after transaction

    Transaction();
    Transaction(const std::string& timestamp, const std::string& userId,
                const std::string& type, const std::string& product,
                double amount, double price, double total, double walletBalance);

    std::string toCSVString() const;           // Serialize to CSV row
    static Transaction fromCSVString(const std::string& line); // Parse from CSV row
};
