# QuantX

QuantX encompasses modules for real-time stock data collection, analysis, AI-driven position sizing, and data insights, providing traders with informed decision-making capabilities. Highlighting dynamic hedging strategies, advanced simulations, and slippage modeling, QuantX facilitates risk reduction and strategy enhancement. With its technical analysis tools and API integration, the project offers a robust platform for quantitative trading and in-depth market analysis.


## Features
 

- **Position Sizing Module:**
  - Calculates position sizes based on risk percentage and stop loss percentage.
  - Supports calculation of position size with a specified maximum loss amount.

- **Moving Average Module:**
  - Computes simple moving averages (SMA) and exponential moving averages (EMA) for a given window size.
  - Tracks and updates moving averages as new data points are added.

- **Mean Reversion Analysis Module:**
  - Analyzes mean reversion using historical price data.
  - Provides methods to calculate mean reversion values, average mean reversion, and identify extreme values.

- **Market Data Analysis Module:**
  - Performs various market data analyses on historical price data.
  - Calculates average price, volatility, upward and downward trends, and the relative strength index (RSI).

- **Dynamic Hedging Module:**
  - Implements dynamic hedging strategies based on moving averages.
  - Determines short and long moving averages windows for analysis.
  - Outputs moving average values and their changes over time.

- **Correlation Analysis Module:**
  - Conducts correlation analysis between two sets of historical price data.
  - Calculates the correlation coefficient to measure the relationship between asset prices.

- **Slippage Modeling Module:**
  - Models slippage effects on trading.
  - Considers slippage due to market conditions and trading volume.
  - Adjusts slippage based on trade volume and market volatility.

- **Price History Management:**
  - Manages historical price data for various assets.
  - Fetches historical price data using Yahoo Finance API.
  - Stores and organizes data points for analysis.

- **Advanced Simulation:**
  - Performs Monte Carlo simulations to assess trading strategy performance.
  - Simulates multiple scenarios with varying initial investments.

- **Time and Date Utilities:**
  - Converts between different time representations (POSIX timestamps and human-readable dates).
  - Compares dates and checks their order.



```cpp
//sys.cpp
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

```
```cpp
//position_sizing.cpp
int main() {
    // Create an instance of the PositionSizing class with a risk percentage and stop loss percentage
    PositionSizing positionSizer(2.5, 0.02); // Example risk: 2.5%, stop loss: 2%

    // Calculate position size based on risk percentage and stop loss
    double portfolioSize = 100000; // Example portfolio size: $100,000
    double entryPrice = 50; // Example entry price: $50 per share
    double positionSize = positionSizer.calculatePositionSize(portfolioSize, entryPrice);
    
    std::cout << "Calculated Position Size: " << positionSize << " shares" << std::endl;

    // Modify risk and stop loss percentages
    positionSizer.setRiskPercentage(3.0); // Change risk percentage to 3%
    positionSizer.setStopLossPercentage(0.03); // Change stop loss percentage to 3%

    // Calculate position size with a maximum loss amount
    double maxLossAmount = 2500; // Example maximum allowable loss: $2,500
    double positionSizeWithMaxLoss = positionSizer.calculatePositionSizeWithMaxLoss(portfolioSize, entryPrice, maxLossAmount);
    
    std::cout << "Position Size with Max Loss: " << positionSizeWithMaxLoss << " shares" << std::endl;

    return 0;
}

```

```cpp
//moving_averages.cpp
int main() {
    // Create a PriceHistory instance for the "AAPL" asset
    PriceHistory priceHistory("AAPL");

    // Fetch historical data for the specified date range and interval
    priceHistory.fetchHistoricalData("2023-01-01", "2023-08-31", "1d");

    // Define window size for moving averages and EMA smoothing factor
    size_t windowSize = 5;
    double emaSmoothingFactor = 0.2;

    // Calculate and display moving averages for the fetched data
    priceHistory.calculateMovingAverages(windowSize, emaSmoothingFactor);

    return 0;
}
```

