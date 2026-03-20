#!/usr/bin/env python3
"""
Performance comparison chart generator for inverted index benchmarks
This script creates visualizations from benchmark_results.csv
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os


def generate_performance_charts():
    # Check if CSV file exists
    csv_path = "../data/benchmark_results.csv"
    if not os.path.exists(csv_path):
        print("Error: benchmark_results.csv not found. Run the benchmark first.")
        return

    # Read the CSV data
    df = pd.read_csv(csv_path)

    # Create output directory for charts
    os.makedirs("../charts", exist_ok=True)

    # 1. Bar chart: Average execution time by algorithm
    plt.figure(figsize=(10, 6))
    avg_times = df.groupby("Algorithm")["Time(ms)"].mean()
    colors = ["skyblue", "lightcoral"]
    bars = plt.bar(avg_times.index, avg_times.values, color=colors, alpha=0.8)

    plt.title("Average Execution Time by Search Algorithm")
    plt.ylabel("Time (milliseconds)")
    plt.xlabel("Algorithm")

    # Add value labels on bars
    for bar in bars:
        height = bar.get_height()
        plt.text(
            bar.get_x() + bar.get_width() / 2.0,
            height,
            f"{height:.3f}ms",
            ha="center",
            va="bottom",
        )

    plt.tight_layout()
    plt.savefig("../charts/avg_execution_time.png", dpi=300, bbox_inches="tight")
    plt.close()

    # 2. Performance ratio chart
    plt.figure(figsize=(10, 6))
    performance_ratio = avg_times["TF-IDF"] / avg_times["BM25"]
    plt.bar(["TF-IDF / BM25"], [performance_ratio], color="lightgreen", alpha=0.8)
    plt.title("Performance Ratio (TF-IDF / BM25)")
    plt.ylabel("Performance Ratio")
    plt.axhline(y=1, color="red", linestyle="--", alpha=0.7)
    plt.text(0, 1.05, "Equal Performance", color="red", ha="center")
    plt.grid(axis="y", alpha=0.3)
    plt.savefig("../charts/performance_ratio.png", bbox_inches="tight", dpi=300)
    plt.close()

    # 3. Score distribution comparison
    plt.figure(figsize=(12, 6))
    tfidf_scores = df[df["Algorithm"] == "TF-IDF"]["TopScore"]
    bm25_scores = df[df["Algorithm"] == "BM25"]["TopScore"]

    plt.hist(tfidf_scores, bins=20, alpha=0.7, label="TF-IDF", color="skyblue")
    plt.hist(bm25_scores, bins=20, alpha=0.7, label="BM25", color="lightcoral")
    plt.title("Score Distribution Comparison")
    plt.xlabel("Top Score")
    plt.ylabel("Frequency")
    plt.legend()
    plt.grid(alpha=0.3)
    plt.savefig("../charts/score_distribution.png", bbox_inches="tight", dpi=300)
    plt.close()

    # 4. Results count comparison
    plt.figure(figsize=(10, 6))
    result_counts = df.groupby("Algorithm")["Results"].mean()
    colors = ["lightblue", "lightpink"]
    bars = plt.bar(result_counts.index, result_counts.values, color=colors, alpha=0.8)
    plt.title("Average Number of Results by Algorithm")
    plt.ylabel("Average Results")
    plt.xlabel("Algorithm")
    for bar in bars:
        height = bar.get_height()
        plt.text(
            bar.get_x() + bar.get_width() / 2.0,
            height,
            f"{height:.1f}",
            ha="center",
            va="bottom",
        )
    plt.grid(axis="y", alpha=0.3)
    plt.savefig("../charts/result_count_comparison.png", bbox_inches="tight", dpi=300)
    plt.close()

    print("Generated 5 performance comparison charts in 'charts/' directory:")
    print("1. avg_execution_time.png - Average execution times")
    print("2. execution_time_by_query.png - Detailed query performance")
    print("3. score_comparison.png - Score differences between algorithms")
    print("4. results_count_comparison.png - Result count comparison")
    print("5. performance_ratio.png - Performance ratio analysis")

    # Print summary statistics
    print("\n=== SUMMARY STATISTICS ===")
    print(f"Average TF-IDF time: {avg_times['TF-IDF']:.3f}ms")
    print(f"Average BM25 time: {avg_times['BM25']:.3f}ms")
    print(f"Overall performance ratio: {avg_times['BM25']/avg_times['TF-IDF']:.2f}x")


if __name__ == "__main__":
    generate_performance_charts()
