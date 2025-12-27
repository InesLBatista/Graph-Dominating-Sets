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
#include <float.h>  // Para DBL_MAX (valor máximo para doubles)

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
// Complexidade temporal: O(V + E) no pior caso, onde V é o número de vértices
// no conjunto vertSet e E é o número total de arestas no grafo.
// Cada vértice no conjunto requer a obtenção dos seus adjacentes.
//



// Função estática que devolve um array com os elementos de um IndicesSet
// Retorna um ponteiro para unsigned int ou NULL se o conjunto estiver vazio
static unsigned int* _getElems(IndicesSet* set) {

  // Obtém o número de elementos presentes no conjunto
  unsigned int num = IndicesSetGetNumElems(set);

  // Se não houver elementos, retorna NULL
  if (num == 0) return NULL;

  // Aloca memória para um array com 'num' elementos do tipo unsigned int
  unsigned int* elems = (unsigned int*)malloc(num * sizeof(unsigned int));

  // Garante que a alocação de memória foi bem-sucedida
  assert(elems != NULL);
  
  // Obtém o primeiro elemento do conjunto
  int v = IndicesSetGetFirstElem(set);

  // Índice usado para percorrer o array 'elems'
  unsigned int i = 0;

  // Percorre o conjunto enquanto houver elementos válidos e enquanto não ultrapassar o número esperado de elementos
  while (v != -1 && i < num) {

    // Guarda o elemento atual no array
    elems[i++] = (unsigned int)v;

    // Obtém o próximo elemento do conjunto
    v = IndicesSetGetNextElem(set);
  }

  // Retorna o array com os elementos do conjunto
  return elems;
}



int GraphIsDominatingSet(const Graph* g, IndicesSet* vertSet) {
  // Verificação de pré-condições:
  // 1. O ponteiro para o grafo deve ser válido (não NULL)
  assert(g != NULL);
  // 2. O grafo deve ser não-orientado, pois a definição de conjunto dominante aplica-se apenas a grafos não-orientados
  assert(GraphIsDigraph(g) == 0);
  // 3. O conjunto de vértices não deve estar vazio. Um conjunto vazio pode ser dominante apenas se o grafo não tiver vértices, mas esta restrição
  //    simplifica a implementação. Em compilações de produção (com NDEBUG definido), este assert é removido.
  assert(IndicesSetIsEmpty(vertSet) == 0);

  //
  // TO BE COMPLETED
  //

  // Primeiro, obtemos todos os vértices que existem no grafo.
  // Esta operação devolve uma cópia do conjunto de vértices do grafo.
  // É necessário porque vamos precisar de verificar se todos os vértices do grafo estão dominados.
  IndicesSet* allVertices = GraphGetSetVertices(g);
  
  // Criamos um novo conjunto para armazenar todos os vértices que estão dominados pelo conjunto vertSet. Inicializamos este conjunto com uma cópia de vertSet, pois cada vértice no conjunto dominante está automaticamente dominado.
  IndicesSet* dominated = IndicesSetCreateCopy(vertSet);
  
  // Para calcular eficientemente os vértices dominados, precisamos de iterar sobre todos os vértices em vertSet. Para isso, obtemos um array com os elementos do conjunto.
  unsigned int* vertices = _getElems(vertSet);
  unsigned int numVertices = IndicesSetGetNumElems(vertSet);
  
  // Para cada vértice no conjunto vertSet, vamos determinar quais são os vértices que ele domina. Um vértice v domina todos os seus vértices adjacentes.
  for (unsigned int i = 0; i < numVertices; i++) {
    unsigned int v = vertices[i];
    
    // Obter o conjunto de todos os vértices adjacentes a v.
    // A função GraphGetSetAdjacentsTo devolve um novo conjunto que contém os índices dos vértices que são vizinhos de v.
    IndicesSet* adjToV = GraphGetSetAdjacentsTo(g, v);
    
    // Adicionar todos os vértices adjacentes a v ao conjunto dominated.
    // A operação de união de conjuntos garante que não há duplicados.
    // Após esta operação, o conjunto dominated contém todos os vértices que são dominados por pelo menos um vértice em vertSet.
    IndicesSetUnion(dominated, adjToV);
    
    // Libertar a memória do conjunto de adjacentes, pois não é mais necessário. A função IndicesSetDestroy liberta todos os recursos associados ao conjunto, incluindo a estrutura interna.
    IndicesSetDestroy(&adjToV);
  }
  
  // Libertar o array de vértices obtido anteriormente.
  // É importante libertar esta memória para evitar memory leaks.
  free(vertices);

  // Agora verificamos se o conjunto dominated contém todos os vértices do grafo. Se dominated for um superconjunto de allVertices (ou seja, se todos os vértices do grafo estão em dominated), então vertSet é um conjunto dominante.
  // A função IndicesSetIsSubset(A, B) retorna 1 se todos os elementos de A estão em B, e 0 caso contrário.
  int isDominating = IndicesSetIsSubset(allVertices, dominated);
  
  // Libertar a memória dos conjuntos temporários criados durante a execução da função. Esta etapa é crucial para evitar memory leaks em programas de longa duração.
  IndicesSetDestroy(&allVertices);
  IndicesSetDestroy(&dominated);

  return isDominating;
}