```cpp
//mean_reversion.cpp
int main() {
    PriceHistory priceHistory("AAPL"); // Create a PriceHistory instance for the "AAPL" asset
    priceHistory.fetchHistoricalData("2023-01-01", "2023-08-31", "1d"); // Fetch historical data

    int movingWindowSize = 10; // Define the moving window size for mean reversion calculation
    MeanReversion meanReversion(priceHistory, movingWindowSize); // Create a MeanReversion instance

    meanReversion.calculateMeanReversion(); // Calculate mean reversion values
    meanReversion.printMeanReversionResults(); // Print mean reversion results

    // Get statistics about mean reversion values
    std::cout << "Average Mean Reversion: " << meanReversion.getAverageMeanReversion() << std::endl;
    std::cout << "Max Mean Reversion Index: " << meanReversion.getMaxMeanReversionIndex() << std::endl;
    std::cout << "Min Mean Reversion Index: " << meanReversion.getMinMeanReversionIndex() << std::endl;

    return 0;
}
```

```cpp
//market_data_analysis.cpp
int main() {
    MarketDataAnalysis marketAnalysis("AAPL"); // Create a MarketDataAnalysis instance for "AAPL" asset
    marketAnalysis.priceHistory_.fetchHistoricalData("2023-01-01", "2023-08-31", "1d"); // Fetch historical data

    double avgPrice = marketAnalysis.calculateAveragePrice();
    double volatility = marketAnalysis.calculateVolatility();
    cout << "Average Price: " << avgPrice << endl;
    cout << "Volatility: " << volatility << endl;

    vector<PricePoint> upwardTrends = marketAnalysis.getUpwardTrends();
    vector<PricePoint> downwardTrends = marketAnalysis.getDownwardTrends();
    cout << "Number of Upward Trends: " << upwardTrends.size() << endl;
    cout << "Number of Downward Trends: " << downwardTrends.size() << endl;

    size_t rsiPeriod = 14; // RSI calculation period
    double rsiValue = marketAnalysis.calculateRelativeStrengthIndex(rsiPeriod);
    cout << "RSI for period " << rsiPeriod << ": " << rsiValue << endl;

    return 0;
}

```

```cpp
//dynamic_hedging.cpp
int main() {
    string assetSymbol = "AAPL";
    const char* startDate = "2023-01-01";
    const char* endDate = "2023-08-31";
    const char* interval = "1d";

    DynamicHedging dynamicHedging(assetSymbol, startDate, endDate, interval);
    dynamicHedging.performDynamicHedging();

    return 0;
}
```cpp
//correlation_analysis.cpp
int main() {
    string assetSymbolA = "AAPL"; // Asset symbol for the first historical data
    string assetSymbolB = "MSFT"; // Asset symbol for the second historical data

    const char* startDate = "2023-01-01";
    const char* endDate = "2023-08-31";
    const char* interval = "1d";

    // Create PriceHistory instances for both assets and fetch historical data
    PriceHistory historyA(assetSymbolA);
    historyA.fetchHistoricalData(startDate, endDate, interval);

    PriceHistory historyB(assetSymbolB);
    historyB.fetchHistoricalData(startDate, endDate, interval);

    // Create a CorrelationAnalysis instance and calculate correlation
    CorrelationAnalysis correlationAnalysis(historyA, historyB);
    double correlation = correlationAnalysis.calculateCorrelation();

    std::cout << "Correlation between " << assetSymbolA << " and " << assetSymbolB << ": " << correlation << std::endl;

    return 0;
}

```

```cpp
//slippage_model
int main() {
    PriceHistory priceHistory("AAPL");
    priceHistory.fetchHistoricalData("2023-01-01", "2023-08-01", "1d");

    SlippageModel slippageModel(priceHistory, 0.01);

    // Convert date strings to time_t using a function like convertDateToEpoch()

    time_t entryDate = convertDateToEpoch("2023-03-15");
    double entryPrice = 150.0;
    time_t exitDate = convertDateToEpoch("2023-06-15");
    double exitPrice = 170.0;
    double tradeVolume = 10000.0;

    double calculatedSlippage = slippageModel.calculateSlippage(entryDate, entryPrice, exitDate, exitPrice, tradeVolume);
    std::cout << "Calculated slippage: " << calculatedSlippage << std::endl;

    return 0;
}
```
