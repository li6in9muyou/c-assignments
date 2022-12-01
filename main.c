#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#define nVertex 10

int minDistance(const int dist[], const bool sptSet[]) {
  int min = INT_MAX;
  int min_index;

  for (int v = 0; v < nVertex; v++) {
    if (sptSet[v] == false && dist[v] <= min) {
      min = dist[v];
      min_index = v;
    }
  }

  return min_index;
}

void printSolution(const int dist[nVertex], const int parent[nVertex], const int weights[nVertex][nVertex]) {
  printf("Vertex\t\tDistance\tPath\n");
  for (int i = 0; i < nVertex; i++) {
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

void dijkstra(int graph[nVertex][nVertex], int src) {
  bool processed[nVertex];

  int dist[nVertex];
  for (int i = 0; i < nVertex; i++) {
    dist[i] = INT_MAX;
    processed[i] = false;
  }
  dist[src] = 0;

  int previousStop[nVertex];
  previousStop[src] = -1;
  for (int i = 0; i < nVertex - 1; i++) {
    int u = minDistance(dist, processed);
    bool reachable = dist[u] != INT_MAX;
    if (!reachable) {
      continue;
    }

    processed[u] = true;
    for (int nbg = 0; nbg < nVertex; nbg++) {
      bool isNeighbour = graph[u][nbg] != 0;
      if (isNeighbour && !processed[nbg]
          && dist[u] + graph[u][nbg] < dist[nbg]) {
        dist[nbg] = dist[u] + graph[u][nbg];
        previousStop[nbg] = u;
      }
    }
  }

  printSolution(dist, previousStop, graph);
}

int main() {
  int edgeWeights[nVertex][nVertex] = {
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