// Estrutura de contexto para a procura exaustiva do conjunto dominante mínimo.
// Esta estrutura agrupa todos os dados necessários para a função recursiva de procura, evitando a necessidade de usar variáveis globais ou funções aninhadas (que não são suportadas no padrão C).
typedef struct {
    const Graph* graph;              // Ponteiro para o grafo original
    const unsigned int* vertices;    // Array com todos os vértices do grafo
    unsigned int numVertices;        // Número total de vértices no grafo
    IndicesSet* bestSet;             // Melhor conjunto dominante encontrado
    unsigned int bestSize;           // Tamanho do melhor conjunto encontrado
} SearchContext;



// Função auxiliar recursiva para procura exaustiva do conjunto dominante mínimo.
// Esta função implementa um algoritmo de backtracking que explora todos os subconjuntos possíveis de vértices, aplicando podas para melhorar eficiência.

// Parâmetros:
// - ctx: Ponteiro para a estrutura de contexto com dados partilhados
// - currentSet: Conjunto atual sendo explorado na recursão
// - startIdx: Índice no array de vértices a partir do qual começar a exploração

static void _searchMinDominatingSet(SearchContext* ctx, IndicesSet* currentSet, 
                                    unsigned int startIdx) {
    InstrCount[0]++; // Count recursive calls
    // Obter o tamanho atual do conjunto sendo explorado.
    unsigned int currentSize = IndicesSetGetNumElems(currentSet);
    
    // Poda por tamanho: se o conjunto atual já tem um tamanho igual ou maior que o melhor conjunto encontrado até agora, não vale a pena continuar a explorar este ramo da árvore de recursão, pois não poderá resultar num conjunto melhor (com menor cardinalidade).
    if (currentSize >= ctx->bestSize) {
        return;  // Podar este ramo da árvore de busca
    }
    
    // Verificar se o conjunto atual é um conjunto dominante válido.
    // Só verificamos se o conjunto não está vazio, pois um conjunto vazio nunca é dominante (a não ser que o grafo não tenha vértices).
    if (currentSize > 0 && GraphIsDominatingSet(ctx->graph, currentSet)) {
        // Encontramos um conjunto dominante válido que é melhor que o melhor anterior (devido a acima, sabemos que currentSize < bestSize).
        if (currentSize < ctx->bestSize) {
            // Atualizar o melhor tamanho encontrado
            ctx->bestSize = currentSize;
            
            // Limpar o conjunto bestSet anterior para receber o novo melhor
            IndicesSetDestroy(&(ctx->bestSet));
            ctx->bestSet = IndicesSetCreateEmpty(GraphGetVertexRange(ctx->graph));
            
            // Obter os elementos do conjunto atual
            unsigned int* elems = _getElems(currentSet);
            
            // Adicionar todos os elementos do conjunto atual ao bestSet
            for (unsigned int i = 0; i < currentSize; i++) {
                IndicesSetAdd(ctx->bestSet, elems[i]);
            }
            
            // Libertar o array de elementos
            free(elems);
        }
        return;  // Não é necessário continuar a expandir este conjunto pois já é dominante e qualquer expansão aumentaria o tamanho
    }
    
    // Continuar a exploração recursiva, adicionando vértices ao conjunto atual.
    // O parâmetro startIdx garante que geramos cada subconjunto apenas uma vez (evitando permutações do mesmo conjunto).
    for (unsigned int i = startIdx; i < ctx->numVertices; i++) {
        // Adicionar o vértice atual ao conjunto
        IndicesSetAdd(currentSet, ctx->vertices[i]);
        
        // Chamada recursiva para explorar subconjuntos que incluem este vértice.
        // Passamos i+1 como startIdx para garantir que cada vértice só é considerado uma vez (evita gerar o mesmo subconjunto em ordens diferentes).
        _searchMinDominatingSet(ctx, currentSet, i + 1);
        
        // Backtracking: remover o vértice adicionado para explorar outras possibilidades. Esta operação é essencial para garantir que todas as combinações são exploradas.
        IndicesSetRemove(currentSet, ctx->vertices[i]);
    }
}

