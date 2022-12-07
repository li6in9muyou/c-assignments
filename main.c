#include <stdbool.h>
#include <stdio.h>
#include "stdlib.h"

struct Node {
  int data;
  struct Node* left;
  struct Node* right;
};
typedef struct Node Node;

#define SIZE 40

struct queue {
  Node* items[SIZE];
  int front;
  int rear;
};

struct queue* createQueue();
void enqueue(struct queue* q, Node*);
Node* dequeue(struct queue* q);
bool isEmptyQueue(struct queue* q);

void printNode(Node* node, int offset, int depth, char s[20][255]);

struct queue* createQueue() {
  struct queue* q = malloc(sizeof(struct queue));
  q->front = -1;
  q->rear = -1;
  return q;
}

bool isEmptyQueue(struct queue* q) {
  if (q->rear == -1)
    return 1;
  else
    return 0;
}

void enqueue(struct queue* q, Node* value) {
  if (q->rear == SIZE - 1) {
    printf("\nQueue is Full!!");
  } else {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

Node* dequeue(struct queue* q) {
  Node* item;
  if (isEmptyQueue(q)) {
    item = NULL;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
      q->front = q->rear = -1;
    }
  }
  return item;
}

bool isNullNode(Node* node) {
  return node == NULL || node->data == 0;
}

Node* fromLevelOrderEncoding(const int levelOrder[], int count) {
  const int* read = &levelOrder[1];
  Node* root = (Node*) malloc(sizeof(Node));
  root->data = *read;
  read++;

  struct queue* Q = createQueue();
  enqueue(Q, root);

  while (!isEmptyQueue(Q)) {
    Node* sub = dequeue(Q);

    int leftValue = *read;
    read++;
    if (leftValue == 0) {
      sub->left = NULL;
    } else {
      sub->left = (Node*) malloc(sizeof(Node));
      sub->left->data = leftValue;
      enqueue(Q, sub->left);
    }

    int rightValue = *read;
    read++;
    if (rightValue == 0) {
      sub->right = NULL;
    } else {
      sub->right = (Node*) malloc(sizeof(Node));
      sub->right->data = rightValue;
      enqueue(Q, sub->right);
    }
  }
  return root;
}

Node* fromPreOrderEncodingHelper(
    const int preOrder[],
    int count,
    int start,
    int* nextNodeStart
) {
  if (start >= count) {
    return NULL;
  }

  Node* root = (Node*) malloc(sizeof(Node));
  root->data = preOrder[start];
  if (isNullNode(root)) {
    *nextNodeStart += 1;
    root->left = NULL;
    root->right = NULL;
  } else {
    int next = start + 1;
    root->left = fromPreOrderEncodingHelper(preOrder, count, next, &next);
    root->right = fromPreOrderEncodingHelper(preOrder, count, next, &next);
    *nextNodeStart = next;
  }
  return root;
}

Node* fromPreOrderEncoding(
    const int preOrder[],
    int count,
    int start
) {
  int next = start;
  return fromPreOrderEncodingHelper(preOrder, count, start, &next);
}

Node* Stack[100];
Node** pTop = Stack;

void Stack_push(Node* x) {
  *pTop = x;
  pTop += 1;
}

Node* Stack_peek() {
  return *(pTop - 1);
}

Node* Stack_pop() {
  Node* ans = Stack_peek();
  pTop -= 1;
  return ans;
}

bool Stack_empty() {
  return pTop == Stack;
}

void postTravel(Node* root) {
  Node* cur;
  Node* pre = NULL;
  Stack_push(root);
  while (!Stack_empty()) {
    cur = Stack_peek();
    if ((cur->left == NULL && cur->right == NULL)
        || (pre != NULL && (pre == cur->left || pre == cur->right))) {
      printf("%d ", cur->data);
      Stack_pop();
      pre = cur;
    } else {
      if (cur->right != NULL) {
        Stack_push(cur->right);
      }
      if (cur->left != NULL) {
        Stack_push(cur->left);
      }
    }
  }
}

#define width 5

void printLineSegment(char frameBuffer[20][255], int y, int left, int right) {
  for (int x = left; x < right; x++)
    frameBuffer[y][x] = '-';

  frameBuffer[y][left] = '+';
  frameBuffer[y][right] = '+';
}

int printSubTree(Node* tree, int is_left, int offset, int depth, char s[20][255]) {
  if (!tree) return 0;

  int leftSubTreeWidth = printSubTree(tree->left, true, offset, depth + 1, s);
  int rightSubTreeWidth = printSubTree(tree->right, false, offset + leftSubTreeWidth + width, depth + 1, s);
  printNode(tree, offset + leftSubTreeWidth, depth, s);

  if (depth && is_left) {
    int middleOfThisNode = offset + leftSubTreeWidth + width / 2;
    printLineSegment(
        s,
        2 * depth - 1,
        middleOfThisNode,
        middleOfThisNode + width + rightSubTreeWidth
    );
  }

  if (depth && !is_left) {
    int middleOfThisNode = offset - width / 2 - 1;
    printLineSegment(
        s,
        2 * depth - 1,
        middleOfThisNode,
        middleOfThisNode + leftSubTreeWidth + width
    );
  }
  return leftSubTreeWidth + width + rightSubTreeWidth;
}

void printNode(Node* node, int offset, int depth, char s[20][255]) {
  char b[20];
  if (isNullNode(node)) {
    sprintf_s(b, 6, " NULL");
  } else {
    sprintf_s(b, 6, "(%03d)", node->data);
  }
  for (int i = 0; i < width; i++)
    s[2 * depth][offset + i] = b[i];
}

void printFromRoot(Node* tree) {
  char frameBuffer[20][255];
  for (int i = 0; i < 20; i++)
    sprintf_s(frameBuffer[i], 255, "%254s", "");

  printSubTree(tree, false, 0, 0, frameBuffer);

  for (int i = 0; i < 20; i++) {
    printf("%s\n", frameBuffer[i]);

    bool emptyLine = true;
    for (int j = 0; j < 80; ++j) {
      if (frameBuffer[i][j] != ' ') {
        emptyLine = false;
      }
    }
    if (emptyLine) {
      break;
    }
  }
}

int main() {
  puts("this program create a binary tree\n"
       "based on its pre-order traversal sequence "
       "or level-order traversal sequence");
  puts("\nchoose: pre-order (enter 1) or level-order (enter 2)");
  int format;
  scanf_s("%d", &format);
  if (format == 1) {
    puts("you chose pre-order");
  } else if (format == 2) {
    puts("you chose level-order");
  } else {
    puts("wrong input, expecting 1 or 2");
    exit(EXIT_FAILURE);
  }
  int nodeCnt;
  puts("enter node count");
  scanf_s("%d", &nodeCnt);
  printf("%d nodes\n", nodeCnt);

  puts("use -1 to indicate NULL nodes");
  int* preOrderCompleteBinaryTree = calloc(nodeCnt + 1, sizeof(int));
  for (int i = 0; i < nodeCnt; ++i) {
    scanf_s("%d", &preOrderCompleteBinaryTree[i + 1]);
  }

  printf("you entered sequence: ");
  for (int i = 0; i < nodeCnt; ++i) {
    printf("%d ", preOrderCompleteBinaryTree[i + 1]);
  }
  puts("");

  Node* root;
  if (format == 2) {
    root = fromLevelOrderEncoding(preOrderCompleteBinaryTree, nodeCnt);
  } else {
    root = fromPreOrderEncoding(preOrderCompleteBinaryTree, nodeCnt + 1, 1);
  }

  printf("post order traversal: ");
  postTravel(root);
  putchar('\n');
  puts("pretty print tree: ");
  printFromRoot(root);

  return 0;
}
