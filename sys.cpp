#include <iostream>
#include <sstream>
#include <cstring>
#include <ctime>
#include <curl/curl.h>
#include <vector>
#include "./includes/data_sys.hpp"
#include "position_sizing.cpp"
using namespace std;
 


size_t onDataReceived(char *data, size_t size, size_t nmemb, void *userData) {
    ((string*)userData)->append(data, size * nmemb);
    return size * nmemb;
}
string fetchYahooCsvData(
    string stockSymbol,
    time_t startTime,
    time_t endTime,
    string interval
) {
    stringstream ssStart; 
    ssStart << startTime; 
    stringstream ssEnd; 
    ssEnd << endTime;
    string url = "https://query1.finance.yahoo.com/v7/finance/download/"
            + stockSymbol
            + "?period1=" + ssStart.str()
            + "&period2=" + ssEnd.str()
            + "&interval=" + interval
            + "&events=history";

    CURL* curlHandle = curl_easy_init();
    string responseData;
    if (curlHandle) {
        curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, onDataReceived);
        curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &responseData);
        CURLcode result = curl_easy_perform(curlHandle);
        curl_easy_cleanup(curlHandle);
    }

    return responseData;
}

time_t getCurrentEpoch() {
    return time(nullptr);
}

time_t convertDateToEpoch(const char *date) {
    char yearSegment[5] = {0};
    strncpy(yearSegment, date, 4);

    char monthSegment[3] = {0};
    strncpy(monthSegment, date + 5, 2);

    char daySegment[3] = {0};
    strncpy(daySegment, date + 8, 2);

    struct tm timeStruct = {0};
    timeStruct.tm_year = atoi(yearSegment) - 1900;
    timeStruct.tm_mon = atoi(monthSegment) - 1;
    timeStruct.tm_mday = atoi(daySegment);

    return timegm(&timeStruct);
}

string convertEpochToDate(const time_t epoch) {
    struct tm *timeInfo = gmtime(&epoch);
    stringstream yearStream;
    yearStream << timeInfo->tm_year + 1900;

    stringstream monthStream;
    if (timeInfo->tm_mon < 9) {
        monthStream << 0 << timeInfo->tm_mon + 1;
    } else {
        monthStream << timeInfo->tm_mon + 1;
    }

    stringstream dayStream;
    if (timeInfo->tm_mday < 10) {
        dayStream << 0 << timeInfo->tm_mday;
    } else {
        dayStream << timeInfo->tm_mday;
    }

    string dateStr = yearStream.str() + "-" + monthStream.str() + "-" + dayStream.str();
    return dateStr;
}

bool isDateEarlierOrEqual(const char *dateA, const char *dateB) {
    time_t epochA = convertDateToEpoch(dateA);
    time_t epochB = convertDateToEpoch(dateB);

    return epochA <= epochB;
}

 
PricePoint::PricePoint(time_t date, double opening, double highest, double lowest, double closing) {
    this->date = date;
    this->opening = opening;
    this->highest = highest;
    this->lowest = lowest;
    this->closing = closing;
}

PricePoint::PricePoint(string date, double opening, double highest, double lowest, double closing) {
    this->date = convertDateToEpoch(date.c_str());
    this->opening = opening;
    this->highest = highest;
    this->lowest = lowest;
    this->closing = closing;
}

PricePoint::PricePoint(time_t date, double price) {
    this->date = date;
    this->closing = price;
    this->opening = price;
    this->highest = price;
    this->lowest = price;
}

PricePoint::PricePoint(string date, double price) {
    this->date = convertDateToEpoch(date.c_str());
    this->closing = price;
    this->opening = price;
    this->highest = price;
    this->lowest = price;
}

PricePoint::~PricePoint() {}

time_t PricePoint::getDate() {
    return this->date;
}

string PricePoint::getDateString() {
    return convertEpochToDate(this->date);
}

double PricePoint::getOpening() {
    return this->opening;
}

double PricePoint::getHighest() {
    return this->highest;
}

