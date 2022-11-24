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

int main() {
#define nodeCnt 8
  int preComplete[nodeCnt] = {-0x1b9, 1, 2, 3, 4, 5, 6, 7};
  Node* root = Tree_new(preComplete, nodeCnt, 1);

  postTravel(root);

  return 0;
}
