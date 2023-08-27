#ifndef CORRELATION_ANALYSIS_HPP
#define CORRELATION_ANALYSIS_HPP

#include "data_sys.hpp" 

class CorrelationAnalysis {
public:
    CorrelationAnalysis(PriceHistory &historyA, PriceHistory &historyB);
    double calculateCorrelation();

private:
    PriceHistory &historyA;
    PriceHistory &historyB;
};

#endif  