//
// TO BE COMPLETED
//
// Compute a MIN VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the/a dominating set
//
// Esta função implementa uma procura exaustiva (força bruta) para encontrar
// o menor conjunto dominante de um grafo não-orientado. O algoritmo explora
// todos os subconjuntos possíveis de vértices, começando pelos menores,
// e aplica podas para melhorar a eficiência.
//
// Complexidade temporal: O(2^V * (V+E)) no pior caso, onde V é o número de
// vértices e E o número de arestas. A verificação de dominância para cada
// subconjunto é O(V+E), e há 2^V subconjuntos possíveis.
//
// Limitação prática: Apenas viável para grafos pequenos (V <= 20-25).
// Para grafos maiores, seriam necessárias heurísticas ou algoritmos aproximados.
//
IndicesSet* GraphComputeMinDominatingSet(const Graph* g) {
  // Verificação de pré-condições
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  //
  // TO BE COMPLETED
  //

  // Primeiro, obtemos todos os vértices existentes no grafo.
  // Isto é necessário porque o grafo pode não conter todos os vértices no intervalo [0, indicesRange-1] (pode ser um grafo esparso).
  IndicesSet* allVertices = GraphGetSetVertices(g);
  
  // Obter um array com os vértices e o seu número total.
  // Esta representação é mais eficiente para iteração do que trabalhar diretamente com a estrutura IndicesSet.
  unsigned int* vertices = _getElems(allVertices);
  unsigned int numVertices = IndicesSetGetNumElems(allVertices);
  
  // Criar o conjunto que irá armazenar o melhor resultado encontrado.
  // Inicialmente está vazio. A capacidade do conjunto é definida pelo range de vértices do grafo original.
  IndicesSet* bestSet = IndicesSetCreateEmpty(GraphGetVertexRange(g));
  
  // Inicializar o melhor tamanho com um valor impossível (maior que o máximo possível). Isto garante que o primeiro conjunto dominante encontrado será considerado como o melhor.
  unsigned int bestSize = numVertices + 1;
  
  // Preparar a estrutura de contexto que será passada para a função recursiva. Esta estrutura contém todos os dados partilhados que a função recursiva precisa aceder.
  SearchContext ctx;
  ctx.graph = g;
  ctx.vertices = vertices;
  ctx.numVertices = numVertices;
  ctx.bestSet = bestSet;
  ctx.bestSize = bestSize;
  
  // Criar um conjunto temporário que será usado durante a procura recursiva. Este conjunto representa o subconjunto atual sendo explorado na árvore de busca.
  IndicesSet* currentSet = IndicesSetCreateEmpty(GraphGetVertexRange(g));
  
  // Iniciar a procura exaustiva. A função começa com um conjunto vazio e startIdx = 0, o que significa que começará a considerar todos os vértices possíveis.
  _searchMinDominatingSet(&ctx, currentSet, 0);
  
  // Após a procura, atualizar a variável local bestSize com o valor encontrado pela função recursiva (que pode ter sido modificado no contexto).
  bestSize = ctx.bestSize;
  bestSet = ctx.bestSet;
  
  // Caso especial: se a procura não encontrou nenhum conjunto dominante (o que só acontece se numVertices == 0 ou se há um bug no algoritmo), usamos todos os vértices como fallback. Tecnicamente, o conjunto de todos os vértices é sempre um conjunto dominante válido.
  if (IndicesSetIsEmpty(bestSet) && numVertices > 0) {
    // Adicionar todos os vértices ao conjunto de fallback
    for (unsigned int i = 0; i < numVertices; i++) {
      IndicesSetAdd(bestSet, vertices[i]);
    }
    bestSize = numVertices;
  }
  
  // Libertar todos os recursos temporários alocados durante a execução.
  // Esta etapa é crucial para evitar memory leaks.
  free(vertices);                     // Array obtido de IndicesSetGetElems
  IndicesSetDestroy(&allVertices);    // Conjunto temporário de todos os vértices
  IndicesSetDestroy(&currentSet);     // Conjunto usado na procura recursiva
  
  // Retornar o melhor conjunto dominante encontrado.
  return bestSet;
}





