#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "inverted_index.h"
#include <string>
#include <vector>

// Benchmarking structure
struct BenchmarkResult {
    std::string algorithm;
    std::string query;
    double execution_time_ms;
    size_t results_count;
    double top_score;
};

// Function to run benchmark for a specific algorithm
BenchmarkResult run_benchmark(
    SearchAlgorithm algorithm,
    const std::string &query,
    const std::vector<std::string> &docs,
    const InvertedIndex &index,
    int runs = 10);

// Function to run comprehensive benchmark
void run_comprehensive_benchmark(
    const std::vector<std::string> &docs,
    const InvertedIndex &index,
    const std::vector<std::string> &test_queries);

#endif // BENCHMARK_H