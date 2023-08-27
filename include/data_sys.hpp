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
 
    double getOpening() const; // Mark as const
    double getHighest() const; // Mark as const
    double getLowest() const;  // Mark as const
    double getClosing() const; // Mark as const

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
 size_t dataPointsCount() const; 
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

private:
    string assetSymbol;
    vector<PricePoint> dataPoints;
};
 