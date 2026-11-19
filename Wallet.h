#pragma once

#include <string>
#include <map>
#include "OrderBookEntry.h"
#include <iostream>

// Tracks balances and enforces affordability checks for orders
class Wallet 
{
    public:
        Wallet();
        // Add currency to holdings
        void insertCurrency(std::string type, double amount);
        // Remove currency if available
        bool removeCurrency(std::string type, double amount);
        
        // Verify balance suffices
        bool containsCurrency(std::string type, double amount);
        // Ensure funds exist for an incoming order
        bool canFulfillOrder(OrderBookEntry order);
        // Apply post-trade balance changes
        void processSale(OrderBookEntry& sale);

        // Fiat on/off ramps
        void depositMoney(double amount);
        bool withdrawMoney(double amount);
        double getUSDTBalance() const;

        // Human-readable snapshot
        std::string toString();
        friend std::ostream& operator<<(std::ostream& os, Wallet& wallet);

        // CSV persistence for wallet balances
        void saveToCSV(const std::string& userId, const std::string& filename = "wallets.csv");
        void loadFromCSV(const std::string& userId, const std::string& filename = "wallets.csv");

    private:
        std::map<std::string,double> currencies;

};



	

