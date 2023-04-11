#include <iostream>
#include <vector>

#include "./include/data_sys.hpp"

class MeanReversion {
public:
    MeanReversion(const PriceHistory& history, int windowSize);
    void calculateMeanReversion();
    void printMeanReversionResults();
    double getMeanReversionValue(size_t index) const;
    double getAverageMeanReversion() const;
    size_t getMaxMeanReversionIndex() const;
    size_t getMinMeanReversionIndex() const;

private:
    const PriceHistory& priceHistory;
    int movingWindowSize;
    std::vector<double> meanReversionValues;

    double calculateMovingAverage(size_t startIndex) const;
};

 

double MeanReversion::getMeanReversionValue(size_t index) const {
    if (index < meanReversionValues.size()) {
        return meanReversionValues[index];
    }
    throw std::out_of_range("Index out of range.");
}

double MeanReversion::getAverageMeanReversion() const {
    double sum = 0.0;
    for (double value : meanReversionValues) {
        sum += value;
    }
    return sum / meanReversionValues.size();
}

size_t MeanReversion::getMaxMeanReversionIndex() const {
    double maxValue = *std::max_element(meanReversionValues.begin(), meanReversionValues.end());
    return std::distance(meanReversionValues.begin(), std::find(meanReversionValues.begin(), meanReversionValues.end(), maxValue));
}

size_t MeanReversion::getMinMeanReversionIndex() const {
    double minValue = *std::min_element(meanReversionValues.begin(), meanReversionValues.end());
    return std::distance(meanReversionValues.begin(), std::find(meanReversionValues.begin(), meanReversionValues.end(), minValue));
}
