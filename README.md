# RA-Assignment3
This project corresponds to the third assignment of the Randomized Algorithms course from the MIRI master's program at UPC.

## Structure

- ```datasets/```: Contains all datasets used by the program.
- ```Estimators/```: Includes implementations of cardinality estimators, with each estimator in its own file. The base class is defined in ```CardinalityEstimator.hpp```.
-  ```utils/```: Contains utility files, including:
    - A test file for program testing.
    - The data stream generator.
    - The xxHash library for hashing.

## Usage Instructions
### 1. **Compile the Program**  
   First, compile the program using the command:
```bash
make all
```

### 2. **Run the Program**  
The program **./Cardinality** can be executed in two modes: **cardinality estimation** and **random data stream generation**.

#### Mode 1: Cardinality Estimation
To run the cardinality estimators, execute the program with the following parameters:
```bash
./Cardinality --file=<file_path> --param=<param> [--seed=<seed>] [--estimator=HLL|REC|PCSA]

```
- **-f (--file)**: Specifies the dataset file name, located in the ```datasets/``` folder. Examples:
    - ```dracula.txt```
    - ```streams/random_stream-n5000-N40000-a4.txt```
- **-p (--param)**: Specifies the parameter parameter value (*k* or *m*) for the estimators.
- **-s (--seed)** (optional): Specifies a seed value for the hash function. If omitted, a random seed is used.
- **[-e (--estimator)=HLL|REC|PCSA]** (optional): Specifies the estimator to run (HLL|REC|PCSA) as parameter. If provided it will just execute the indicated estimator. Otherwise will execute all of them.

#### Mode 2: Random Data Stream Generation
To generate a random datastream, execute the program with the following parameters:
```bash
./Cardinality -n=<number> -N=<number> --alpha=<alpha_value>
```
- **-n**: Specifies the number of distinct elements in the data stream.
- **-N**: Specifies the total number of values in the generated data stream.
- **-a (--alpha)**: Specifies the skewness parameter $\alpha \in [0.0 , 1.0]$.
