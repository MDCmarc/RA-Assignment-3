//
// Created by Marc on 19/12/2024.
//

#ifndef RA_3_HYPER_LOG_LOG_H
#define RA_3_HYPER_LOG_LOG_H

#include <cmath>
#include <algorithm>
#include <fstream>

#include "CardinalityEstimator.h"

using std::string;

class HyperLogLog : public CardinalityEstimator {
public:
    explicit HyperLogLog(const uint32_t m, const uint32_t seed)
        : CardinalityEstimator(seed),
          m_(m),
          log_m_(static_cast<uint8_t>(std::log2(m))),
          alphaMM_(computeAlphaMM(m))
    {};

    uint32_t EstimateCardinality(const string& path);

private:
    static double computeAlphaMM(uint32_t m);

    const uint32_t m_;
    const uint8_t log_m_;
    const double alphaMM_;
    const double pow_2_32 = 4294967296.0;
};


double HyperLogLog::computeAlphaMM(const uint32_t m) {
    double alpha;
    switch (m) {
        case 16:
            alpha = 0.673;
            break;
        case 32:
            alpha = 0.697;
            break;
        case 64:
            alpha = 0.709;
            break;
        default:
            alpha = 0.7213 / (1.0 + 1.079 / m);
    }
    return alpha * m * m;
}

uint32_t HyperLogLog::EstimateCardinality(const string& path) {
    std::ifstream file(path);
    string word;

    uint8_t R[m_] = {0}; // m bytes (<=5 bits)

    while (file >> word) {
        uint32_t y = ComputeXXHash32(word);
        uint32_t head = ExtractHighBits(y, log_m_);
        uint32_t tail = ExtractLowBits(y, log_m_);
        R[head] = std::max(R[head], FindFirstSetBit(tail, log_m_));
    }

    double harmonic_sum = 0.0;
    int V = 0; // Registers set to 0 (paper)
    for (const uint8_t r : R) {
        harmonic_sum += 1.0 / std::pow(2, r);
        if( r == 0) ++V;
    }

    double E = alphaMM_ / harmonic_sum;

    if (E <= 2.5 * m_ && V > 0 ) {     // Small range correction
        E = m_ * std::log(static_cast<double>(m_) / V);
    }
    else if (E > pow_2_32 / 30.0) {    // Large range correction
        E = -pow_2_32 * std::log(1.0 - E / pow_2_32);
    }

    return static_cast<uint32_t>(E);
}

#endif //RA_3_HYPER_LOG_LOG_H
