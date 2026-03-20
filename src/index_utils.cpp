#include "inverted_index.h"
#include <cctype>
#include <algorithm>

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