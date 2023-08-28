#include <iostream>
#include <vector>
#include <cmath> 
#include "./includes/data_sys.hpp"

class MovingAverage {
public:
    MovingAverage(size_t windowSize);
    void addData(double value);
    double getSMA();
    double getEMA(double smoothingFactor);

private:
    size_t windowSize;
    std::vector<double> data;
};

MovingAverage::MovingAverage(size_t windowSize) : windowSize(windowSize) {}

void MovingAverage::addData(double value) {
    data.push_back(value);
    if (data.size() > windowSize) {
        data.erase(data.begin());
    }
}

double MovingAverage::getSMA() {
    double sum = 0.0;
    for (double value : data) {
        sum += value;
    }
    return sum / data.size();
}

double MovingAverage::getEMA(double smoothingFactor) {
    double ema = data.back(); 

    for (size_t i = data.size() - 2; i < data.size(); --i) {
        ema = (data[i] - ema) * smoothingFactor + ema;
    }

return ema;
}
 
void PriceHistory::calculateMovingAverages(size_t windowSize, double emaSmoothingFactor) {
    MovingAverage sma(windowSize);
    MovingAverage ema(windowSize);

    for (size_t i = 0; i < dataPoints.size(); ++i) {
        double closingPrice = dataPoints[i].getClosing();
        sma.addData(closingPrice);
        ema.addData(closingPrice);

        std::cout << "Date: " << dataPoints[i].getDateString()
                  << " Closing Price: " << closingPrice
                  << " SMA: " << sma.getSMA()
                  << " EMA: " << ema.getEMA(emaSmoothingFactor) << std::endl;
}
}

int main() {
    PriceHistory priceHistory("AAPL");  
    priceHistory.fetchHistoricalData("2023-01-01", "2023-08-31", "1d");
    size_t windowSize = 5;  
    double emaSmoothingFactor = 0.2;  
    priceHistory.calculateMovingAverages(windowSize, emaSmoothingFactor);
    return 0;
}
