#include <iostream>
#include "sys.cpp"
using namespace std;

int main() {
    
    PriceHistory *snp500History = new PriceHistory("AAPL");
    PriceHistory *eurusdHistory = new PriceHistory("EURUSD=X");
    PriceHistory *euraudHistory = new PriceHistory("EURAUD=X");

    
    snp500History->fetchHistoricalData("2017-12-01", "2017-12-31", "1d");
    eurusdHistory->fetchHistoricalData("2018-01-01", "2018-01-10", "1d");
    euraudHistory->fetchHistoricalData("2018-01-01", "2018-01-10", "1d");

    
    cout << "Historical data for S&P 500 (Dec 2017):" << endl;
    snp500History->printDataPoints();

    
    try {
        PricePoint dataPoint = snp500History->getDataPoint("2017-12-01");
        cout << "Data point at 2017-12-01:" << endl;
        dataPoint.printPricePoint();
    } catch (const exception &e) {
        cerr << e.what() << endl;
    }

    
    cout << "EUR/USD rates (Jan 2018):" << endl;
    eurusdHistory->printDataPoints();

    
    cout << "EUR/AUD rates (Jan 2018):" << endl;
    euraudHistory->printDataPoints();

    
    delete snp500History;
    delete eurusdHistory;
    delete euraudHistory;
}
