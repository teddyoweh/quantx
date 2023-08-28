 
 
#include <cmath>
#include <algorithm>
#include <cmath>
#include "sys.cpp"
using namespace std;
class MarketDataAnalysis {
public:
    MarketDataAnalysis(string assetSymbol);

    double calculateAveragePrice();
    double calculateVolatility();
    vector<PricePoint> getUpwardTrends();
    vector<PricePoint> getDownwardTrends();
    double calculateRelativeStrengthIndex(size_t period);
 
    
    PriceHistory priceHistory_;
};


MarketDataAnalysis::MarketDataAnalysis(string assetSymbol) : priceHistory_(assetSymbol) {}

double MarketDataAnalysis::calculateAveragePrice() {
    double total = 0.0;
    size_t count = priceHistory_.dataPointsCount();

    for (size_t i = 0; i < count; ++i) {
        total += priceHistory_.getDataPoint(i).getClosing();
    }

    return total / count;
}

double MarketDataAnalysis::calculateVolatility() {
    size_t count = priceHistory_.dataPointsCount();
    if (count < 2) {
        return 0.0;
    }

    double sumSquaredDifferences = 0.0;
    double averagePrice = calculateAveragePrice();

    for (size_t i = 0; i < count; ++i) {
        double price = priceHistory_.getDataPoint(i).getClosing();
        sumSquaredDifferences += (price - averagePrice) * (price - averagePrice);
    }

    return sqrt(sumSquaredDifferences / (count - 1));
}

vector<PricePoint> MarketDataAnalysis::getUpwardTrends() {
    vector<PricePoint> upwardTrends;
    size_t count = priceHistory_.dataPointsCount();

    for (size_t i = 1; i < count; ++i) {
        PricePoint prevPoint = priceHistory_.getDataPoint(i - 1);
        PricePoint currentPoint = priceHistory_.getDataPoint(i);
        
        if (currentPoint.getClosing() > prevPoint.getClosing()) {
            upwardTrends.push_back(currentPoint);
        }
    }

    return upwardTrends;
}

vector<PricePoint> MarketDataAnalysis::getDownwardTrends() {
    vector<PricePoint> downwardTrends;
    size_t count = priceHistory_.dataPointsCount();

    for (size_t i = 1; i < count; ++i) {
        PricePoint prevPoint = priceHistory_.getDataPoint(i - 1);
        PricePoint currentPoint = priceHistory_.getDataPoint(i);
        
        if (currentPoint.getClosing() < prevPoint.getClosing()) {
            downwardTrends.push_back(currentPoint);
        }
    }

    return downwardTrends;
}
 
 double MarketDataAnalysis::calculateRelativeStrengthIndex(size_t period) {
    size_t count = priceHistory_.dataPointsCount();
    if (count <= period) {
        return 0.0; 
    }
    double avgGain = 0.0;
    double avgLoss = 0.0;
    for (size_t i = 1; i < period; ++i) {
        double diff = priceHistory_.getDataPoint(i).getClosing() - priceHistory_.getDataPoint(i - 1).getClosing();
        if (diff > 0) {
            avgGain += diff;
        } else {
            avgLoss -= diff; 
        }
    }
    avgGain /= period;
    avgLoss /= period;
    double relativeStrength = avgGain / avgLoss;
    double rsi = 100.0 - (100.0 / (1.0 + relativeStrength));

    return rsi;
}
 

 



