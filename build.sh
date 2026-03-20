#!/bin/bash

# Build script for C++ Inverted Index Project
# This script compiles the project with proper flags and handles dependencies

# Set colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Project information
PROJECT_NAME="C++ Inverted Index"
VERSION="1.0.0"
TARGET="inverted_index"

# Source files
SOURCES=("main.cpp" "index_utils.cpp" "search_algorithms.cpp" "benchmark.cpp")

# Compiler settings
CXX="g++"
CXXFLAGS="-std=c++11 -Wall -Wextra -O2"
LDFLAGS=""

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

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to clean build artifacts
clean() {
    print_status "Cleaning build artifacts..."
    rm -f "$TARGET" *.o
    print_success "Clean complete"
}

# Function to show help
show_help() {
    echo "Usage: ./build.sh [OPTIONS]"
    echo "Build script for $PROJECT_NAME"
    echo ""
    echo "Options:"
    echo "  build       Build the project (default)"
    echo "  clean       Remove build artifacts"
    echo "  run         Build and run the program"
    echo "  debug       Build with debug symbols"
    echo "  release     Build with optimization"
    echo "  help        Show this help message"
    echo ""
    echo "Examples:"
    echo "  ./build.sh           # Build the project"
    echo "  ./build.sh run       # Build and run"
    echo "  ./build.sh clean     # Clean build files"
}

# Function to build the project
build_project() {
    local mode="$1"
    
    print_status "Building $PROJECT_NAME v$VERSION"
    
    # Check if compiler exists
    if ! command_exists "$CXX"; then
        print_error "Compiler $CXX not found. Please install g++"
        exit 1
    fi
    
    # Set compiler flags based on mode
    local extra_flags=""
    case "$mode" in
        "debug")
            extra_flags="-g -O0"
            print_status "Building in debug mode"
            ;;
        "release")
            extra_flags="-O3 -DNDEBUG"
            print_status "Building in release mode"
            ;;
        *)
            extra_flags="-O2"
            print_status "Building in default mode"
            ;;
    esac
    
    # Build command
    local cmd="$CXX $CXXFLAGS $extra_flags ${SOURCES[@]} -o $TARGET $LDFLAGS"
    
    print_status "Compiling: $cmd"
    
    # Execute build command
    if $cmd; then
        print_success "Build successful!"
        print_status "Executable created: $TARGET"
        
        # Show file information
        if command_exists "file"; then
            file "$TARGET"
        fi
        
        if command_exists "size"; then
            size "$TARGET"
        fi
        
        return 0
    else
        print_error "Build failed!"
        return 1
    fi
}

# Function to run the program
run_program() {
    if [[ ! -f "$TARGET" ]]; then
        print_warning "Executable not found. Building first..."
        build_project "$1"
        if [[ $? -ne 0 ]]; then
            return 1
        fi
    fi
    
    print_status "Running $TARGET..."
    echo ""
    
    # Run the program
    ./"$TARGET"
}

# Main script logic
main() {
    local action="${1:-build}"
    
    case "$action" in
        "build")
            build_project "default"
            ;;
        "clean")
            clean
            ;;
        "run")
            build_project "default" && run_program
            ;;
        "debug")
            build_project "debug"
            ;;
        "release")
            build_project "release"
            ;;
        "help" | "--help" | "-h")
            show_help
            ;;
        *)
            print_error "Unknown action: $action"
            show_help
            exit 1
            ;;
    esac
}

# Run main function with all arguments
main "$@"