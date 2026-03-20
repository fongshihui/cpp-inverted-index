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
    if not os.path.exists('benchmark_results.csv'):
        print("Error: benchmark_results.csv not found. Run the benchmark first.")
        return
    
    # Read the CSV data
    df = pd.read_csv('benchmark_results.csv')
    
    # Create output directory for charts
    os.makedirs('charts', exist_ok=True)
    
    # 1. Bar chart: Average execution time by algorithm
    plt.figure(figsize=(10, 6))
    avg_times = df.groupby('Algorithm')['Time(ms)'].mean()
    colors = ['skyblue', 'lightcoral']
    bars = plt.bar(avg_times.index, avg_times.values, color=colors, alpha=0.8)
    
    plt.title('Average Execution Time by Search Algorithm')
    plt.ylabel('Time (milliseconds)')
    plt.xlabel('Algorithm')
    
    # Add value labels on bars
    for bar in bars:
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.3f}ms',
                ha='center', va='bottom')
    
    plt.tight_layout()
    plt.savefig('charts/avg_execution_time.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 2. Performance by query
    plt.figure(figsize=(12, 8))
    
    # Pivot data for easier plotting
    pivot_df = df.pivot(index='Query', columns='Algorithm', values='Time(ms)')
    
    x = np.arange(len(pivot_df.index))
    width = 0.35
    
    plt.bar(x - width/2, pivot_df['TF-IDF'], width, label='TF-IDF', alpha=0.8)
    plt.bar(x + width/2, pivot_df['BM25'], width, label='BM25', alpha=0.8)
    
    plt.xlabel('Query Terms')
    plt.ylabel('Execution Time (milliseconds)')
    plt.title('Execution Time by Query and Algorithm')
    plt.xticks(x, pivot_df.index, rotation=45, ha='right')
    plt.legend()
    plt.tight_layout()
    plt.savefig('charts/execution_time_by_query.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 3. Score comparison chart
    plt.figure(figsize=(12, 8))
    
    score_pivot = df.pivot(index='Query', columns='Algorithm', values='TopScore')
    
    x = np.arange(len(score_pivot.index))
    width = 0.35
    
    plt.bar(x - width/2, score_pivot['TF-IDF'], width, label='TF-IDF', alpha=0.8)
    plt.bar(x + width/2, score_pivot['BM25'], width, label='BM25', alpha=0.8)
    
    plt.xlabel('Query Terms')
    plt.ylabel('Top Score')
    plt.title('Top Result Scores by Query and Algorithm')
    plt.xticks(x, score_pivot.index, rotation=45, ha='right')
    plt.legend()
    plt.tight_layout()
    plt.savefig('charts/score_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 4. Results count comparison
    plt.figure(figsize=(12, 8))
    
    count_pivot = df.pivot(index='Query', columns='Algorithm', values='Results')
    
    x = np.arange(len(count_pivot.index))
    width = 0.35
    
    plt.bar(x - width/2, count_pivot['TF-IDF'], width, label='TF-IDF', alpha=0.8)
    plt.bar(x + width/2, count_pivot['BM25'], width, label='BM25', alpha=0.8)
    
    plt.xlabel('Query Terms')
    plt.ylabel('Number of Results')
    plt.title('Number of Results by Query and Algorithm')
    plt.xticks(x, count_pivot.index, rotation=45, ha='right')
    plt.legend()
    plt.tight_layout()
    plt.savefig('charts/results_count_comparison.png', dpi=300, bbox_inches='tight')
    plt.close()
    
    # 5. Performance ratio chart
    plt.figure(figsize=(10, 6))
    
    performance_ratio = pivot_df['BM25'] / pivot_df['TF-IDF']
    
    plt.bar(performance_ratio.index, performance_ratio.values, alpha=0.8, color='orange')
    plt.axhline(y=1.0, color='red', linestyle='--', alpha=0.7, label='Equal Performance')
    
    plt.xlabel('Query Terms')
    plt.ylabel('Performance Ratio (BM25/TF-IDF)')
    plt.title('Performance Ratio: BM25 vs TF-IDF (Lower is Better)')
    plt.xticks(rotation=45, ha='right')
    plt.legend()
    plt.tight_layout()
    plt.savefig('charts/performance_ratio.png', dpi=300, bbox_inches='tight')
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