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

// bfs 和 dfs 唯一的差别是 queue 和 stack ，这使得它们处理各节点的顺序不一样。

void bfs(int graph[nVertex][nVertex], int startVertex) {
  bool visited[nVertex];
  for (int i = 0; i < nVertex; ++i) {
    visited[i] = false;
  }

  struct queue* q = createQueue();
  visited[startVertex] = true;
  enqueue(q, startVertex);

  printf("Processed Vertex\tQueue");

  while (!isEmptyQueue(q)) {
    puts("");
    int me = dequeue(q);
    printf("%d\t\t\t", me);

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

  printf("Processed Vertex\tStack");

  while (!isEmptyStack(s)) {
    puts("");
    int me = pop(s);
    printf("%d\t\t\t", me);
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

int closestUnprocessedNode(const int dist[], const bool processed[]) {
  int min = INT_MAX;
  int min_index;

  for (int v = 0; v < nVertex; v++) {
    if (processed[v] == false && dist[v] <= min) {
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
    // 跟踪前序节点，并打印路径上的节点和距离，直到再无前序节点。
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
  // 开始时，从原节点到各节点的距离都是无穷，且各节点都未被处理。
  int dist[nVertex];
  bool processed[nVertex];
  for (int i = 0; i < nVertex; i++) {
    dist[i] = INT_MAX;
    processed[i] = false;
  }

  // 源节点到源节点的距离为零。
  dist[src] = 0;

  // 该数组记录到某一节点的最短路上其前序节点是谁。
  int previousStop[nVertex];
  previousStop[src] = -1;
  for (int i = 0; i < nVertex - 1; i++) {
    // 找出未被处理的节点中，距离最小的一个
    int u = closestUnprocessedNode(dist, processed);
    // 节点不可达则跳过
    bool reachable = dist[u] != INT_MAX;
    if (!reachable) {
      continue;
    }

    processed[u] = true;
    // 遍历该最小距离节点的额各个邻接节点。
    for (int nbg = 0; nbg < nVertex; nbg++) {
      bool isNeighbour = graph[u][nbg] != 0;
      if (isNeighbour && !processed[nbg]
          && dist[u] + graph[u][nbg] < dist[nbg]) {
        // 找到通往 ngb 节点的更小的路径 i -> u -> ngb ，相应地更新距离表和前序节点表。
        dist[nbg] = dist[u] + graph[u][nbg];
        previousStop[nbg] = u;
      }
    }
  }

  printSolution(dist, previousStop, graph);
}

int main() {
  // 硬编码给定的图为邻接矩阵，优点为简洁易懂，直观形象。
  // 0 表示不存在边，距离为 edgeWeights[from][to] 。
  int edgeWeights[nVertex][nVertex] = {
      {0, 2, 0, 5, 0, 0, 0, 0, 0, 0,},
      {2, 0, 5, 2, 0, 0, 0, 0, 0, 0,},
      {0, 5, 0, 0, 8, 4, 0, 0, 0, 0,},
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

  puts("\n\nDFS traversal");
  dfs(edgeWeights, 0);

  return 0;
}