double PricePoint::getLowest() {
    return this->lowest;
}

double PricePoint::getClosing() {
    return this->closing;
}

string PricePoint::toString() {
    ostringstream osOpening;
    osOpening << this->opening;
    ostringstream osHighest;
    osHighest << this->highest;
    ostringstream osLowest;
    osLowest << this->lowest;
    ostringstream osClosing;
    osClosing << this->closing;
    return "{ date: " + this->getDateString()
            + " opening: " + osOpening.str()
            + " highest: " + osHighest.str()
            + " lowest: " + osLowest.str()
            + " closing: " + osClosing.str()
            + " }";
}

void PricePoint::printPricePoint() {
    cout << this->toString() << endl;
}
PriceHistory::PriceHistory(string assetSymbol) {
    this->assetSymbol = assetSymbol;
}

PriceHistory::~PriceHistory() {}

size_t PriceHistory::dataPointsCount() {
    return this->dataPoints.size();
}

PricePoint PriceHistory::getDataPoint(size_t index) {
    if (index < this->dataPoints.size()) {
        return this->dataPoints[index];
    }
    stringstream ss;
    ss << this->dataPoints.size();

    string error = "ERROR: getDataPoint(index) - Index must be less than "
      + ss.str();
    throw invalid_argument(error);
}

PricePoint PriceHistory::getDataPoint(time_t date) {
    for (auto it = this->dataPoints.begin(); it != this->dataPoints.end(); ++it) {
        if (it->getDate() == date) {
            return *it;
        }
    }
    string error = "ERROR: getDataPoint(date) - No data point at " + to_string(date);
    throw invalid_argument(error);
}

PricePoint PriceHistory::getDataPoint(string date) {
    for (auto it = this->dataPoints.begin(); it != this->dataPoints.end(); ++it) {
        if (it->getDateString() == date) {
            return *it;
        }
    }
    string error = "ERROR: getDataPoint(date) - No data point at " + date;
    throw invalid_argument(error);
}

void PriceHistory::printDataPoints() {
    for (auto it = this->dataPoints.begin(); it != this->dataPoints.end(); ++it) {
        it->printPricePoint();
    }
}

void PriceHistory::clearDataPoints() {
    this->dataPoints.clear();
}

void PriceHistory::calculatePositionSizeForTrade(
        size_t dataIndex,
        double portfolioSize,
        double riskPercentage,
        double stopLossPercentage
    ) {
        if (dataIndex >= dataPoints.size()) {
            cout << "Invalid data index." << endl;
            return;
        }

        PositionSizing positionSizer(riskPercentage, stopLossPercentage);
        double entryPrice = dataPoints[dataIndex].getClosing();

        double positionSize = positionSizer.calculatePositionSize(portfolioSize, entryPrice);

        cout << "Position Size for trade at index " << dataIndex << ": " << positionSize << endl;
    }
string PriceHistory::getHistoricalCsv(
    time_t startDate,
    time_t endDate,
    const char *interval
) {
    return fetchYahooCsvData(this->assetSymbol, startDate, endDate, interval);
}
vector<double> PriceHistory::calculateBollingerBands(size_t period, double stdDevFactor) {
    vector<double> bollingerUpper, bollingerLower;
    size_t dataSize = dataPoints.size();

    for (size_t i = period - 1; i < dataSize; ++i) {
        double sum = 0.0;
        for (size_t j = i - period + 1; j <= i; ++j) {
            sum += dataPoints[j].getClosing();
        }
        double movingAvg = sum / period;

        double squaredDiffSum = 0.0;
        for (size_t j = i - period + 1; j <= i; ++j) {
            double diff = dataPoints[j].getClosing() - movingAvg;
            squaredDiffSum += diff * diff;
        }
        double stdDev = sqrt(squaredDiffSum / period);

        bollingerUpper.push_back(movingAvg + stdDevFactor * stdDev);
        bollingerLower.push_back(movingAvg - stdDevFactor * stdDev);
    }

    return bollingerUpper;  
}

 pair<vector<double>, vector<double>> PriceHistory::calculateSMA(size_t windowSize) {
    size_t dataSize = dataPoints.size();
    vector<double> smaValues(dataSize, 0.0);
    for (size_t i = windowSize - 1; i < dataSize; ++i) {
        double sum = 0.0;
        for (size_t j = i - windowSize + 1; j <= i; ++j) {
            sum += dataPoints[j].getClosing();
        }
        smaValues[i] = sum / windowSize;
    }

    return make_pair(smaValues, vector<double>());
}

