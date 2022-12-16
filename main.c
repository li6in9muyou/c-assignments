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

struct stack {
  int items[SIZE];
  int* pTop;
};

struct queue* createQueue();
void enqueue(struct queue* q, int);
int dequeue(struct queue* q);
int isEmptyQueue(struct queue* q);
void printQueue(struct queue* q);

struct stack* createStack();
void push(struct stack* s, int item);
int pop(struct stack* s);
int isEmptyStack(struct stack* s);

void prim(int edges[10][10]);
void kruskal(int edges[10][10]);

struct queue* createQueue() {
  struct queue* q = malloc(sizeof(struct queue));
  q->front = -1;
  q->rear = -1;
  return q;
}

int isEmptyQueue(struct queue* q) {
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
  if (isEmptyQueue(q)) {
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
  if (!isEmptyQueue(q)) {
    for (i = q->front; i < q->rear + 1; i++) {
      printf("%d", q->items[i]);
      if (i < q->rear) {
        printf(" ");
      }
    }
  }
  printf("]");
}

void push(struct stack* s, int item) {
  *(s->pTop) = item;
  s->pTop += 1;
}

int pop(struct stack* s) {
  int ans = *(s->pTop - 1);
  s->pTop -= 1;
  return ans;
}

int isEmptyStack(struct stack* s) {
  return s->items == s->pTop;
}

struct stack* createStack() {
  struct stack* s = malloc(sizeof(struct stack));
  s->pTop = s->items;
  return s;
}

void printStack(struct stack* s) {
  printf("[");
  if (!isEmptyStack(s)) {
    for (int* r = s->items; r != s->pTop; r++) {
      printf("%d", *r);
      if (r != s->pTop - 1) {
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

  printf("%-20s%-20s", "Processed Vertex", "Queue");

  while (!isEmptyQueue(q)) {
    puts("");
    int me = dequeue(q);
    printf("%-20d", me);

    for (int ngb = 0; ngb < nVertex; ++ngb) {
      if (0 == graph[me][ngb]) {
        continue;
      }

      if (visited[ngb] == false) {
        visited[ngb] = true;
        enqueue(q, ngb);
      }
    }
    printQueue(q);
  }
}

void dfs(int graph[nVertex][nVertex], int startVertex) {
  bool visited[nVertex];
  for (int i = 0; i < nVertex; ++i) {
    visited[i] = false;
  }

  struct stack* s = createStack();
  visited[startVertex] = true;
  push(s, startVertex);

  printf("%-20s%-20s", "Processed Vertex", "Stack");

  while (!isEmptyStack(s)) {
    puts("");
    int me = pop(s);
    printf("%-20d", me);
    for (int ngb = 0; ngb < nVertex; ++ngb) {
      if (0 == graph[me][ngb]) {
        continue;
      }

      if (visited[ngb] == false) {
        visited[ngb] = true;
        push(s, ngb);
      }
    }
    printStack(s);
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
  printf("%-10s%-10s%-10s\n", "Vertex", "Distance", "Path");
  for (int i = 0; i < nVertex; i++) {
    printf("%-10d%-10d", i, dist[i]);
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
  int dist[nVertex];
  bool processed[nVertex];
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

char vertexLable[nVertex] = "BCDFHLWXYZ";

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

void kruskal(int edges[nVertex][nVertex]) {

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
    printf("%c -(%d)- %c\n", vertexLable[x], edges[x][y], vertexLable[y]);

    vertexIsSelected[y] = true;
    edgeCnt += 1;
  }
  printf("tree weight: %d\n", treeWeight);
}
