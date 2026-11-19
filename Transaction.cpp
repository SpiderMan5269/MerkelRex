#include "Transaction.h"
#include <sstream>

Transaction::Transaction()
        : timestamp(""), userId(""), type(""), product(""), amount(0), price(0), 
            total(0), walletBalanceAfter(0)
{
}

Transaction::Transaction(const std::string& timestamp, const std::string& userId,
                         const std::string& type, const std::string& product,
                         double amount, double price, double total, double walletBalance)
    : timestamp(timestamp), userId(userId), type(type), product(product),
      amount(amount), price(price), total(total), walletBalanceAfter(walletBalance)
{
}

// Serialize the transaction to a CSV row
std::string Transaction::toCSVString() const
{
    std::stringstream ss;
    ss << timestamp << "," << userId << "," << type << "," << product << ","
       << amount << "," << price << "," << total << "," << walletBalanceAfter;
    return ss.str();
}

// Parse a transaction from a CSV row
Transaction Transaction::fromCSVString(const std::string& line)
{
    std::stringstream ss(line);
    std::string timestamp, userId, type, product;
    double amount, price, total, walletBalance;

    std::getline(ss, timestamp, ',');
    std::getline(ss, userId, ',');
    std::getline(ss, type, ',');
    std::getline(ss, product, ',');
    ss >> amount;
    ss.ignore();
    ss >> price;
    ss.ignore();
    ss >> total;
    ss.ignore();
    ss >> walletBalance;

    return Transaction(timestamp, userId, type, product, amount, price, total, walletBalance);
}
