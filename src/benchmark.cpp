#include "benchmark.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <iomanip>

// Function to run benchmark for a specific algorithm
BenchmarkResult run_benchmark(
    SearchAlgorithm algorithm,
    const std::string &query,
    const std::vector<std::string> &docs,
    const InvertedIndex &index,
    int runs) {
    
    BenchmarkResult result;
    result.query = query;
    
    switch (algorithm) {
        case SearchAlgorithm::TFIDF:
            result.algorithm = "TF-IDF";
            break;
        case SearchAlgorithm::BM25:
            result.algorithm = "BM25";
            break;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < runs; ++i) {
        std::vector<std::pair<DocId, double>> results;
        
        switch (algorithm) {
            case SearchAlgorithm::TFIDF:
                results = search_tfidf(query, docs, index);
                break;
            case SearchAlgorithm::BM25:
                results = search_bm25(query, docs, index);
                break;
        }
        
        if (i == 0) { // Only capture results from first run
            result.results_count = results.size();
            if (!results.empty()) {
                result.top_score = results[0].second;
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    result.execution_time_ms = duration.count() / 1000.0 / runs; // Average time per run
    
    return result;
}

// Function to run comprehensive benchmark
void run_comprehensive_benchmark(
    const std::vector<std::string> &docs,
    const InvertedIndex &index,
    const std::vector<std::string> &test_queries) {
    
    std::vector<BenchmarkResult> all_results;
    
    std::cout << "\n=== RUNING COMPREHENSIVE BENCHMARK ===\n";
    
    for (const auto &query : test_queries) {
        std::cout << "Testing query: '" << query << "'\n";
        
        // Test TF-IDF
        auto tfidf_result = run_benchmark(SearchAlgorithm::TFIDF, query, docs, index);
        all_results.push_back(tfidf_result);
        
        // Test BM25
        auto bm25_result = run_benchmark(SearchAlgorithm::BM25, query, docs, index);
        all_results.push_back(bm25_result);
        
        std::cout << "  " << tfidf_result.algorithm << ": " 
                  << std::fixed << std::setprecision(3) << tfidf_result.execution_time_ms << "ms, "
                  << tfidf_result.results_count << " results, top score: " 
                  << std::setprecision(6) << tfidf_result.top_score << "\n";
        
        std::cout << "  " << bm25_result.algorithm << ": " 
                  << std::fixed << std::setprecision(3) << bm25_result.execution_time_ms << "ms, "
                  << bm25_result.results_count << " results, top score: " 
                  << std::setprecision(6) << bm25_result.top_score << "\n\n";
    }
    
    // Calculate averages
    double avg_tfidf_time = 0.0, avg_bm25_time = 0.0;
    int tfidf_count = 0, bm25_count = 0;
    
    for (const auto &result : all_results) {
        if (result.algorithm == "TF-IDF") {
            avg_tfidf_time += result.execution_time_ms;
            tfidf_count++;
        } else if (result.algorithm == "BM25") {
            avg_bm25_time += result.execution_time_ms;
            bm25_count++;
        }
    }
    
    if (tfidf_count > 0) avg_tfidf_time /= tfidf_count;
    if (bm25_count > 0) avg_bm25_time /= bm25_count;
    
    std::cout << "=== BENCHMARK SUMMARY ===\n";
    std::cout << "Average TF-IDF time: " << std::fixed << std::setprecision(3) << avg_tfidf_time << "ms\n";
    std::cout << "Average BM25 time: " << std::fixed << std::setprecision(3) << avg_bm25_time << "ms\n";
    std::cout << "Performance ratio: " << std::setprecision(2) << (avg_bm25_time / avg_tfidf_time) << "x\n";
    
    // Save results to CSV for analysis
    std::ofstream csv_file("benchmark_results.csv");
    csv_file << "Algorithm,Query,Time(ms),Results,TopScore\n";
    
    for (const auto &result : all_results) {
        csv_file << result.algorithm << ","
                 << result.query << ","
                 << std::fixed << std::setprecision(3) << result.execution_time_ms << ","
                 << result.results_count << ","
                 << std::setprecision(6) << result.top_score << "\n";
    }
    
    csv_file.close();
    std::cout << "Results saved to benchmark_results.csv\n";
}