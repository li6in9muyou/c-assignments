#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#define V 10

int minDistance(int dist[], bool sptSet[]) {
  int min = INT_MAX, min_index;

  for (int v = 0; v < V; v++)
    if (sptSet[v] == false && dist[v] <= min)
      min = dist[v], min_index = v;

  return min_index;
}

void printSolution(const int dist[V], const int parent[V], const int weights[V][V]) {
  printf("Vertex\t\tDistance\tPath\n");
  for (int i = 0; i < V; i++) {
    printf("%d\t\t%d\t\t", i, dist[i]);
    int to = i;
    while (true) {
      int from = parent[to];
      printf("(%d)", to);
      if (from == -1) {
        break;
      }
      printf("-%d-", weights[from][to]);
      to = from;
    }
    puts("");
  }
}

void dijkstra(int graph[V][V], int src) {
  int dist[V];
  int parent[V];

  bool sptSet[V];

  for (int i = 0; i < V; i++)
    dist[i] = INT_MAX, sptSet[i] = false;

  dist[src] = 0;
  parent[src] = -1;

  for (int count = 0; count < V - 1; count++) {
    int u = minDistance(dist, sptSet);

    sptSet[u] = true;

    for (int v = 0; v < V; v++)

      if (!sptSet[v] && graph[u][v]
          && dist[u] != INT_MAX
          && dist[u] + graph[u][v] < dist[v]) {
        dist[v] = dist[u] + graph[u][v];
        parent[v] = u;
      }
  }

  printSolution(dist, parent, graph);
}

int main() {
  int edgeWeights[V][V] = {
      {0, 2, 0, 5, 0, 0, 0, 0, 0, 0,},
      {2, 0, 5, 2, 0, 0, 0, 0, 0, 0,},
      {0, 5, 0, 0, 8, 4, 0, 0, 2, 0,},
      {5, 2, 0, 0, 0, 4, 2, 0, 0, 0,},
      {0, 0, 8, 0, 0, 2, 0, 5, 0, 0,},
      {0, 0, 4, 4, 2, 0, 3, 9, 6, 0,},
      {0, 0, 0, 2, 0, 3, 0, 0, 7, 0,},
      {0, 0, 0, 0, 5, 9, 0, 0, 3, 4,},
      {0, 0, 2, 0, 0, 6, 7, 3, 0, 8,},
      {0, 0, 0, 0, 0, 0, 0, 4, 8, 0,},
  };

  dijkstra(edgeWeights, 0);

  return 0;
}