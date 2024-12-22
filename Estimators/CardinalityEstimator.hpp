//
// Created by Marc on 20/12/2024.
//

#ifndef RA_3_CARDINALITY_ESTIMATOR_H
#define RA_3_CARDINALITY_ESTIMATOR_H

#include <cstdint>
#include "../utils/xxhash.hpp"

using std::string;

class CardinalityEstimator {

public:
    explicit CardinalityEstimator(const uint32_t seed) : seed_(seed)
    {};

protected:
    [[nodiscard]] uint32_t ComputeXXHash32(const std::string& input) const;
    [[nodiscard]] static constexpr uint32_t ExtractHighBits(uint32_t value, uint8_t pos) ;
    [[nodiscard]] static constexpr uint32_t ExtractLowBits(uint32_t value, uint8_t pos) ;
    [[nodiscard]] static constexpr uint8_t FindFirstSetBit(uint32_t tail, uint8_t start_pos) ;

private:
    const uint32_t seed_;
};



uint32_t CardinalityEstimator::ComputeXXHash32(const std::string& input) const {
    return xxh::xxhash<32>(input.c_str(), input.size(),seed_);
}

constexpr uint32_t CardinalityEstimator::ExtractHighBits(const uint32_t value, const uint8_t pos) {
    return value >> (32 - pos);
}

constexpr uint32_t CardinalityEstimator::ExtractLowBits(const uint32_t value, const uint8_t pos) {
    return value & ((1UL << (32 - pos)) - 1);
}

// Given a start position [0-31], returns the position of the first bit set to 1 in the range [1-32]
constexpr uint8_t CardinalityEstimator::FindFirstSetBit(const uint32_t tail, const uint8_t start_pos) {
    int adjusted_clz = std::max((int)__builtin_clz(tail) - static_cast<int>(start_pos), 0);
    return static_cast<uint8_t>(adjusted_clz + 1); // Range 1-32
}



#endif //RA_3_CARDINALITY_ESTIMATOR_H