pair<vector<double>, vector<double>> PriceHistory::calculateEMA(size_t windowSize, double emaSmoothingFactor) {
    size_t dataSize = dataPoints.size();
    vector<double> emaValues(dataSize, 0.0);

    emaValues[windowSize - 1] = dataPoints[windowSize - 1].getClosing();
    for (size_t i = windowSize; i < dataSize; ++i) {
        emaValues[i] = emaSmoothingFactor * dataPoints[i].getClosing() + (1 - emaSmoothingFactor) * emaValues[i - 1];
    }

    return make_pair(vector<double>(), emaValues);
}

double PriceHistory::performMonteCarloSimulationAdvanced(size_t numSimulations, double initialInvestment) {
    size_t dataSize = dataPoints.size();
    double totalProfit = 0.0;

    for (size_t simulation = 0; simulation < numSimulations; ++simulation) {
        double investment = initialInvestment;
        double cash = investment;
        double sharesOwned = 0.0;
        size_t currentDay = 0;
 
        vector<double> movingAveragesShort = calculateSMA(20); 
        vector<double> movingAveragesLong = calculateSMA(50);  

        for (size_t day = 51; day < dataSize; ++day) {

            bool shouldBuy = movingAveragesShort[day - 1] > movingAveragesLong[day - 1] &&
                             movingAveragesShort[day] <= movingAveragesLong[day];

            bool shouldSell = movingAveragesShort[day - 1] < movingAveragesLong[day - 1] &&
                              movingAveragesShort[day] >= movingAveragesLong[day];

            if (shouldBuy) {
                double amountToInvest = cash * 0.2; // Invest 20% of available funds
                double sharesToBuy = amountToInvest / dataPoints[day].getClosing();
                sharesOwned += sharesToBuy;
                cash -= amountToInvest;
            } else if (shouldSell && sharesOwned > 0.0) {
                double amountToSell = sharesOwned * dataPoints[day].getClosing();
                cash += amountToSell;
                sharesOwned = 0.0;
            }
        }

        totalProfit += (cash + sharesOwned * dataPoints[dataSize - 1].getClosing() - initialInvestment);
    }

    return totalProfit / numSimulations;
}

void PriceHistory::fetchHistoricalData(
    time_t startDate,
    time_t endDate,
    const char *interval
) {
    string csvData = this->getHistoricalCsv(startDate, endDate, interval);
    istringstream csvStream(csvData);
    string line;
    getline(csvStream, line);

    while (getline(csvStream, line)) {
        vector<string> dataFields;
        stringstream iss(line);
        string field;
        while (getline(iss, field, ',')) {
            dataFields.push_back(field);
        }

        if (dataFields.size() >= 5 && dataFields[0] != "null" && dataFields[4] != "null") {
            PricePoint dataPoint(
                dataFields[0], // date
                stod(dataFields[1]), // open
                stod(dataFields[2]), // high
                stod(dataFields[3]), // low
                stod(dataFields[4])  // close
            );
            this->dataPoints.push_back(dataPoint);
        }
    }
}

void PriceHistory::fetchHistoricalData(
    const char *startDate,
    const char *endDate,
    const char *interval
) {
    time_t startTimestamp = convertDateToEpoch(startDate);
    time_t endTimestamp = convertDateToEpoch(endDate);

    this->fetchHistoricalData(startTimestamp, endTimestamp, interval);
}

