#include <iostream>
#include <string>
#include <getopt.h>
#include <cstdlib>
#include <cmath>

#include "SyntheticDataStream.h"
#include "Estimators/HyperLogLog.h"
#include "Estimators/Recordinality.h"
#include "Estimators/ProbabilisticCounting.h"

using namespace std;

void printUsage() {
    std::cout << "Usage:\n"
              << "Option 1: --file=<file_path> --param=<param> [--seed=<seed>] [--estimator=HLL|REC|PCSA]\n"
              << "Option 2: -n=<number> -N=<number> -alpha=<alpha_value>\n";
}

int main(int argc, char* argv[]) {

    srand(time(nullptr));
    string path; uint32_t param = 0, seed = rand(); string estimator;

    int n = 0, N = 0; double alpha = 0.0;

    // Define long options for getopt_long
    struct option long_options[] = {
            {"file", required_argument, nullptr, 'f'},
            {"param", required_argument, nullptr, 'p'},
            {"seed", required_argument, nullptr, 's'},
            {"estimator", required_argument, nullptr, 'e'},
            {"n", required_argument, nullptr, 'n'},
            {"N", required_argument, nullptr, 'N'},
            {"alpha", required_argument, nullptr, 'a'},
            {nullptr, 0, nullptr, 0} // End of options
    };
    int opt;
    while ((opt = getopt_long(argc, argv, "f:p:s:E:n:N:A:", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'f':
                path = optarg; // path
                break;
            case 'p':
                param = std::stoul(optarg); // param
                break;
            case 's':
                seed = std::stoul(optarg); // seed
                break;
            case 'e':
                estimator = optarg; // estimator
                break;
            case 'n':
                n = std::stoi(optarg); // n
                break;
            case 'N':
                N = std::stoi(optarg); // N
                break;
            case 'a':
                alpha = std::stod(optarg); // alpha
                break;
            default:
                printUsage();
                return 1;
        }
    }

    if (!path.empty() && param > 0) {
        if (estimator.empty() || estimator == "REC") {
            Recordinality REC(param, seed);
            int64_t cardinality = REC.EstimateCardinality(path);
            //int64_t error = std::llabs(cardinality - real);
            //double SE = 1.04 / std::sqrt(param);
            //double EE = static_cast<double>(error) / real;
            std::cout << "Recordinality: " << cardinality << std::endl;
            //std::cout << "\tExpected error: " << SE << ", Actual error: " << EE << std::endl;
        }
        if (estimator.empty() || estimator == "HLL") {
            HyperLogLog HLL(param, seed);
            int64_t cardinality = HLL.EstimateCardinality(path);
            //int64_t error = std::llabs(cardinality - real);
            //double SE = 1.04 / std::sqrt(param);
            //double EE = static_cast<double>(error) / real;
            std::cout << "HyperLogLog: " << cardinality << std::endl;
            //std::cout << "\tExpected error: " << SE << ", Actual error: " << EE << std::endl;
        }

        if (estimator.empty() || estimator == "PCSA") {
            ProbabilisticCounting PCSA(param, seed);
            int64_t cardinality = PCSA.EstimateCardinality(path);
            //int64_t error = std::llabs(cardinality - real);
            //double SE = 0.78 / std::sqrt(param);
            //double EE = static_cast<double>(error) / real;
            std::cout << "Probabilistic Counting: " << cardinality << std::endl;
            //std::cout << "\tExpected error: " << SE << ", Actual error: " << EE << std::endl;
        }
    } else if (n > 0 && N > 0 && alpha > 0.0) {
        // Option 2: Generate synthetic data stream
        SyntheticDataStream::GenerateStream(n, N, alpha);
        std::cout << "Synthetic data stream generated with n=" << n << ", N=" << N << ", alpha=" << alpha << std::endl;
    } else {
        printUsage();
        return 1;
    }

    return 0;
}
