//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// TestAdditional.c - Additional tests for Graph Dominating Sets
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphDominatingSets.h"
#include "IndicesSet.h"
#include "instrumentation.h"

void TestAdjacents(Graph* g) {
  printf("\n--- TestAdjacents ---\n");
  int numVertices = GraphGetNumVertices(g);
  for (int i = 0; i < numVertices; i++) {
    IndicesSet* adj = GraphGetSetAdjacentsTo(g, i);
    printf("Vertex %d adjacents: ", i);
    IndicesSetDisplay(adj);
    IndicesSetDestroy(&adj);
  }
}

void TestVertexWeights(Graph* g) {
  printf("\n--- TestVertexWeights ---\n");
  double* weights = GraphComputeVertexWeights(g);
  int range = GraphGetVertexRange(g);
  for (int i = 0; i < range; i++) {
    if (weights[i] != -1.0) {
      printf("Vertex %d weight: %.2f\n", i, weights[i]);
    }
  }
  free(weights);
}

void TestSubgraph(Graph* g) {
  printf("\n--- TestSubgraph ---\n");
  IndicesSet* subset = IndicesSetCreateEmpty(GraphGetVertexRange(g));
  // Select vertices 0, 1, 2 (triangle part of the square)
  IndicesSetAdd(subset, 0);
  IndicesSetAdd(subset, 1);
  IndicesSetAdd(subset, 2);
  
  printf("Creating subgraph with vertices: ");
  IndicesSetDisplay(subset);
  
  Graph* sub = GraphGetSubgraph(g, subset);
  
  printf("Subgraph properties:\n");
  printf("Num Vertices: %d\n", GraphGetNumVertices(sub));
  printf("Num Edges: %d\n", GraphGetNumEdges(sub));
  
  TestAdjacents(sub);
  
  GraphDestroy(&sub);
  IndicesSetDestroy(&subset);
}

void TestDominatingSets(Graph* g) {
  printf("\n--- TestDominatingSets ---\n");
  
  // Test Min Dominating Set
  printf("Computing Min Dominating Set...\n");
  IndicesSet* minDS = GraphComputeMinDominatingSet(g);
  printf("Min Dominating Set: ");
  IndicesSetDisplay(minDS);
  printf("Is Dominating? %d\n", GraphIsDominatingSet(g, minDS));
  IndicesSetDestroy(&minDS);
  
  // Test Min Weight Dominating Set
  printf("Computing Min Weight Dominating Set...\n");
  IndicesSet* minWeightDS = GraphComputeMinWeightDominatingSet(g);
  printf("Min Weight Dominating Set: ");
  IndicesSetDisplay(minWeightDS);
  printf("Is Dominating? %d\n", GraphIsDominatingSet(g, minWeightDS));
  IndicesSetDestroy(&minWeightDS);
}

int main(void) {
  printf("=== Additional Tests ===\n");
  
  // Create a graph with 2 components
  // Component 1: 0-1, 1-2, 2-3, 3-0, 0-2 (Square with diagonal)
  // Component 2: 4-5, 5-6, 6-4 (Triangle)
  
  printf("\nCreating Unweighted Graph...\n");
  Graph* g = GraphCreate(7, 0, 0); // 7 vertices, undirected, unweighted
  GraphAddEdge(g, 0, 1);
  GraphAddEdge(g, 1, 2);
  GraphAddEdge(g, 2, 3);
  GraphAddEdge(g, 3, 0);
  GraphAddEdge(g, 0, 2);
  
  GraphAddEdge(g, 4, 5);
  GraphAddEdge(g, 5, 6);
  GraphAddEdge(g, 6, 4);
  
  TestAdjacents(g);
  TestVertexWeights(g); // Should be degrees
  TestSubgraph(g);
  TestDominatingSets(g);
  
  GraphDestroy(&g);
  
  printf("\nCreating Weighted Graph...\n");
  Graph* gw = GraphCreate(7, 0, 1); // 7 vertices, undirected, weighted
  // Weights will be edge weights.
  // Make vertex 0 very expensive by giving high weights to its edges
  GraphAddWeightedEdge(gw, 0, 1, 10.0);
  GraphAddWeightedEdge(gw, 1, 2, 1.0);
  GraphAddWeightedEdge(gw, 2, 3, 1.0);
  GraphAddWeightedEdge(gw, 3, 0, 10.0);
  GraphAddWeightedEdge(gw, 0, 2, 10.0);
  
  // Triangle with small weights
  GraphAddWeightedEdge(gw, 4, 5, 2.0);
  GraphAddWeightedEdge(gw, 5, 6, 2.0);
  GraphAddWeightedEdge(gw, 6, 4, 2.0);
  
  TestVertexWeights(gw);
  TestDominatingSets(gw);
  
  GraphDestroy(&gw);
  
  printf("\n=== All Tests Completed ===\n");
  return 0;
}
