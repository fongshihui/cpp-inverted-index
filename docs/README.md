# C++ Inverted Index with TF-IDF and BM25 Search

A comprehensive implementation of an inverted index system with multiple search algorithms (TF-IDF and BM25) written in modern C++. This project demonstrates information retrieval fundamentals with professional-grade features including benchmarking, performance comparison, and extensible architecture.

## 🚀 Features

### Core Functionality
- **Inverted Index Construction**: Efficient index building from document collections
- **Multiple Search Algorithms**: TF-IDF and BM25 with configurable parameters
- **Advanced Tokenization**: Case-insensitive processing with proper text normalization
- **Interactive CLI**: User-friendly command-line interface for real-time searching
- **Ranked Results**: Relevance-based scoring and sorting

### Advanced Capabilities
- **Performance Benchmarking**: Comprehensive timing and comparison framework
- **Algorithm Comparison**: Side-by-side TF-IDF vs BM25 performance analysis
- **CSV Export**: Detailed results export for further analysis
- **Visualization Ready**: Python scripts for generating performance charts
- **Modular Architecture**: Separated components for maintainability and extensibility

## 📊 How It Works

### Inverted Index Structure

The inverted index uses efficient C++ standard library containers:
```cpp
using DocId = int;  // Document identifier
using Term = std::string;  // Normalized term
using PostingList = std::unordered_map<DocId, int>;  // docId -> term frequency
using InvertedIndex = std::unordered_map<Term, PostingList>;  // term -> posting list
```

### Search Algorithms

#### TF-IDF (Term Frequency-Inverse Document Frequency)
- **Term Frequency (TF)**: Raw count of term occurrences in a document
- **Inverse Document Frequency (IDF)**: `log(N / (1 + df))` where:
  - `N` = total number of documents
  - `df` = number of documents containing the term
- **Score**: `tf * idf` - balances term frequency with collection rarity

#### BM25 (Best Matching 25)
- **Advanced ranking** function that improves upon TF-IDF
- **Document length normalization**: Accounts for varying document sizes
- **Configurable parameters**: `k1` (term frequency saturation), `b` (length normalization)
- **Better handling** of common terms and document length variations

### Tokenization Process

1. **Case normalization**: All text converted to lowercase
2. **Term extraction**: Split on non-alphanumeric characters
3. **Whitespace handling**: Proper handling of consecutive separators
4. **Unicode support**: Basic character classification support

## 🛠️ Project Structure

```
cpp-inverted-index/
├── src/                    # C++ source code
│   ├── main.cpp           # Main program entry point
│   ├── index_utils.cpp    # Tokenization and index building
│   ├── search_algorithms.cpp # TF-IDF and BM25 implementations
│   └── benchmark.cpp      # Benchmarking implementation
├── include/               # C++ header files
│   ├── inverted_index.h   # Main header with type definitions
│   └── benchmark.h       # Benchmarking interface
├── web/                   # Web interface
│   ├── web_interface.py  # Flask web server for visualization
│   └── templates/
│       └── index.html    # Web interface template
├── scripts/               # Build and utility scripts
│   ├── build.sh          # Automated build script
│   ├── run.sh            # Multi-mode execution script
│   └── generate_charts.py # Performance visualization script
├── data/                  # Data files
│   └── benchmark_results.csv # Benchmark results
├── docs/                  # Documentation
│   └── README.md         # This documentation
└── .gitignore            # Git ignore rules
```

## 📦 Installation & Usage

### Build Automation

Use the provided build script for easy compilation:

```bash
# Make the build script executable
chmod +x build.sh

# Run the build script
./build.sh
```

The build script automatically:
- Compiles all C++ source files with proper flags
- Links all object files into the final executable
- Handles both development and production builds
- Provides clear error messages and success indicators

### Manual Compilation (Alternative)

```bash
# Single command compilation
g++ -std=c++11 -o inverted_index main.cpp index_utils.cpp search_algorithms.cpp benchmark.cpp

# Or compile separately (better for development)
g++ -std=c++11 -c index_utils.cpp
g++ -std=c++11 -c search_algorithms.cpp  
g++ -std=c++11 -c benchmark.cpp
g++ -std=c++11 -c main.cpp
g++ -std=c++11 -o inverted_index main.o index_utils.o search_algorithms.o benchmark.o
```

### Running the Application

You have multiple options:

#### 1. Using the run script (recommended)
```bash
# Make the run script executable
chmod +x run.sh

# Run with default interactive mode
./run.sh

# Run benchmark mode
./run.sh benchmark

# Run with specific algorithm
./run.sh tfidf
./run.sh bm25
```

#### 2. Direct execution
```bash
./inverted_index
```

You'll be presented with two modes:

#### 1. Interactive Search Mode
- Choose between TF-IDF and BM25 algorithms
- Enter search queries interactively
- See real-time results with relevance scores

#### 2. Benchmark Mode
- Automated performance testing across multiple queries
- Comparative analysis of both algorithms
- CSV export for detailed analysis

### Web Interface Visualization

Start the web interface for interactive search and visualization:

```bash
# Install Python dependencies (if not already installed)
pip install flask

# Start the web server
python3 web_interface.py
```

Then open your browser to: `http://localhost:5000`

**Web Interface Features:**
- Real-time search with both TF-IDF and BM25 algorithms
- Interactive search results with relevance scores
- Performance benchmarking with visual charts
- Live execution time measurements
- Responsive design for desktop and mobile

### Performance Visualization (CLI)

