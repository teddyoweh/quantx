#include <iostream>
#include <sstream>
#include <cstring>
#include <ctime>
#include <curl/curl.h>
#include <vector>
using namespace std;
time_t getCurrentPosixTimestamp();
time_t convertDateToEpoch(const char *date);
string convertEpochToDate(const time_t epoch);
bool isDateBeforeOrEqual(const char *dateA, const char *dateB);

class PricePoint {

public:
 
    PricePoint(time_t date, double opening, double highest, double lowest, double closing);
    PricePoint(string date, double opening, double highest, double lowest, double closing);
    PricePoint(time_t date, double price);
    PricePoint(string date, double price);
    ~PricePoint();
    time_t getDate();
    string getDateString();
 
    double getOpening();
    double getHighest();
    double getLowest(); 
    double getClosing();

    string toString();
    void printPricePoint();
private:
    time_t date;
    double opening;
    double highest;
    double lowest;
    double closing;
};

size_t responseCallback(char *data, size_t itemSize, size_t itemCount, void *userData);
string fetchYahooCsvData(
    string stockSymbol,
    time_t startTime,
    time_t endTime,
    string intervalType
);
class PriceHistory {

public:
    PriceHistory(string assetSymbol);
    ~PriceHistory();
 size_t dataPointsCount(); 
    PricePoint getDataPoint(size_t index);
    PricePoint getDataPoint(time_t date);
    PricePoint getDataPoint(string date);
    void printDataPoints();
    void clearDataPoints();
    string getHistoricalCsv(time_t startDate,
                                 time_t endDate,
                                 const char *interval);
    void fetchHistoricalData(time_t startDate,
                             time_t endDate,
                             const char *interval);
    void fetchHistoricalData(const char *startDate,
                             const char *endDate,
                             const char *interval);
      void calculateMovingAverages(size_t windowSize, double emaSmoothingFactor);
    void calculatePositionSizeForTrade(
        size_t dataIndex,
        double portfolioSize,
        double riskPercentage,
        double stopLossPercentage
    );
    pair<vector<double>, vector<double>> calculateSMA(size_t windowSize);
    pair<vector<double>, vector<double>> calculateEMA(size_t windowSize, double emaSmoothingFactor);
 
    vector<double> calculateBollingerBands(size_t period, double stdDevFactor);
    double PriceHistory::performMonteCarloSimulationAdvanced(size_t numSimulations, double initialInvestment);
private:
    string assetSymbol;
    vector<PricePoint> dataPoints;
};
 