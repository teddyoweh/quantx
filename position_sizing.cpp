
#include <iostream>
#include <sstream>
#include <cstring>
 
class PositionSizing {
public:
    PositionSizing(double riskPercentage, double stopLossPercentage)
        : riskPercentage(riskPercentage), stopLossPercentage(stopLossPercentage) {}
    void setRiskPercentage(double percentage) {
        riskPercentage = percentage;
    }
    void setStopLossPercentage(double percentage) {
        stopLossPercentage = percentage;
    }
       double calculatePositionSize(double portfolioSize, double entryPrice) {
        double riskAmount = portfolioSize * riskPercentage;
        double stopLossAmount = entryPrice * stopLossPercentage;
        double positionSize = riskAmount / stopLossAmount;
        return positionSize;
    }
    double calculatePositionSizeWithMaxLoss(double portfolioSize, double entryPrice, double maxLossAmount) {
        double positionSize = maxLossAmount / (entryPrice * stopLossPercentage);
        return positionSize;
    }

private:
    double riskPercentage;      
    double stopLossPercentage;  
};
