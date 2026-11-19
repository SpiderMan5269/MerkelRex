#include "Candlestick.h"
#include <vector>
#include <string>

// Simple container for batching candlesticks before display
class CandlestickCollection {
public:
    void addCandlestick(const Candlestick& candle) {
        candlesticks.push_back(candle);
    }
    const std::vector<Candlestick>& getCandlesticks() const {
        return candlesticks;
    }
private:
    std::vector<Candlestick> candlesticks;
};
