# Graph Dominating Sets
**Date:** 2025-12-27<br>
<br>
This project focuses on implementing and extending the Graph Abstract Data Type (ADT) and developing algorithms to determine Vertex Dominating Sets in undirected graphs.
The work explores adjacency list-based graph representation, manipulation of index sets (IndicesSet), and exhaustive search algorithms for combinatorial optimization.

## Authors
- Inês Batista, 124877
- Maria Quinteiro, 124996

## Implemented Features

### 1. IndicesSet ADT
Auxiliary data structure for efficient manipulation of vertex index sets.
- Set operations: Union, Intersection, Difference.
- Iteration and subset verification.

### 2. Graph ADT (Extensions)
Advanced functionalities added to the base graph structure:
- `GraphGetSetAdjacentsTo`: Retrieval of the set of adjacent vertices.
- `GraphComputeVertexWeights`: Calculation of vertex weights (based on degrees or incident edge weights).
- `GraphGetSubgraph`: Construction of subgraphs induced by a set of vertices.

### 3. Dominating Sets Algorithms
`GraphDominatingSets` module for dominance analysis:
- Verification: `GraphIsDominatingSet` validates if a set dominates the graph.
- Optimization (Cardinality): `GraphComputeMinDominatingSet` finds the smallest dominating set (fewest vertices) using exhaustive search with pruning.
- Optimization (Weight): `GraphComputeMinWeightDominatingSet` finds the dominating set with the minimum total weight.

## Compilation and Execution

The project uses a `Makefile` for automation.

### Compile all
```bash
make
```

### Run Tests
The project includes several test programs to validate implementations:

```bash
./TestGraphBasics     # Tests basic graph operations
./TestIndicesSet      # Tests the IndicesSet ADT
./TestDominatingSets  # Tests dominating sets algorithms
./TestAdditional      # Additional tests (complex scenarios and extra requirements)
```

### Clean compiled files
```bash
make clean
```
