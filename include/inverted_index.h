#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <string>
#include <vector>
#include <unordered_map>

using DocId = int;
using Term = std::string;

// Inverted index: term -> (docId -> term frequency in that doc)
using PostingList = std::unordered_map<DocId, int>;
using InvertedIndex = std::unordered_map<Term, PostingList>;

// Tokenization functions
std::vector<Term> tokenize(const std::string &text);

// Index building functions
InvertedIndex build_index(const std::vector<std::string> &docs);

// Search algorithms
std::vector<std::pair<DocId, double>> search_tfidf(
    const std::string &queryTerm,
    const std::vector<std::string> &docs,
    const InvertedIndex &index);

std::vector<std::pair<DocId, double>> search_bm25(
    const std::string &queryTerm,
    const std::vector<std::string> &docs,
    const InvertedIndex &index,
    double k1 = 1.5, double b = 0.75);

// Search algorithm type
enum class SearchAlgorithm {
    TFIDF,
    BM25
};

#endif // INVERTED_INDEX_H