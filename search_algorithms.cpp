#include "inverted_index.h"
#include <cmath>
#include <algorithm>

// Simple one-term TF-IDF search
std::vector<std::pair<DocId, double>> search_tfidf(
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
        double score = tf * idf;  // TF-IDF
        results.emplace_back(docId, score);
    }

    std::sort(results.begin(), results.end(),
              [](const std::pair<DocId, double> &a, const std::pair<DocId, double> &b) { return a.second > b.second; });

    return results;
}

// BM25 search algorithm (more advanced than TF-IDF)
std::vector<std::pair<DocId, double>> search_bm25(
    const std::string &queryTerm,
    const std::vector<std::string> &docs,
    const InvertedIndex &index,
    double k1, double b) {

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
    
    // Calculate average document length
    double avg_doc_length = 0.0;
    for (const auto &doc : docs) {
        avg_doc_length += tokenize(doc).size();
    }
    avg_doc_length /= N;

    // BM25 IDF component
    double idf = std::log((N - df + 0.5) / (df + 0.5) + 1.0);

    for (const auto &p : postings) {
        DocId docId = p.first;
        int tf = p.second;
        
        // Document length
        double doc_length = tokenize(docs[docId]).size();
        
        // BM25 scoring formula
        double numerator = tf * (k1 + 1);
        double denominator = tf + k1 * (1 - b + b * (doc_length / avg_doc_length));
        double score = idf * (numerator / denominator);
        
        results.emplace_back(docId, score);
    }

    std::sort(results.begin(), results.end(),
              [](const std::pair<DocId, double> &a, const std::pair<DocId, double> &b) { return a.second > b.second; });

    return results;
}