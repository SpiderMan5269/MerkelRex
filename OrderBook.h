#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#pragma once

#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

// Lightweight in-memory order book sourced from CSV data; provides lookups and matching
class OrderBook
{
    public:
        OrderBook(std::string filename); // load historical orders from CSV

        std::vector<std::string> getKnownProducts();

        // Filter by type, product, and exact timestamp
        std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                              std::string product, 
                                              std::string timestamp);

        std::string getEarliestTime();
        // Returns the next timestamp or wraps to earliest if at end
        std::string getNextTime(std::string timestamp);

        void insertOrder(OrderBookEntry& order);

        // Greedy price-time matching at a single timestamp
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);
        
        // Return all orders for a product and side (ignores timestamp)
        std::vector<OrderBookEntry> getAllOrdersForProduct(std::string product, OrderBookType type);

    private:
        std::vector<OrderBookEntry> orders;
};

