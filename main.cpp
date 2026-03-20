#include <iostream>
#include <string>
#include <vector>

#include "inverted_index.h"
#include "benchmark.h"

int main() {
    // 1. Example documents (you can replace with file loading later)
    std::vector<std::string> docs = {
        "Hello world, this is a test document.",
        "Another test document about C++ and search.",
        "Search engines build inverted indexes from documents.",
        "C++ can be used to implement simple search engines."
    };

    // 2. Build index
    InvertedIndex index = build_index(docs);

    std::cout << "Built index over " << docs.size() << " documents.\n";

    // Test queries for benchmarking
    std::vector<std::string> test_queries = {
        "search", "C++", "document", "engine", "test", "index"
    };

    std::cout << "\nChoose mode:\n";
    std::cout << "1. Interactive search (default)\n";
    std::cout << "2. Run benchmark comparison\n";
    std::cout << "Enter choice (1 or 2): ";
    
    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "2") {
        // Run comprehensive benchmark
        run_comprehensive_benchmark(docs, index, test_queries);
    } else {
        // Interactive search mode
        std::cout << "\n=== INTERACTIVE SEARCH MODE ===\n";
        std::cout << "Choose algorithm:\n";
        std::cout << "1. TF-IDF (default)\n";
        std::cout << "2. BM25\n";
        std::cout << "Enter choice (1 or 2): ";
        
        std::string algo_choice;
        std::getline(std::cin, algo_choice);
        SearchAlgorithm selected_algo = (algo_choice == "2") ? SearchAlgorithm::BM25 : SearchAlgorithm::TFIDF;
        
        std::string algorithm_name = (selected_algo == SearchAlgorithm::TFIDF) ? "TF-IDF" : "BM25";
        std::cout << "Using " << algorithm_name << " algorithm\n";

        std::string query;
        while (true) {
            std::cout << "\nEnter a single-term query (or 'quit'): ";
            if (!std::getline(std::cin, query)) break;
            if (query == "quit" || query == "exit") break;

            std::vector<std::pair<DocId, double>> results;
            
            switch (selected_algo) {
                case SearchAlgorithm::TFIDF:
                    results = search_tfidf(query, docs, index);
                    break;
                case SearchAlgorithm::BM25:
                    results = search_bm25(query, docs, index);
                    break;
            }
            
            if (results.empty()) {
                std::cout << "No documents found for term: " << query << "\n";
                continue;
            }

            std::cout << "Found " << results.size() << " matching docs. Top results:" << std::endl;
            int printed = 0;
            for (const auto &r : results) {
                DocId docId = r.first;
                double score = r.second;
                std::cout << "docId=" << docId << ", score=" << score
                          << "\n  " << docs[docId] << "\n";
                if (++printed >= 5) break;  // top 5
            }
        }
    }

    return 0;
}