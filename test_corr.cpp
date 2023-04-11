 
#include "sys.cpp"
#include <cmath>
#include <numeric>
#include "correlation_analysis.cpp"

int main() {
    PriceHistory historyA("AAPL"); 
    PriceHistory historyB("TSLA");

 
    historyA.fetchHistoricalData("2022-01-01", "2022-12-31", "1d");
    historyB.fetchHistoricalData("2022-01-01", "2022-12-31", "1d");

    CorrelationAnalysis correlationAnalysis(historyA, historyB);
    double correlation = correlationAnalysis.calculateCorrelation();

    cout << "Correlation between AAPL and MSFT: " << correlation << endl;

    return 0;
}
