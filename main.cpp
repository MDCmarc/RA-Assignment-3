#include <iostream>
#include <getopt.h>
#include <filesystem>
#include <random>

#include "utils/SyntheticDataStream.hpp"
#include "Estimators/HyperLogLog.hpp"
#include "Estimators/Recordinality.hpp"
#include "Estimators/ProbabilisticCounting.hpp"

using namespace std;

void printUsage() {
    std::cout << "Usage:\n"
              << "Option 1: --file=<file_path> --param=<param> [--seed=<seed>] [--estimator=HLL|REC|PCSA]\n"
              << "Option 2: -n=<number> -N=<number> --alpha=<alpha_value>\n";
}

int main(int argc, char* argv[]) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist;

    string path, estimator; 
    uint32_t param = 0, seed = dist(rng);

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
    while ((opt = getopt_long(argc, argv, "f:p:s:e:n:N:a:", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'f':
                path = "datasets/" + std::string(optarg);
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
        // Option 1: Run Cardinality Estimators
        if(!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)){
            cerr << "Path specified does not exist: "<< path << endl;
            return 1;
        }
        if (estimator.empty() || estimator == "REC") {
            Recordinality REC(param, seed);
            std::cout << "Recordinality: " << REC.EstimateCardinality(path) << std::endl;
        }
        if (estimator.empty() || estimator == "HLL") {
            HyperLogLog HLL(param, seed);
            std::cout << "HyperLogLog: " << HLL.EstimateCardinality(path) << std::endl;
        }

        if (estimator.empty() || estimator == "PCSA") {
            ProbabilisticCounting PCSA(param, seed);
            std::cout << "Probabilistic Counting: " << PCSA.EstimateCardinality(path) << std::endl;
        }
    } 
    else if (n > 0 && N > 0 && alpha > 0.0) {
        // Option 2: Generate synthetic data stream
        SyntheticDataStream::GenerateStream(n, N, alpha);
        std::cout << "Synthetic data stream generated with n=" << n << ", N=" << N << ", alpha=" << alpha << std::endl;
    } else {
        printUsage();
        return 1;
    }

    return 0;
}