// Estrutura de contexto para a procura exaustiva do conjunto dominante com peso mínimo. Similar à anterior, mas inclui informações sobre os pesos dos vértices.
typedef struct {
    const Graph* graph;              // Ponteiro para o grafo original
    const unsigned int* vertices;    // Array com todos os vértices
    const double* vertexWeights;     // Array com os pesos de cada vértice
    unsigned int numVertices;        // Número total de vértices
    IndicesSet* bestSet;             // Melhor conjunto encontrado
    double bestWeight;               // Melhor peso encontrado
} WeightSearchContext;

// Função auxiliar recursiva para procura exaustiva com minimização de peso.
// O algoritmo é similar ao anterior, mas em vez de minimizar a cardinalidade,
// minimiza a soma dos pesos dos vértices no conjunto.

// Parâmetros adicionais:
// - currentWeight: Peso acumulado do conjunto atual
static void _searchMinWeightDominatingSet(WeightSearchContext* ctx, 
                                          IndicesSet* currentSet, 
                                          unsigned int startIdx, 
                                          double currentWeight) {
    InstrCount[1]++; // Count recursive calls
    // Poda por peso: se o peso acumulado do conjunto atual já é maior ou igual ao melhor peso encontrado até agora, não vale a pena continuar a explorar este ramo. Esta poda é fundamental para a eficiência.
    if (currentWeight >= ctx->bestWeight) {
        return;  // Podar este ramo da árvore de busca
    }
    
    // Verificar se o conjunto atual é dominante.
    // Note-se que, ao contrário da versão sem pesos, não verificamos se currentSize > 0 porque a poda por peso já trata do caso do conjunto vazio (peso 0, que será melhor que qualquer conjunto com peso positivo).
    if (IndicesSetGetNumElems(currentSet) > 0 && 
        GraphIsDominatingSet(ctx->graph, currentSet)) {
        // Encontramos um conjunto dominante válido com peso menor
        if (currentWeight < ctx->bestWeight) {
            // Atualizar o melhor peso encontrado
            ctx->bestWeight = currentWeight;
            
            // Atualizar o melhor conjunto
            IndicesSetDestroy(&(ctx->bestSet));
            ctx->bestSet = IndicesSetCreateEmpty(GraphGetVertexRange(ctx->graph));
            unsigned int* elems = _getElems(currentSet);
            unsigned int size = IndicesSetGetNumElems(currentSet);
            for (unsigned int i = 0; i < size; i++) {
                IndicesSetAdd(ctx->bestSet, elems[i]);
            }
            free(elems);
        }
        return;  // Não expandir mais este conjunto
    }
    
    // Continuar a exploração recursiva
    for (unsigned int i = startIdx; i < ctx->numVertices; i++) {
        unsigned int v = ctx->vertices[i];
        double weight = ctx->vertexWeights[v];
        
        // Ignorar vértices com peso negativo. Na função GraphComputeVertexWeights,
        // vértices que não existem no grafo têm peso -1.0.
        if (weight < 0) continue;
        
        // Adicionar o vértice ao conjunto atual
        IndicesSetAdd(currentSet, v);
        
        // Chamada recursiva com peso atualizado
        _searchMinWeightDominatingSet(ctx, currentSet, i + 1, 
                                     currentWeight + weight);
        
        // Backtracking
        IndicesSetRemove(currentSet, v);
    }
}

