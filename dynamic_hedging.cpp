#include <iostream>
#include <sstream>
#include <cstring>
#include "./include/data_sys.hpp"
class DynamicHedging {
public:
    DynamicHedging(const string& assetSymbol, const char* startDate, const char* endDate, const char* interval)
        : priceHistory(assetSymbol) {
        priceHistory.fetchHistoricalData(startDate, endDate, interval);
    }

void performDynamicHedging() {
    const size_t shortWindow = 20; 
    const size_t longWindow = 50;  

    size_t dataSize = priceHistory.dataPointsCount();
    if (dataSize < longWindow) {
        cout << "Insufficient data for hedging." << endl;
        return;
    }

    vector<double> shortMA(dataSize - shortWindow + 1, 0.0);
    vector<double> longMA(dataSize - longWindow + 1, 0.0);
    for (size_t i = 0; i < dataSize - shortWindow + 1; ++i) {
        double sumShort = 0.0;
        double sumLong = 0.0;
        for (size_t j = 0; j < shortWindow; ++j) {
            sumShort += priceHistory.getDataPoint(i + j).getClosing();
            if (j < longWindow) {
                sumLong += priceHistory.getDataPoint(i + j).getClosing();
            }
        }
        shortMA[i] = sumShort / shortWindow;
        longMA[i] = sumLong / longWindow;
    }
    cout << "Moving Averages:" << endl;
    for (size_t i = 0; i < shortMA.size(); ++i) {
        PricePoint currentDataPoint = priceHistory.getDataPoint(i + shortWindow - 1);
        cout << "Date: " << currentDataPoint.getDateString()
             << " Short MA: " << shortMA[i]
             << " Long MA: " << longMA[i] << endl;
    }
}


private:
    PriceHistory priceHistory;
};

int main() {
    const char* startDate = "2023-01-01";
    const char* endDate = "2023-08-01";
    const char* interval = "1d";
    string assetSymbol = "AAPL"; 

    DynamicHedging dynamicHedging(assetSymbol, startDate, endDate, interval);
    dynamicHedging.performDynamicHedging();

    return 0;
}
