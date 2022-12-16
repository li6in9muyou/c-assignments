#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define nVertex 10

void prim(int edges[nVertex][nVertex]);
void kruskal(int weights[nVertex][nVertex]);

void show_edge(int adjacentMatrix[nVertex][nVertex], int x, int y);

typedef struct Edge Edge;
int extractAllEdges(int weights[10][10], Edge* edges);

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
  Edge edges[nVertex * nVertex];
  int edgesSize = extractAllEdges(weights, edges);

  qsort(
      edges,
      edgesSize,
      sizeof(Edge),
      weight_ascending
  );

  int u[nVertex];
  for (int i = 0; i < nVertex; ++i) {
    u[i] = i;
  }

  int treeWeight = 0;
  for (int i = 0; i < edgesSize; ++i) {
    Edge e = edges[i];
    if (do_find(u, e.one) != do_find(u, e.two)) {
      do_union(u, e.one, e.two);
      show_edge(weights, e.one, e.two);
      treeWeight += e.weight;
    }
  }
  printf("tree weight: %d\n", treeWeight);
}

int extractAllEdges(int weights[10][10], Edge* edges) {
  int edgeCnt = 0;
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
  return edgeCnt;
}

void prim(int edges[nVertex][nVertex]) {
  int edgeCnt = 0;
  int treeWeight = 0;
  int vertexIsSelected[nVertex] = {false,};

  vertexIsSelected[nVertex - 1] = true;

  while (edgeCnt < nVertex - 1) {
    int min = INT_MAX;
    int x = 0;
    int y = 0;
    for (int i = 0; i < nVertex; ++i) {
      if (vertexIsSelected[i]) {
        for (int j = 0; j < nVertex; ++j) {
          if (!vertexIsSelected[j] && edges[i][j]) {
            if (min > edges[i][j]) {
              min = edges[i][j];
              x = i;
              y = j;
            }
          }
        }
      }
    }
    treeWeight += edges[x][y];
    show_edge(edges, x, y);

    vertexIsSelected[y] = true;
    edgeCnt += 1;
  }
  printf("tree weight: %d\n", treeWeight);
}

void show_edge(int adjacentMatrix[nVertex][nVertex], int x, int y) {
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
