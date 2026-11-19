#include "TradingSimulator.h"
#include <ctime>
#include <random>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

TradingSimulator::TradingSimulator()
{
}

// Create a small batch of asks/bids per product at the current time
std::vector<OrderBookEntry> TradingSimulator::generateSimulatedOrders(
    const std::vector<std::string>& products,
    const std::vector<OrderBookEntry>& historicalData)
{
    std::vector<OrderBookEntry> orders;
    std::string timestamp = getCurrentTimestamp();

    // Generate 5 asks and 5 bids for each product
    for (const auto& product : products)
    {
        for (int i = 0; i < 5; i++)
        {
            // Generate ask
            double askPrice = calculatePrice(product, historicalData, true);
            OrderBookEntry ask(askPrice, 0.5 + (i * 0.1), timestamp, product, 
                             OrderBookType::ask, "simuser");
            orders.push_back(ask);

            // Generate bid
            double bidPrice = calculatePrice(product, historicalData, false);
            OrderBookEntry bid(bidPrice, 0.5 + (i * 0.1), timestamp, product, 
                             OrderBookType::bid, "simuser");
            orders.push_back(bid);
        }
    }

    return orders;
}

// Create dated orders spread over several past intervals
std::vector<OrderBookEntry> TradingSimulator::generateBackdatedOrders(
    const std::vector<std::string>& products,
    const std::vector<OrderBookEntry>& historicalData)
{
    // Backfill dates to make candlestick filtering meaningful
    std::vector<OrderBookEntry> orders;

    // Generate orders backdated to different dates in 2025-2026
    std::vector<int> daysBackList = {1, 7, 14, 30, 60, 90, 180};

    for (const auto& product : products)
    {
        for (int daysBack : daysBackList)
        {
            std::string timestamp = generateBackdatedTimestamp(daysBack);

            // Generate ask
            double askPrice = calculatePrice(product, historicalData, true);
            OrderBookEntry ask(askPrice, 0.3, timestamp, product, 
                             OrderBookType::ask, "simuser");
            orders.push_back(ask);

            // Generate bid
            double bidPrice = calculatePrice(product, historicalData, false);
            OrderBookEntry bid(bidPrice, 0.3, timestamp, product, 
                             OrderBookType::bid, "simuser");
            orders.push_back(bid);
        }
    }

    return orders;
}

// Price from historical average plus variance/spread
double TradingSimulator::calculatePrice(const std::string& product,
                                       const std::vector<OrderBookEntry>& historicalData,
                                       bool isAsk)
{
    double avgPrice = getAveragePrice(product, historicalData);
    if (avgPrice == 0.0)
    {
        avgPrice = 100.0; // Default if no historical data
    }
    return applyVariance(avgPrice, isAsk);
}

// Simple mean over historical entries for this product
double TradingSimulator::getAveragePrice(const std::string& product,
                                        const std::vector<OrderBookEntry>& historicalData)
{
    double sum = 0.0;
    int count = 0;

    for (const auto& entry : historicalData)
    {
        if (entry.product == product)
        {
            sum += entry.price;
            count++;
        }
    }

    return (count > 0) ? (sum / count) : 0.0;
}

// Add random noise and skew slightly for bid/ask spread
double TradingSimulator::applyVariance(double basePrice, bool isAsk)
{
    // Add slight randomness so generated orders are not identical
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.1, 0.1); // ±10% variance

    double variance = dis(gen);
    double newPrice = basePrice * (1.0 + variance);

    // Asks are slightly higher, bids slightly lower
    if (isAsk)
    {
        newPrice += basePrice * 0.005; // +0.5% for asks
    }
    else
    {
        newPrice -= basePrice * 0.005; // -0.5% for bids
    }

    return newPrice;
}

// Current system time formatted as YYYY/MM/DD HH:MM:SS.000000
std::string TradingSimulator::getCurrentTimestamp()
{
    time_t now = time(0);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    std::stringstream ss;
    ss << std::put_time(&timeinfo, "%Y/%m/%d %H:%M:%S");
    return ss.str() + ".000000"; // Add microsecond placeholder
}

// Produce a timestamp N days in the past with random h/m/s
std::string TradingSimulator::generateBackdatedTimestamp(int daysBack)
{
    time_t now = time(0);
    now -= (daysBack * 24 * 3600); // Subtract days

    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    std::stringstream ss;
    ss << std::put_time(&timeinfo, "%Y/%m/%d");
    
    // Add random hour, minute, second
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> hourDis(0, 23);
    std::uniform_int_distribution<> minSecDis(0, 59);

    int hour = hourDis(gen);
    int min = minSecDis(gen);
    int sec = minSecDis(gen);

    ss << std::put_time(&timeinfo, " %H:%M:%S");
    ss << ".000000";

    return ss.str();
}
