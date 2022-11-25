#include <stdbool.h>
#include <stdio.h>
#include "stdlib.h"

struct Node {
  int index;
  struct Node* left;
  struct Node* right;
};
typedef struct Node Node;

Node* Tree_new(const int preOrder[], int count, int start) {
  if (start >= count) {
    return NULL;
  }

  Node* root = (Node*) malloc(sizeof(Node));
  root->index = preOrder[start];
  root->left = Tree_new(preOrder, count, 2 * start);
  root->right = Tree_new(preOrder, count, 2 * start + 1);
  return root;
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
      printf("%d ", cur->index);
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

int _print_t(Node* tree, int is_left, int offset, int depth, char s[20][255]) {
  char b[20];
  int width = 5;

  if (!tree) return 0;

  sprintf(b, "(%03d)", tree->index);

  int left = _print_t(tree->left, 1, offset, depth + 1, s);
  int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);

  for (int i = 0; i < width; i++)
    s[2 * depth][offset + left + i] = b[i];

  if (depth && is_left) {
    for (int i = 0; i < width + right; i++)
      s[2 * depth - 1][offset + left + width / 2 + i] = '-';

    s[2 * depth - 1][offset + left + width / 2] = '+';
    s[2 * depth - 1][offset + left + width + right + width / 2] = '+';
  }

  if (depth && !is_left) {
    for (int i = 0; i < left + width; i++)
      s[2 * depth - 1][offset - width / 2 + i] = '-';

    s[2 * depth - 1][offset + left + width / 2] = '+';
    s[2 * depth - 1][offset - width / 2 - 1] = '+';
  }
  return left + width + right;
}

void print_t(Node* tree) {
  char frameBuffer[20][255];
  for (int i = 0; i < 20; i++)
    sprintf(frameBuffer[i], "%254s", "");

  _print_t(tree, 0, 0, 0, frameBuffer);

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
  int nodeCnt;
  puts("enter node count");
  scanf_s("%d", &nodeCnt);

  int* preComplete = calloc(nodeCnt + 1, sizeof(int));
  for (int i = 0; i < nodeCnt; ++i) {
    scanf_s("%d", &preComplete[i]);
  }

  Node* root = Tree_new(preComplete, nodeCnt, 1);

  postTravel(root);
  putchar('\n');
  print_t(root);

  return 0;
}
