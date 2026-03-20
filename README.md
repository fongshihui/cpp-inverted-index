# C++ Inverted Index with TF-IDF Search

A simple yet efficient implementation of an inverted index with TF-IDF (Term Frequency-Inverse Document Frequency) search functionality written in C++.

## Features

- **Inverted Index Construction**: Builds an inverted index from a collection of documents
- **TF-IDF Scoring**: Implements basic TF-IDF algorithm for relevance ranking
- **Simple Tokenization**: Basic tokenizer that converts to lowercase and splits on non-alphanumeric characters
- **Interactive Search**: Command-line interface for searching documents
- **Ranked Results**: Returns search results sorted by relevance score

## How It Works

### Inverted Index Structure

The inverted index is implemented as:
```cpp
using PostingList = std::unordered_map<DocId, int>;  // docId -> term frequency
using InvertedIndex = std::unordered_map<Term, PostingList>;  // term -> posting list
```

### TF-IDF Algorithm

The search function calculates relevance scores using:
- **Term Frequency (TF)**: Count of how many times a term appears in a document
- **Inverse Document Frequency (IDF)**: `log(N / (1 + df))` where:
  - `N` = total number of documents
  - `df` = number of documents containing the term
- **TF-IDF Score**: `tf * idf` - higher scores indicate more relevant documents

## Usage

### Building and Running

```bash
# Compile the program
g++ -std=c++11 -o inverted_index main.cpp

# Run the program
./inverted_index
```

### Example Session

```
Built index over 4 documents.

Enter a single-term query (or 'quit'): search
Found 2 matching docs. Top results:
docId=2, score=0.693147
  Search engines build inverted indexes from documents.
docId=1, score=0.346574
  Another test document about C++ and search.

Enter a single-term query (or 'quit'): C++
Found 2 matching docs. Top results:
docId=3, score=0.693147
  C++ can be used to implement simple search engines.
docId=1, score=0.346574
  Another test document about C++ and search.

Enter a single-term query (or 'quit'): quit
```

## Example Documents

The program includes these sample documents:
1. "Hello world, this is a test document."
2. "Another test document about C++ and search."
3. "Search engines build inverted indexes from documents."
4. "C++ can be used to implement simple search engines."

## Implementation Details

### Key Functions

- `tokenize(const std::string &text)`: Converts text to lowercase tokens
- `build_index(const std::vector<std::string> &docs)`: Builds inverted index from documents
- `search_one_term(const std::string &queryTerm, ...)`: Performs TF-IDF search

### Tokenization

The tokenizer:
- Converts all characters to lowercase
- Splits on non-alphanumeric characters
- Handles consecutive non-alphanumeric characters correctly

### Search Features

- Returns top 5 results by default
- Handles terms not found in the index
- Provides relevance scores for ranking
- Case-insensitive search

## Extending the Project

### Potential Enhancements

1. **Multi-term queries**: Support for AND/OR operations between terms
2. **File loading**: Read documents from files instead of hardcoded strings
3. **Advanced tokenization**: Support for stemming, stop words, etc.
4. **BM25 scoring**: More advanced ranking algorithm
5. **Persistence**: Save/load index to/from disk
6. **Web interface**: Add HTTP API for remote searching

### Performance Considerations

- Current implementation uses `std::unordered_map` for O(1) average lookup
- For larger datasets, consider memory-mapped files or specialized data structures
- Tokenization could be optimized for better performance

## Dependencies

- C++11 or later
- Standard Library headers: `<iostream>`, `<string>`, `<vector>`, `<unordered_map>`, `<cctype>`, `<algorithm>`, `<cmath>`

## License

This is a simple educational implementation. Feel free to use and modify for learning purposes.