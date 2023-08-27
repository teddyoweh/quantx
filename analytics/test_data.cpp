
#include "market_data_analysis.cpp"
#include <iostream>
using namespace std;
int main() {
    MarketDataAnalysis analysis("AAPL"); 
    analysis.priceHistory_.fetchHistoricalData("2023-01-01", "2023-08-31", "1d");
    double averagePrice = analysis.calculateAveragePrice();
    cout << "Average Price: " << averagePrice << endl;    
    double volatility = analysis.calculateVolatility();
    cout << "Volatility: " << volatility << endl;
    vector<PricePoint> upwardTrends = analysis.getUpwardTrends();
    cout << "Upward Trends: " << upwardTrends.size() << " data points" << endl;
    vector<PricePoint> downwardTrends = analysis.getDownwardTrends();
    cout << "Downward Trends: " << downwardTrends.size() << " data points" << endl;

 
    return 0;
}
