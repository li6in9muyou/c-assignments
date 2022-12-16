#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define nVertex 10

void prim(int edges[nVertex][nVertex]);
void kruskal(int weights[nVertex][nVertex]);

void show_edge(int adjacentMatrix[10][10], int x, int y);

int main() {
  int edgeWeights[nVertex][nVertex] = {
      {0, 0, 5, 0, 0, 4, 5, 0, 0, 8,},
      {0, 0, 0, 7, 6, 0, 0, 0, 6, 0,},
      {5, 0, 0, 0, 0, 4, 0, 0, 0, 0,},
      {0, 7, 0, 0, 3, 2, 0, 0, 0, 0,},
      {0, 6, 0, 3, 0, 4, 4, 7, 5, 0,},
      {4, 0, 4, 2, 4, 0, 0, 0, 0, 0,},
      {5, 0, 0, 0, 4, 0, 0, 8, 0, 5,},
      {0, 0, 0, 0, 7, 0, 8, 0, 5, 0,},
      {0, 6, 0, 0, 5, 0, 0, 5, 0, 0,},
      {8, 0, 0, 0, 0, 0, 5, 0, 0, 0,},
  };

  puts("\nBy Prim algorithm");
  prim(edgeWeights);

  puts("\nBy kruskal algorithm");
  kruskal(edgeWeights);

  return 0;
}

struct Edge {
  int one;
  int two;
  int weight;
};

typedef struct Edge Edge;

int do_find(const int parent[nVertex], int i) {
  while (parent[i] != i) {
    i = parent[i];
  }
  return i;
}

void do_union(int parent[nVertex], int i, int j) {
  int a = do_find(parent, i);
  int b = do_find(parent, j);
  parent[a] = b;
}

int weight_ascending(const void* _a, const void* _b) {
  Edge* a = (Edge*) _a;
  Edge* b = (Edge*) _b;
  return a->weight - b->weight;
}

void kruskal(int weights[nVertex][nVertex]) {
  int edgeCnt = 0;
  Edge edges[nVertex * nVertex];
  for (int from = 0; from < nVertex; ++from) {
    for (int to = 0; to < nVertex; ++to) {
      bool isAdjacent = weights[from][to] != 0;
      if (isAdjacent) {
        edges[edgeCnt].weight = weights[from][to];
        edges[edgeCnt].one = from;
        edges[edgeCnt].two = to;
        edgeCnt += 1;
      }
    }
  }

  qsort(
      edges,
      edgeCnt,
      sizeof(Edge),
      weight_ascending
  );

  int u[nVertex];
  for (int i = 0; i < nVertex; ++i) {
    u[i] = i;
  }

  int treeWeight = 0;
  for (int i = 0; i < edgeCnt; ++i) {
    Edge e = edges[i];
    if (do_find(u, e.one) != do_find(u, e.two)) {
      do_union(u, e.one, e.two);
      show_edge(weights, e.one, e.two);
      treeWeight += e.weight;
    }
  }
  printf("tree weight: %d\n", treeWeight);
}

typedef struct Closure {
  bool* vertexIsSelected;
  int* weights;
} Closure;

typedef struct Predicate {
  Closure closure;
  bool (* apply)(Closure closure, Edge);
} Predicate;

Edge* fn_filter(Predicate predicate, Edge array[], int size, Edge* filtered, int* filteredCount) {
  *filteredCount = 0;
  for (int i = 0; i < size; ++i) {
    if (predicate.apply(predicate.closure, array[i])) {
      filtered[*filteredCount] = array[i];
      (*filteredCount) += 1;
    }
  }
  return filtered;
}

Edge fn_min(int(CompareFunction(const void*, const void*)), Edge array[], const int* size) {
  Edge e = array[0];
  for (int i = 0; i < *size; ++i) {
    if (CompareFunction(&array[i], &e) < 0) {
      e = array[i];
    }
  }
  return e;
}

bool doesNotFormLoop(Closure closure, Edge e) {
  return closure.vertexIsSelected[e.one]
      && !closure.vertexIsSelected[e.two]
      && ((int (*)[nVertex]) closure.weights)[e.one][e.two];
}

void prim(int weights[nVertex][nVertex]) {
  int edgeCnt = 0;
  Edge edges[nVertex * nVertex];
  for (int from = 0; from < nVertex; ++from) {
    for (int to = 0; to < nVertex; ++to) {
      bool isAdjacent = weights[from][to] != 0;
      if (isAdjacent) {
        edges[edgeCnt].weight = weights[from][to];
        edges[edgeCnt].one = from;
        edges[edgeCnt].two = to;
        edgeCnt += 1;
      }
    }
  }

  int treeWeight = 0;
  bool vertexIsSelected[nVertex] = {false,};

  vertexIsSelected[nVertex - 1] = true;

  Predicate predicate = {
      (Closure) {vertexIsSelected, (int*) weights},
      doesNotFormLoop
  };

  int edgeCntInTree = 0;
  while (edgeCntInTree < nVertex - 1) {
    int filteredCount = 0;
    Edge filtered[nVertex * nVertex];
    Edge e = fn_min(
        weight_ascending,
        fn_filter(
            predicate,
            edges,
            edgeCnt,
            filtered,
            &filteredCount
        ),
        &filteredCount
    );
    vertexIsSelected[e.one] = true;
    vertexIsSelected[e.two] = true;

    treeWeight += e.weight;
    show_edge(weights, e.one, e.two);

    edgeCntInTree += 1;
  }
  printf("tree weight: %d\n", treeWeight);
}

void show_edge(int adjacentMatrix[10][10], int x, int y) {
  int smaller = min(x, y);
  int bigger = max(x, y);

  const char vertexLabel[nVertex] = "BCDFHLWXYZ";
  printf(
      "%c -(%d)- %c\n",
      vertexLabel[smaller],
      adjacentMatrix[smaller][bigger],
      vertexLabel[bigger]
  );
}
