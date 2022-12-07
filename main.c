#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <malloc.h>

#define nVertex 10

#define SIZE 40

struct queue {
  int items[SIZE];
  int front;
  int rear;
};

struct queue* createQueue();
void enqueue(struct queue* q, int);
int dequeue(struct queue* q);
int isEmpty(struct queue* q);
void printQueue(struct queue* q);

struct queue* createQueue() {
  struct queue* q = malloc(sizeof(struct queue));
  q->front = -1;
  q->rear = -1;
  return q;
}

int isEmpty(struct queue* q) {
  if (q->rear == -1)
    return 1;
  else
    return 0;
}

void enqueue(struct queue* q, int value) {
  if (q->rear == SIZE - 1) {
    printf("\nQueue is Full!!");
  } else {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

int dequeue(struct queue* q) {
  int item;
  if (isEmpty(q)) {
    item = -1;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
      q->front = q->rear = -1;
    }
  }
  return item;
}

void printQueue(struct queue* q) {
  int i;
  printf("[");
  if (!isEmpty(q)) {
    for (i = q->front; i < q->rear + 1; i++) {
      printf("%d", q->items[i]);
      if (i < q->rear) {
        printf(" ");
      }
    }
  }
  printf("]");
}

void bfs(int graph[nVertex][nVertex], int startVertex) {
  bool visited[nVertex];
  for (int i = 0; i < nVertex; ++i) {
    visited[i] = false;
  }

  struct queue* q = createQueue();
  visited[startVertex] = true;
  enqueue(q, startVertex);

  printf("Processed Vertex\tQueue");

  while (!isEmpty(q)) {
    puts("");
    int me = dequeue(q);
    printf("%d\t\t\t", me);
    printQueue(q);

    for (int ngb = 0; ngb < nVertex; ++ngb) {
      if (0 == graph[me][ngb]) {
        continue;
      }

      if (visited[ngb] == false) {
        visited[ngb] = true;
        enqueue(q, ngb);
      }
    }
  }
}

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

  puts("\nBFS traversal");
  bfs(edgeWeights, 0);

  return 0;
}