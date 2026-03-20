#!/bin/bash

# Run script for C++ Inverted Index Project
# This script handles running the program with various options and configurations

# Set colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Project information
PROJECT_NAME="C++ Inverted Index"
TARGET="../inverted_index"

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to check if executable exists
check_executable() {
    if [[ ! -f "$TARGET" ]]; then
        print_warning "Executable '$TARGET' not found."
        echo "Please build the project first with: ./build.sh"
        return 1
    fi
    return 0
}

# Function to show help
show_help() {
    echo "Usage: ./run.sh [OPTIONS]"
    echo "Run script for $PROJECT_NAME"
    echo ""
    echo "Options:"
    echo "  interactive    Run in interactive mode (default)"
    echo "  benchmark      Run benchmark mode"
    echo "  tfidf          Run with TF-IDF algorithm"
    echo "  bm25           Run with BM25 algorithm"
    echo "  test           Run quick test with sample queries"
    echo "  charts         Generate performance charts from last benchmark"
    echo "  help           Show this help message"
    echo ""
    echo "Examples:"
    echo "  ./run.sh               # Interactive mode"
    echo "  ./run.sh benchmark     # Run benchmarks"
    echo "  ./run.sh tfidf         # Use TF-IDF algorithm"
    echo "  ./run.sh test          # Quick test"
}

# Function to run interactive mode
run_interactive() {
    print_status "Starting interactive mode..."
    echo ""
    ./"$TARGET"
}

# Function to run benchmark mode
run_benchmark() {
    print_status "Starting benchmark mode..."
    echo ""
    echo "2" | ./"$TARGET"  # Send '2' to select benchmark mode
}

# Function to run with specific algorithm
run_with_algorithm() {
    local algo="$1"
    print_status "Running with $algo algorithm..."
    echo ""
    
    # Run in interactive mode and automatically select algorithm
    echo -e "1\n$([[ "$algo" == "bm25" ]] && echo "2" || echo "1")" | ./"$TARGET"
}

# Function to run quick test
run_test() {
    print_status "Running quick test..."
    echo ""
    
    # Test a few queries automatically
    {
        echo "1"    # Interactive mode
        echo "1"    # TF-IDF algorithm
        echo "search"
        echo "C++"
        echo "document" 
        echo "quit"
    } | ./"$TARGET"
}

# Function to generate charts
generate_charts() {
    print_status "Generating performance charts..."
    
    if [[ ! -f "benchmark_results.csv" ]]; then
        print_error "No benchmark results found. Run benchmarks first with: ./run.sh benchmark"
        return 1
    fi
    
    if ! command -v python3 >/dev/null 2>&1; then
        print_error "Python 3 is required to generate charts. Please install Python 3."
        return 1
    fi
    
    if [[ ! -f "generate_charts.py" ]]; then
        print_error "Chart generation script 'generate_charts.py' not found."
        return 1
    fi
    
    python3 generate_charts.py
    
    if [[ $? -eq 0 ]]; then
        print_success "Charts generated in 'charts/' directory"
        
        if [[ -d "charts" ]]; then
            echo "Generated charts:"
            ls -la charts/
        fi
    else
        print_error "Failed to generate charts"
        return 1
    fi
}

# Function to clean up test files
cleanup() {
    # Remove any temporary files if needed
    true
}

# Main script logic
main() {
    local action="${1:-interactive}"
    
    # Check if executable exists
    if ! check_executable; then
        return 1
    fi
    
    # Handle different actions
    case "$action" in
        "interactive")
            run_interactive
            ;;
        "benchmark")
            run_benchmark
            ;;
        "tfidf")
            run_with_algorithm "tfidf"
            ;;
        "bm25")
            run_with_algorithm "bm25"
            ;;
        "test")
            run_test
            ;;
        "charts")
            generate_charts
            ;;
        "help" | "--help" | "-h")
            show_help
            ;;
        *)
            print_error "Unknown action: $action"
            show_help
            return 1
            ;;
    esac
    
    # Cleanup
    cleanup
}

# Trap signals for clean exit
trap 'cleanup; exit 1' INT TERM

# Run main function with all arguments
main "$@"