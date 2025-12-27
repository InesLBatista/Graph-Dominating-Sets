//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// TestBenchmark.c - Performance analysis for Graph Dominating Sets
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "Graph.h"
#include "GraphDominatingSets.h"
#include "IndicesSet.h"
#include "instrumentation.h"

// Helper to measure time in seconds
double get_time_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// Generate a random graph with V vertices and E edges
Graph* GenerateRandomGraph(int V, int E, int weighted) {
    Graph* g = GraphCreate(V, 0, weighted); // Undirected
    int edges_added = 0;
    while (edges_added < E) {
        int u = rand() % V;
        int v = rand() % V;
        if (u != v) {
            if (weighted) {
                double w = (rand() % 100) + 1.0; // Weights 1.0 to 100.0
                if (GraphAddWeightedEdge(g, u, v, w)) edges_added++;
            } else {
                if (GraphAddEdge(g, u, v)) edges_added++;
            }
        }
    }
    return g;
}

void RunBenchmark(int V, int density_percent) {
    int max_edges = V * (V - 1) / 2;
    int E = (max_edges * density_percent) / 100;
    
    printf("V=%d, E=%d (Density %d%%)\n", V, E, density_percent);
    
    // 1. Min Dominating Set (Unweighted)
    Graph* g = GenerateRandomGraph(V, E, 0);
    
    InstrReset();
    InstrCount[0] = 0;
    double start = get_time_seconds();
    IndicesSet* res = GraphComputeMinDominatingSet(g);
    double end = get_time_seconds();
    
    printf("  MinDominatingSet: Time=%.6fs, Ops=%lu, SetSize=%d\n", 
           end - start, InstrCount[0], IndicesSetGetNumElems(res));
    
    IndicesSetDestroy(&res);
    GraphDestroy(&g);
    
    // 2. Min Weight Dominating Set (Weighted)
    Graph* gw = GenerateRandomGraph(V, E, 1);
    
    InstrReset();
    InstrCount[1] = 0;
    start = get_time_seconds();
    res = GraphComputeMinWeightDominatingSet(gw);
    end = get_time_seconds();
    
    printf("  MinWeightDominatingSet: Time=%.6fs, Ops=%lu, SetSize=%d\n", 
           end - start, InstrCount[1], IndicesSetGetNumElems(res));
    
    IndicesSetDestroy(&res);
    GraphDestroy(&gw);
}

int main(void) {
    srand(time(NULL));
    
    printf("=== Graph Dominating Sets Benchmark ===\n");
    printf("Measuring execution time and recursive calls (Ops).\n\n");
    
    // Test with varying V (Sparse Graph - 20% density)
    printf("--- Varying V (Density 20%%) ---\n");
    int sizes[] = {5, 10, 15, 20, 25, 27, 30};
    for (int i = 0; i < 7; i++) {
        RunBenchmark(sizes[i], 20);
    }
    
    // Test with varying Density (Fixed V=15)
    printf("\n--- Varying Density (V=15) ---\n");
    int densities[] = {10, 30, 50, 70, 90};
    for (int i = 0; i < 5; i++) {
        RunBenchmark(15, densities[i]);
    }
    
    return 0;
}
