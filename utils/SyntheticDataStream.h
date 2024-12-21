//
// Created by Marc on 21/12/2024.
//

#ifndef RA_3_SYNTHETIC_DATA_STREAM_H
#define RA_3_SYNTHETIC_DATA_STREAM_H

#include <numeric>
#include <random>
#include <fstream>

class SyntheticDataStream {
public:
    // GenerateStream creates a new random instance file in the Synthetic folder
    static void GenerateStream(int n, int N, double alpha);

private:
    static constexpr double CalculateCn(int n, double alpha);
};

constexpr double SyntheticDataStream::CalculateCn(int n, double alpha) {
    double sum = 0.0;
    for (int i = 1; i <= n; ++i) sum += 1.0 / std::pow(i, alpha);
    return 1.0 / sum;
}

void SyntheticDataStream::GenerateStream(int n, int N, double alpha) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double cn = CalculateCn(n, alpha);
    std::vector<double> probabilities(n,0);
    for (int i = 0; i < n; ++i) {
        probabilities[i] = cn / std::pow(i + 1, alpha); // i+1 because indices start at 1
    }

    // Create a cumulative distribution
    std::vector<double> cumulative(n,0);
    std::partial_sum(probabilities.begin(), probabilities.end(), cumulative.begin());

    char filename[256];
    std::string path = R"(C:\Users\Marc\Desktop\RA-3\random-streams)";
    std::sprintf(filename, "%s\\random_stream-n%d-N%d-a%d.txt", path.c_str(), n, N, int(alpha*10));
    std::ofstream file(filename);
    for( int i = 0; i< N; ++i){
        auto it = std::lower_bound(cumulative.begin(), cumulative.end(), dist(rng));
         file << (it - cumulative.begin()) + 1 << '\n';
    }
}

#endif //RA_3_SYNTHETIC_DATA_STREAM_H
