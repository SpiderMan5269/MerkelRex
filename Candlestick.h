#pragma once
#include <string>

// Minimal OHLC record used when summarizing per-timestamp prices
class Candlestick {
public:
    std::string date; // e.g. 2023-06-01
    double open;
    double high;
    double low;
    double close;

    Candlestick(const std::string& date, double open, double high, double low, double close)
        : date(date), open(open), high(high), low(low), close(close) {}
};
