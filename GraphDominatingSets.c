//
// Algoritmos e Estruturas de Dados --- 2025/2026
//
// Joaquim Madeira - Nov 2025
//
// GraphDominatingSets - Computing Vertex Dominating Sets for UNDIRECTED graphs
//

// Student Name : Inês Simões Batista
// Student Number : 124877
// Student Name : Maria Clara da Fonseca Silva Vaz Quinteiro
// Student Number : 124996

/*** COMPLETE THE GraphIsDominatingSet FUNCTION ***/
/*** COMPLETE THE GraphMinDominatingSet FUNCTION ***/
/*** COMPLETE THE GraphMinWeightDominatingSet FUNCTION ***/

#include "GraphDominatingSets.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IndicesSet.h"
#include "instrumentation.h"

//
// TO BE COMPLETED
//
// Check if the given set is a dominating set for the graph
// Return 1 if true, or 0 otherwise
//
// A dominating set is a set of graph vertices such that every other
// graph vertex not in the set is adjacent to a graph vertex in the set
//
int GraphIsDominatingSet(const Graph* g, IndicesSet* vertSet) {
  // Verificar que o ponteiro para o grafo não é NULL
  assert(g != NULL);
  // Verificar que o grafo é não-orientado já que se trata de uma função para grafos não-orientados
  assert(GraphIsDigraph(g) == 0);
  // Verificar que o conjunto de vertíces não está vazio.
  // // Um conjunto vazio pode ser dominante apenas se o grafo não tiver vértices
  assert(IndicesSetIsEmpty(vertSet) == 0);

  //
  // TO BE COMPLETED
  //

  // Obter todos os vértices do grafo
  IndicesSet* allVertices = GraphGetSetVertices(g);
  
  // Caso especial: conjunto vazio não pode ser dominante (assert já verifica)
  // O assert garante que vertSet não está vazio, mas mantemos esta verificação
  
  // Criar conjunto para armazenar vértices dominados. Inicialmente contém os próprios vértices do conjunto
  IndicesSet* dominated = IndicesSetCreateCopy(vertSet);
  
  // Obter lista de vértices do conjunto
  unsigned int* vertices = IndicesSetGetElems(vertSet);
  unsigned int numVertices = IndicesSetGetNumElems(vertSet);
  
  // Para cada vértice no conjunto
  for (unsigned int i = 0; i < numVertices; i++) {
    unsigned int v = vertices[i];
    
    // Obter todos os vértices adjacentes a v
    IndicesSet* adjToV = GraphGetSetAdjacentsTo(g, v);
    
    // Adicionar vértices adjacentes ao conjunto de dominados
    IndicesSetUnion(dominated, adjToV);
    
    // Libertar conjunto de adjacentes
    IndicesSetDestroy(&adjToV);
  }
  
  // Libertar array de vértices
  free(vertices);

  // Verificar se todos os vértices do grafo estão dominados
  int isDominating = IndicesSetIsSubset(allVertices, dominated);
  
  // Libertar memória
  IndicesSetDestroy(&allVertices);
  IndicesSetDestroy(&dominated);
  
  return isDominating;
}

//
// TO BE COMPLETED
//
// Compute a MIN VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the/a dominating set
//
IndicesSet* GraphComputeMinDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  //
  // TO BE COMPLETED
  //

  // Change this according to your algorithm
  IndicesSet* result = IndicesSetCreateEmpty(GraphGetVertexRange(g));

  return result;
}

//
// TO BE COMPLETED
//
// Compute a MIN WEIGHT VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the dominating set
//
IndicesSet* GraphComputeMinWeightDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  //
  // TO BE COMPLETED
  //

  // Change this according to your algorithm
  IndicesSet* result = IndicesSetCreateEmpty(GraphGetVertexRange(g));

  return result;
}
