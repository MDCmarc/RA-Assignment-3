//
// Created by Marc on 20/12/2024.
//

#ifndef RA_3_RECORDINALITY_H
#define RA_3_RECORDINALITY_H

#include <cmath>
#include <algorithm>
#include <fstream>
#include <set>

#include "CardinalityEstimator.h"

using std::string;

class Recordinality : public CardinalityEstimator {
public:
    explicit Recordinality(const uint32_t k, const uint32_t seed)
            : CardinalityEstimator(seed), k_(k)
    {};

    uint32_t EstimateCardinality(const string& path);

private:
    const uint32_t k_;
};

uint32_t Recordinality::EstimateCardinality(const string& path) {
    std::ifstream file(path);
    string word;

    std::set<uint32_t> hashes; // Size of k * 64 bits
    uint32_t r = 0;
    while (r < k_ && file >> word ) {
        uint32_t y = ComputeXXHash32(word);
        if (hashes.insert(y).second) r++;
    }

    auto min_node = hashes.begin();
    while (file >> word) {
        uint32_t y = ComputeXXHash32(word);
        if (y > *min_node && hashes.find(y) == hashes.end() ) {
            hashes.erase(min_node);
            hashes.insert(y);
            min_node = hashes.begin();
            r++;
        }
    }

    double E = k_ * std::pow(1 + 1.0 / k_, r - k_ + 1) - 1;
    return static_cast<uint32_t>(E);
}

#endif //RA_3_RECORDINALITY_H
