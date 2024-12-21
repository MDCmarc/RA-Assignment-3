//
// Created by Marc on 20/12/2024.
//

#ifndef RA_3_PROBABILISTIC_COUNTING_H
#define RA_3_PROBABILISTIC_COUNTING_H

#include <cmath>
#include <algorithm>
#include <fstream>

#include "CardinalityEstimator.h"

using std::string;

class ProbabilisticCounting : public CardinalityEstimator {
public:
    explicit ProbabilisticCounting(const uint32_t m, const uint32_t seed)
            : CardinalityEstimator(seed),
              m_(m),
              log_m_(static_cast<uint8_t>(std::log2(m))),
              phiM_(m_ * (1.0 / 0.77351))
    {};

    uint32_t EstimateCardinality(const string &path);

private:
    const uint32_t m_;
    const uint8_t log_m_;
    const double phiM_;
};


uint32_t ProbabilisticCounting::EstimateCardinality(const string& path) {
    std::ifstream file(path);
    string word;

    uint8_t bmap[m_][32] = {0}; // m words (<=32 bits)

    while (file >> word) {
        uint32_t y = ComputeXXHash32(word);
        uint32_t head = ExtractHighBits(y, log_m_);
        uint32_t tail = ExtractLowBits(y, log_m_);
        uint8_t p = FindFirstSetBit(tail, log_m_) - 1;
        bmap[head][p] = 1;
    }

    double arithmetic_sum = 0.0;
    for (const auto R : bmap) {
        int count = 0, i = 0;
        while(R[i++]==1) ++count;
        arithmetic_sum+=count;
    }

    double E = phiM_ * std::pow(2, arithmetic_sum/static_cast<double>(m_));
    return static_cast<uint32_t>(E);
}

#endif //RA_3_PROBABILISTIC_COUNTING_H
