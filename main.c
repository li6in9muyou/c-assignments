#include <stdio.h>
#include <stdbool.h>

int parent(int i) {
  return (i - 1) / 2;
}

int left(int i) {
  return 2 * i + 1;
}

int right(int i) {
  return 2 * i + 2;
}

typedef struct Heap {
  int size;
  int* data;
} Heap;

void swap(int* a, int* b) {
  int tmp = *b;
  *b = *a;
  *a = tmp;
}

void heap_insert(Heap* heap, int value) {
  heap->data[heap->size] = value;
  int i = heap->size;
  heap->size += 1;
  while (i != 0 && heap->data[parent(i)] > heap->data[i]) {
    swap(&heap->data[i], &heap->data[parent(i)]);
    i = parent(i);
  }
}

int heap_size(Heap* heap) {
  return heap->size;
}

int heap_at(Heap* heap, int i) {
  return heap->data[i];
}

int main() {
  int _a[100];
  Heap _h = {0, _a};
  Heap* h = &_h;

  puts("enter N, followed by N numbers e.g. \"4 1 2 3 4\"");
  int n = 0x1b91b9;
  scanf_s("%d", &n);
  for (int i = 0; i < n; ++i) {
    int number = 0x1b91b9;
    scanf_s("%d", &number);
    heap_insert(h, number);
  }

  for (int i = 0; i < heap_size(h); ++i) {
    printf("给定下标%d，打印从H[%d]到根节点的路径\n", i, i);

    int j = i;
    while (true) {
      printf("%d(%d)", j, heap_at(h, j));
      if (j == 0) {
        break;
      }
      printf(" -> ");

      j = parent(j);
    }

    printf("\n");
  }

  return 0;
}
