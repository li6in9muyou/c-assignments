#include <stdio.h>

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
  heap->size += 1;
  int i = heap->size - 1;
  heap->data[i] = value;
  while (i != 0 && heap->data[parent(i)] > heap->data[i]) {
    swap(&heap->data[i], &heap->data[parent(i)]);
    i = parent(i);
  }
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
  return 0;
}