After running benchmarks:
```bash
python3 generate_charts.py
```

This generates comprehensive charts in the `charts/` directory:
- Execution time comparisons
- Score distribution analysis
- Performance ratio visualizations
- Result count comparisons

## 🔍 Example Usage

### Sample Session

```
Built index over 4 documents.

Choose mode:
1. Interactive search (default)
2. Run benchmark comparison
Enter choice (1 or 2): 2

=== RUNNING COMPREHENSIVE BENCHMARK ===
Testing query: 'search'
  TF-IDF: 0.045ms, 3 results, top score: 0.693147
  BM25: 0.067ms, 3 results, top score: 1.232123

Testing query: 'C++'
  TF-IDF: 0.032ms, 2 results, top score: 0.693147  
  BM25: 0.051ms, 2 results, top score: 1.456789

=== BENCHMARK SUMMARY ===
Average TF-IDF time: 0.038ms
Average BM25 time: 0.059ms
Performance ratio: 1.55x
Results saved to benchmark_results.csv
```

### Sample Documents

The system includes these test documents:
1. "Hello world, this is a test document."
2. "Another test document about C++ and search."
3. "Search engines build inverted indexes from documents."
4. "C++ can be used to implement simple search engines."

## 🎯 Technical Details

### Key Functions

- **`tokenize(const std::string &text)`**: Text normalization and token extraction
- **`build_index(const std::vector<std::string> &docs)`**: Index construction from documents
- **`search_tfidf(query, docs, index)`**: Traditional TF-IDF ranking
- **`search_bm25(query, docs, index, k1=1.5, b=0.75)`**: BM25 with configurable parameters
- **`run_benchmark(algorithm, query, docs, index)`**: Performance measurement
- **`run_comprehensive_benchmark(docs, index, queries)`**: Full comparative analysis

### Algorithm Parameters

#### BM25 Configuration
- **`k1`**: Term frequency saturation parameter (default: 1.5)
- **`b`**: Document length normalization parameter (default: 0.75)
- **Typical values**: k1 ∈ [1.2, 2.0], b ∈ [0.5, 0.8]

#### TF-IDF Variants
- **Basic implementation**: Standard log-based IDF with raw TF
- **Smoothing**: Add-one smoothing to handle zero probabilities

## 📈 Performance Characteristics

### Time Complexity
- **Index building**: O(N × M) where N = documents, M = average terms per document
- **Term lookup**: O(1) average (hash map)
- **Search execution**: O(df) where df = document frequency of term

### Memory Usage
- **Index storage**: O(V × df_avg) where V = vocabulary size
- **Posting lists**: Compact storage of document IDs and frequencies
- **Tokenization**: Streaming processing with minimal memory overhead

## 🔧 Extending the Project

### Adding New Algorithms

1. Implement new search function following existing interface
2. Add to `SearchAlgorithm` enum
3. Update benchmarking and main application logic

### Performance Optimizations

- **Memory-mapped files**: For larger-than-memory indices
- **Compressed posting lists**: Delta encoding and variable-byte compression
- **Concurrent processing**: Multi-threaded index building and searching
- **GPU acceleration**: CUDA implementation for scoring functions

### Feature Enhancements

- **Multi-term queries**: Boolean operators and phrase search
- **Relevance feedback**: Query expansion based on user interactions
- **Stemming and lemmatization**: Improved term normalization
- **Stop word filtering**: Removal of common low-value terms
- **Spelling correction**: Fuzzy matching and did-you-mean suggestions

## 📚 Dependencies

### Required
- **C++11** or later compatible compiler
- **Standard Library**: Comprehensive C++ STL support

### Optional (for visualization)
- **Python 3.6+**: For performance chart generation
- **Matplotlib**: Chart rendering library
- **Pandas**: Data analysis and CSV processing

### Header Includes
```cpp
#include <iostream>    // Input/output operations
#include <string>       // String manipulation
#include <vector>       // Dynamic arrays
#include <unordered_map> // Hash-based dictionaries
#include <cctype>       // Character classification
#include <algorithm>    // Algorithm utilities
#include <cmath>        // Mathematical functions
#include <chrono>       // Timing and benchmarking
#include <iomanip>      // Output formatting
#include <fstream>      // File operations
```

## 🎓 Educational Value

This project demonstrates:

### Computer Science Concepts
- Information retrieval fundamentals
- Search algorithm design and implementation
- Data structure selection and optimization
- Algorithm complexity analysis
- Performance measurement and benchmarking

### Software Engineering Practices
- Modular code organization
- Header file design
- API design and interface consistency
- Documentation standards
- Build system management

### Real-World Applications
- Search engine technology
- Document retrieval systems
- Text analysis and processing
- Relevance ranking systems
- Performance optimization techniques

## 🤝 Contributing

This project welcomes enhancements in these areas:

### Algorithm Improvements
- Additional ranking algorithms (BM25F, DFR, language models)
- Query expansion techniques
- Relevance feedback mechanisms

### Performance Enhancements
- Memory efficiency optimizations
- Concurrent processing support
- Disk-based indexing for large datasets

### Feature Additions
- Query language support
- Result snippet generation
- Web interface and REST API
- Persistent storage and index serialization

## 📄 License

This educational implementation is open for learning and modification. Please attribute appropriately if used in academic or research contexts.

---

**Note**: This project is designed for educational purposes and demonstrates core information retrieval concepts. For production use, consider established libraries like Lucene, Xapian, or dedicated search engines.