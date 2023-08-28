#include <iostream>
#include <sstream>
#include <cstring>
#include <ctime>
#include <cmath>
#include <random>   
#include <vector>
#include "./includes/data_sys.hpp"
using namespace std;

class SlippageModel {
public:
    SlippageModel(PriceHistory& priceHistory, double slippagePercentage)
        : priceHistory(priceHistory), slippagePercentage(slippagePercentage) {}

    double calculateSlippage(time_t entryDate, double entryPrice, time_t exitDate, double exitPrice, double tradeVolume) {
        PricePoint entryDataPoint = priceHistory.getDataPoint(entryDate);
        PricePoint exitDataPoint = priceHistory.getDataPoint(exitDate);

        double entryActualPrice = calculateActualPrice(entryDataPoint.getClosing(), entryPrice);
        double exitActualPrice = calculateActualPrice(exitDataPoint.getClosing(), exitPrice);

        double slippage = entryActualPrice - exitActualPrice;
        double volumeFactor = calculateVolumeFactor(tradeVolume);

        return slippage * volumeFactor;
    }

private:
    PriceHistory& priceHistory;
    double slippagePercentage;

    double calculateActualPrice(double referencePrice, double requestedPrice) {
        double maxSlippage = referencePrice * slippagePercentage;
        double actualPrice = requestedPrice;

        if (actualPrice > referencePrice) {
            actualPrice = min(referencePrice + maxSlippage, actualPrice);
        } else if (actualPrice < referencePrice) {
            actualPrice = max(referencePrice - maxSlippage, actualPrice);
        }

        return actualPrice;
    }

      double calculateVolumeFactor(double tradeVolume) {
        
        double volumeFactor = 1.0;

        
        const vector<pair<double, double>> volumeTiers = {
            {1000.0, 1.0},    
            {10000.0, 0.95},  
            {50000.0, 0.9},   
            {100000.0, 0.85}  
        };

        
        for (const auto& tier : volumeTiers) {
            if (tradeVolume >= tier.first) {
                volumeFactor = tier.second;
            } else {
                break;
            }
        }

        
        volumeFactor = max(volumeFactor, 0.5);

        
        double marketVolatility = calculateMarketVolatility();
        volumeFactor *= calculateVolatilityAdjustment(marketVolatility);

        return volumeFactor;
    }
    double calculateMarketVolatility() {
        vector<double> priceChanges = generateRandomPriceChanges(30); 
        double averageChange = calculateAverage(priceChanges);
        double squaredDifferencesSum = calculateSquaredDifferencesSum(priceChanges, averageChange);
        double volatility = sqrt(squaredDifferencesSum / priceChanges.size());
        return volatility;
    }
    double calculateVolatilityAdjustment(double marketVolatility) {
        double volatilityFactor = 1.0 - 0.5 * tanh(marketVolatility);
        return max(volatilityFactor, 0.5);
    }

    vector<double> generateRandomPriceChanges(size_t numDays) {
        vector<double> priceChanges;
        random_device rd;  
        mt19937 generator(rd()); 
        normal_distribution<double> distribution(0.0, 0.02); 
        for (size_t i = 0; i < numDays; ++i) {
            priceChanges.push_back(distribution(generator));
        }
        return priceChanges;
    }
    double calculateAverage(const vector<double>& values) {
        double sum = 0.0;
        for (double value : values) {
            sum += value;
        }
        return sum / values.size();
    }
    double calculateSquaredDifferencesSum(const vector<double>& values, double mean) {
        double sum = 0.0;
        for (double value : values) {
            double difference = value - mean;
            sum += difference * difference;
        }
        return sum;
    }
};

int main() {
    
    PriceHistory priceHistory("AAPL");
    priceHistory.fetchHistoricalData("2023-01-01", "2023-08-01", "1d");

    
    SlippageModel slippageModel(priceHistory, 0.01);

    
    time_t entryDate = convertDateToEpoch("2023-03-15");
    double entryPrice = 150.0;
    time_t exitDate = convertDateToEpoch("2023-06-15");
    double exitPrice = 170.0;
    double tradeVolume = 10000.0;  

    double calculatedSlippage = slippageModel.calculateSlippage(entryDate, entryPrice, exitDate, exitPrice, tradeVolume);
    cout << "Calculated slippage: " << calculatedSlippage << endl;

    return 0;
}
