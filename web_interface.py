#!/usr/bin/env python3
"""
Web Interface for C++ Inverted Index
Provides a simple web frontend to visualize search results and performance metrics
"""

from flask import Flask, render_template, request, jsonify
import subprocess
import os
import json
import csv
from datetime import datetime

app = Flask(__name__)

# Configuration
PROJECT_DIR = os.path.dirname(os.path.abspath(__file__))
EXECUTABLE = os.path.join(PROJECT_DIR, "inverted_index")
BENCHMARK_FILE = os.path.join(PROJECT_DIR, "benchmark_results.csv")


@app.route("/")
def index():
    """Main page with search interface"""
    return render_template("index.html")


@app.route("/search", methods=["POST"])
def search():
    """Perform a search using the C++ program"""
    query = request.json.get("query", "")
    algorithm = request.json.get("algorithm", "tfidf")

    if not query:
        return jsonify({"error": "Query is required"}), 400

    if not os.path.exists(EXECUTABLE):
        return jsonify({"error": "Program not compiled. Please build first."}), 500

    try:
        # Run the search
        result = run_search(query, algorithm)
        return jsonify(result)

    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route("/benchmark", methods=["POST"])
def run_benchmarks():
    """Run benchmarks and return results"""
    try:
        # Run benchmarks
        run_benchmark_mode()

        # Read and parse results
        results = read_benchmark_results()
        return jsonify(results)

    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route("/results")
def get_results():
    """Get latest benchmark results"""
    try:
        results = read_benchmark_results()
        return jsonify(results)
    except Exception as e:
        return jsonify({"error": str(e)}), 500


def run_search(query, algorithm):
    """Run search using the C++ program"""
    # Prepare input for the program
    input_data = f"1\n{'2' if algorithm == 'bm25' else '1'}\n{query}\nquit\n"

    # Run the program
    process = subprocess.Popen(
        [EXECUTABLE],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )

    stdout, stderr = process.communicate(input=input_data)

    if process.returncode != 0:
        raise Exception(f"Program failed: {stderr}")

    # Parse results from stdout
    results = parse_search_results(stdout)
    results["query"] = query
    results["algorithm"] = algorithm
    results["timestamp"] = datetime.now().isoformat()

    return results


def run_benchmark_mode():
    """Run the program in benchmark mode"""
    process = subprocess.Popen(
        [EXECUTABLE],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )

    stdout, stderr = process.communicate(input="2\n")

    if process.returncode != 0:
        raise Exception(f"Benchmark failed: {stderr}")


def read_benchmark_results():
    """Read and parse benchmark results from CSV"""
    if not os.path.exists(BENCHMARK_FILE):
        return {"error": "No benchmark results found"}

    results = {"queries": [], "algorithms": {}, "summary": {}}

    with open(BENCHMARK_FILE, "r") as f:
        reader = csv.DictReader(f)

        for row in reader:
            query = row["Query"]
            algorithm = row["Algorithm"]

            # Add unique queries
            if query not in results["queries"]:
                results["queries"].append(query)

            # Initialize algorithm data
            if algorithm not in results["algorithms"]:
                results["algorithms"][algorithm] = []

            # Add result
            results["algorithms"][algorithm].append(
                {
                    "query": query,
                    "time_ms": float(row["Time(ms)"]),
                    "results": int(row["Results"]),
                    "top_score": float(row["TopScore"]),
                }
            )

    # Calculate summary statistics
    for algorithm, data in results["algorithms"].items():
        times = [item["time_ms"] for item in data]
        scores = [item["top_score"] for item in data if item["top_score"] > 0]

        results["summary"][algorithm] = {
            "avg_time": sum(times) / len(times) if times else 0,
            "max_time": max(times) if times else 0,
            "min_time": min(times) if times else 0,
            "avg_score": sum(scores) / len(scores) if scores else 0,
            "total_queries": len(data),
        }

    return results


def parse_search_results(output):
    """Parse search results from program output"""
    lines = output.split("\n")
    results = {"documents": [], "total_results": 0, "execution_time": 0}

    in_results = False

    for line in lines:
        if "Found" in line and "matching docs" in line:
            # Extract total results count
            parts = line.split()
            if len(parts) >= 2:
                results["total_results"] = int(parts[1])

        elif "docId=" in line:
            # Parse document result
            parts = line.split(",")
            if len(parts) >= 2:
                doc_id = parts[0].split("=")[1]
                score = parts[1].split("=")[1]

                # Get content from next line
                content_line = (
                    lines[lines.index(line) + 1]
                    if lines.index(line) + 1 < len(lines)
                    else ""
                )

                results["documents"].append(
                    {
                        "doc_id": int(doc_id),
                        "score": float(score),
                        "content": content_line.strip(),
                    }
                )

    return results


@app.route("/health")
def health_check():
    """Health check endpoint"""
    return jsonify(
        {
            "status": "ok",
            "timestamp": datetime.now().isoformat(),
            "executable_exists": os.path.exists(EXECUTABLE),
        }
    )


if __name__ == "__main__":
    print("Starting web interface for C++ Inverted Index...")
    print(f"Project directory: {PROJECT_DIR}")
    print(f"Executable: {EXECUTABLE}")
    print("Web interface available at: http://localhost:5000")

    app.run(debug=True, host="0.0.0.0", port=5000)
