#include <cmath>
#include <numeric>
#include "./include/correlation_analysis.hpp"

CorrelationAnalysis::CorrelationAnalysis(PriceHistory &historyA, PriceHistory &historyB)
    : historyA(historyA), historyB(historyB) {}

double CorrelationAnalysis::calculateCorrelation() {
    size_t minDataPoints = std::min(historyA.dataPointsCount(), historyB.dataPointsCount());

    std::vector<double> pricesA;
    std::vector<double> pricesB;

    for (size_t i = 0; i < minDataPoints; ++i) {
        pricesA.push_back(historyA.getDataPoint(i).getClosing());
        pricesB.push_back(historyB.getDataPoint(i).getClosing());
    }

    double meanA = std::accumulate(pricesA.begin(), pricesA.end(), 0.0) / pricesA.size();
    double meanB = std::accumulate(pricesB.begin(), pricesB.end(), 0.0) / pricesB.size();

    double covariance = 0.0;
    double varianceA = 0.0;
    double varianceB = 0.0;

    for (size_t i = 0; i < minDataPoints; ++i) {
        double deviationA = pricesA[i] - meanA;
        double deviationB = pricesB[i] - meanB;

        covariance += deviationA * deviationB;
        varianceA += deviationA * deviationA;
        varianceB += deviationB * deviationB;
    }

    covariance /= minDataPoints;
    varianceA /= minDataPoints;
    varianceB /= minDataPoints;

    double correlation = covariance / (std::sqrt(varianceA) * std::sqrt(varianceB));
    return correlation;
}
