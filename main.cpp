#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <algorithm>
#include <cmath>

using DocId = int;
using Term = std::string;

// Inverted index: term -> (docId -> term frequency in that doc)
using PostingList = std::unordered_map<DocId, int>;
using InvertedIndex = std::unordered_map<Term, PostingList>;

// Simple tokenizer: lowercase, split on non-alphanumeric characters
std::vector<Term> tokenize(const std::string &text) {
    std::vector<Term> tokens;
    Term current;
    for (char ch : text) {
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            current.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
        } else {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        }
    }
    if (!current.empty()) {
        tokens.push_back(current);
    }
    return tokens;
}

// Build inverted index from documents
InvertedIndex build_index(const std::vector<std::string> &docs) {
    InvertedIndex index;
    for (DocId docId = 0; docId < (DocId)docs.size(); ++docId) {
        auto tokens = tokenize(docs[docId]);
        for (const auto &t : tokens) {
            index[t][docId]++;  // increment term frequency
        }
    }
    return index;
}

// Simple one-term TF-IDF search
std::vector<std::pair<DocId, double>> search_one_term(
        const std::string &queryTerm,
        const std::vector<std::string> &docs,
        const InvertedIndex &index) {

    std::vector<std::pair<DocId, double>> results;
    if (docs.empty()) return results;

    // normalize query term like tokenizer
    Term q;
    for (char ch : queryTerm) {
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            q.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
        }
    }
    if (q.empty()) return results;

    auto it = index.find(q);
    if (it == index.end()) {
        return results; // term not found
    }

    const PostingList &postings = it->second;
    double N = static_cast<double>(docs.size());
    double df = static_cast<double>(postings.size());
    double idf = std::log(N / (1.0 + df));

    for (const auto &p : postings) {
        DocId docId = p.first;
        int tf = p.second;
        double score = tf * idf;  // super simple TF-IDF
        results.emplace_back(docId, score);
    }

    std::sort(results.begin(), results.end(),
              [](const std::pair<DocId, double> &a, const std::pair<DocId, double> &b) { return a.second > b.second; });

    return results;
}

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

    // 3. Simple REPL loop
    std::string query;
    while (true) {
        std::cout << "\nEnter a single-term query (or 'quit'): ";
        if (!std::getline(std::cin, query)) break;
        if (query == "quit" || query == "exit") break;

        auto results = search_one_term(query, docs, index);
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

    return 0;
}