//
// TO BE COMPLETED
//
// Compute a MIN WEIGHT VERTEX DOMINATING SET of the graph
// using an EXHAUSTIVE SEARCH approach
// Return the dominating set
//
// Esta função encontra o conjunto dominante com menor peso total, onde o peso de um conjunto é a soma dos pesos dos seus vértices. Os pesos dos vértices são obtidos através da função GraphComputeVertexWeights.
// Para grafos não ponderados, GraphComputeVertexWeights atribui peso 1 a cada vértice, e portanto esta função é equivalente a encontrar o conjunto dominante com menor cardinalidade.
// Para grafos ponderados, os pesos podem representar custos, importâncias, ou outras métricas associadas aos vértices.
// Complexidade temporal: O(2^V * (V+E)), semelhante à versão sem pesos.
//
IndicesSet* GraphComputeMinWeightDominatingSet(const Graph* g) {
  assert(g != NULL);
  assert(GraphIsDigraph(g) == 0);

  //
  // TO BE COMPLETED
  //

  // Primeiro passo: obter os pesos de todos os vértices do grafo.
  // A função GraphComputeVertexWeights devolve um array de tamanho
  // g->indicesRange, onde:
  // - vertexWeights[v] = peso do vértice v (≥ 0) se v existe no grafo
  // - vertexWeights[v] = -1.0 se v não existe no grafo
  double* vertexWeights = GraphComputeVertexWeights(g);
  
  // Obter todos os vértices que existem no grafo (como na função anterior)
  IndicesSet* allVertices = GraphGetSetVertices(g);
  unsigned int* vertices = _getElems(allVertices);
  unsigned int numVertices = IndicesSetGetNumElems(allVertices);
  
  // Inicializar o melhor conjunto (vazio) e o melhor peso.
  // DBL_MAX é uma constante definida em float.h que representa o maior valor possível para um double. Qualquer peso real será menor que este valor.
  IndicesSet* bestSet = IndicesSetCreateEmpty(GraphGetVertexRange(g));
  double bestWeight = DBL_MAX;
  
  // Preparar o contexto para a procura recursiva
  WeightSearchContext ctx;
  ctx.graph = g;
  ctx.vertices = vertices;
  ctx.vertexWeights = vertexWeights;
  ctx.numVertices = numVertices;
  ctx.bestSet = bestSet;
  ctx.bestWeight = bestWeight;
  
  // Conjunto temporário para a procura recursiva
  IndicesSet* currentSet = IndicesSetCreateEmpty(GraphGetVertexRange(g));
  
  // Iniciar a procura exaustiva com peso inicial 0.0
  _searchMinWeightDominatingSet(&ctx, currentSet, 0, 0.0);
  
  // Atualizar o melhor peso encontrado
  bestWeight = ctx.bestWeight;
  bestSet = ctx.bestSet;
  
  // Caso especial: se não foi encontrado nenhum conjunto dominante (o que só deve acontecer para grafos sem vértices ou com erro), usar todos os vértices como fallback.
  if (IndicesSetIsEmpty(bestSet) && numVertices > 0) {
    double totalWeight = 0.0;
    for (unsigned int i = 0; i < numVertices; i++) {
      unsigned int v = vertices[i];
      if (vertexWeights[v] >= 0) {  // Apenas vértices existentes
        IndicesSetAdd(bestSet, v);
        totalWeight += vertexWeights[v];
      }
    }
    bestWeight = totalWeight;
  }
  
  // Libertar todos os recursos temporários
  // IMPORTANTE: vertexWeights foi alocado por GraphComputeVertexWeights e deve ser libertado com free, não com IndicesSetDestroy.
  free(vertexWeights);
  free(vertices);
  IndicesSetDestroy(&allVertices);
  IndicesSetDestroy(&currentSet);
  
  // Retornar o conjunto dominante com menor peso encontrado
  return bestSet;
}