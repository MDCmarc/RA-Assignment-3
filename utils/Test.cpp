#include <iostream>
#include "../Estimators/CardinalityEstimator.hpp"

#define IS_TRUE(x, y) \
    if (x != y) { \
        std::cerr << __FUNCTION__ << " failed on line " << __LINE__ << ": " << #x << '\n'; \
        std::cerr << "Got: "<< x << ", expected: "<< y << std::endl; \
    }
class CardinalityEstimatorTest : public CardinalityEstimator {
public:
    CardinalityEstimatorTest() : CardinalityEstimator(0) {}

    void TestExtractHighBits() {
        uint32_t value = 0b11110000111100001111000011110000;
        IS_TRUE(ExtractHighBits(value, 4) , 0b1111);
        IS_TRUE(ExtractHighBits(value, 8) , 0b11110000);
        IS_TRUE(ExtractHighBits(value, 1) , 0b1);

    }

    void TestExtractLowBits() {
        uint32_t value = 0b11110000111100001111000011110001;
        IS_TRUE(ExtractLowBits(value, 4) , 0b0000111100001111000011110001);
        IS_TRUE(ExtractLowBits(value, 8) , 0b111100001111000011110001);
        IS_TRUE(ExtractLowBits(value, 31) , 0b1);
    }


    void TestFindFirstSetBit() {
        uint32_t tail =  (uint32_t)0b00000000100000000000000000000001; // Bit 24 is set
        IS_TRUE(FindFirstSetBit(tail, 0) , 9); // 32 - 24 + 1
        IS_TRUE(FindFirstSetBit(tail, 8) , 1); // Start position adjusted
        uint32_t value =  0xFA000000; // 0x11111010
        IS_TRUE(FindFirstSetBit(value, 0) , 1); // No zeros First position 0
        IS_TRUE(FindFirstSetBit(ExtractLowBits(value,1), 0) , 2); // 0x01111010
        IS_TRUE(FindFirstSetBit(ExtractLowBits(value,2), 0) , 3); // 0x00111010
        IS_TRUE(FindFirstSetBit(ExtractLowBits(value,3), 0) , 4); // 0x00011010
        IS_TRUE(FindFirstSetBit(ExtractLowBits(value,4), 0) , 5); // 0x00011010
        IS_TRUE(FindFirstSetBit(ExtractLowBits(value,5), 0) , 7); // 0x00000010
        IS_TRUE(FindFirstSetBit(ExtractLowBits(value,5), 5) , 2); // 0x00000010
        IS_TRUE(FindFirstSetBit(ExtractLowBits(value,5), 6) , 1); // 0x00000010
    }

    void RunAllTests() {
        TestExtractHighBits();
        TestExtractLowBits();
        TestFindFirstSetBit();
    }
};

int main() {
    CardinalityEstimatorTest test;
    test.RunAllTests();
    std::cout << "All tests completed.\n";
    return 0;
}
