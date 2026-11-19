#pragma once

#include <string>
#include <vector>
#include "OrderBookEntry.h"

// Generates synthetic asks/bids to showcase the platform and filtering
class TradingSimulator
{
public:
    TradingSimulator();

    // Generate simulated orders at current timestamp for all products
    std::vector<OrderBookEntry> generateSimulatedOrders(const std::vector<std::string>& products,
                                                       const std::vector<OrderBookEntry>& historicalData);

    // Generate backdated orders (2025-2026) to exercise filtering
    std::vector<OrderBookEntry> generateBackdatedOrders(const std::vector<std::string>& products,
                                                       const std::vector<OrderBookEntry>& historicalData);

private:
    double calculatePrice(const std::string& product, 
                         const std::vector<OrderBookEntry>& historicalData,
                         bool isAsk); // historical average + variance

    double getAveragePrice(const std::string& product,
                          const std::vector<OrderBookEntry>& historicalData);

    double applyVariance(double basePrice, bool isAsk); // ±10% plus small spread

    std::string getCurrentTimestamp();
    std::string generateBackdatedTimestamp(int daysBack);
};
