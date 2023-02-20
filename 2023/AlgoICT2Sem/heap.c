#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef char BYTE;

#define INITIAL_CAPACITY 1024

#define get_ptr(heap, index) ((BYTE*)heap->data + (index) * heap->element_size)

/* Comparators */

int i_less(const void* a, const void* b) {
  return *(const int*)a < *(const int*)b ? -1 : 1;
}

int i_greater(const void* a, const void* b) {
  return *(const int*)a > *(const int*)b ? -1 : 1;
}

typedef struct {
  void* data;
  size_t element_size;
  size_t size; 
  size_t capacity; 
  int (*cmp)(const void* a, const void* b);
} Heap;

int heap_ctor(Heap* heap, size_t element_size, int (*cmp)(const void* a, const void* b));

void heap_dtor(Heap* heap);

void swap(Heap* heap, size_t index1, size_t index2);

void sift_up(Heap* heap, size_t index);

void sift_down(Heap* heap, size_t index);

int insert(Heap* heap, const void* value);

int top(Heap* heap, void* value);

int pop(Heap* heap);

int reserve(Heap* heap, size_t new_capacity);

int main() {
  srand(time(NULL));

  Heap priority_queue;
  assert(heap_ctor(&priority_queue, sizeof(int), i_less));
  int a[1000];

  int n, k;
  scanf("%d %d", &n, &k);

  for (size_t i = 0; i < n; i++) {
    a[i] = rand() % 1000;
    printf("%d ", a[i]);
  }
  puts("");

  for (size_t i = 0; i < n; i++) {
    if (priority_queue.size < k) {
      insert(&priority_queue, &a[i]);
    } else {
      int x = 0;
      top(&priority_queue, &x);
      if (x > a[i]) {
        pop(&priority_queue);
        insert(&priority_queue, &a[i]);
      }
    }
  }
  
  for (int i = 0; i < k; i++) {
    int x = 0;
    top(&priority_queue, &x);
    printf("%d\n", x);
    pop(&priority_queue);
  }

  heap_dtor(&priority_queue);


  return 0;
}

int heap_ctor(Heap* heap, size_t element_size, int (*cmp)(const void* a, const void* b)) {
  assert(heap);
  heap->data = calloc(INITIAL_CAPACITY, element_size);
  if (heap->data == NULL) {
    return 0;
  }
  heap->size = 0;
  heap->element_size = element_size;
  heap->capacity = INITIAL_CAPACITY;
  heap->cmp = cmp;
  return 1;
}

void heap_dtor(Heap* heap) {
  assert(heap);
  memset(heap->data, 0, heap->capacity * heap->element_size);
  free(heap->data);
  memset(heap, 0, sizeof(Heap));
  free(heap);
}

void swap(Heap* heap, size_t index1, size_t index2) { // to do на x_orax
  assert(heap);
  assert(index1 < heap->size);
  assert(index2 < heap->size);
  for (size_t i = 0; i < heap->element_size; ++i) {
    BYTE temp = *(get_ptr(heap, index1) + i);
    *(get_ptr(heap, index1) + i) = *(get_ptr(heap, index2) + i);
    *(get_ptr(heap, index2) + i) = temp;
  }
}

void sift_up(Heap* heap, size_t index) {
  assert(heap);
  while (index != 0) {
    size_t parent = (index - 1) / 2;
    if (heap->cmp(get_ptr(heap, parent), get_ptr(heap, index)) < 0) {
      swap(heap, parent, index);
      index = parent;
    } else {
      return;
    }
  }

}

void sift_down(Heap* heap, size_t index) {
  assert(heap);
  if (index  >= heap->size) {
    return;
  }
  size_t left_child = index * 2 + 1;
  size_t right_child = index * 2 + 2;
  size_t i_min = index;
  if (left_child < heap->size 
      && heap->cmp(get_ptr(heap, i_min), get_ptr(heap, left_child)) < 0) {
        i_min = left_child;
  }
  if (right_child < heap->size 
      && heap->cmp(get_ptr(heap, i_min), get_ptr(heap, right_child)) < 0) {
        i_min = right_child;
  }
  if (i_min != index) {
    swap(heap, index, i_min);
    sift_down(heap, i_min);
  }
}


int reserve(Heap* heap, size_t new_capacity) {
  assert(heap);
  if (new_capacity < heap->capacity) {
    return 0;
  }
  void* ptr = realloc(heap->data, new_capacity);
  if (ptr != NULL) {
    heap->capacity = new_capacity;
    heap->data = ptr;
    return 1;
  }
  return 0;
}

int insert(Heap* heap, const void* value) {
  assert(heap);
  if (heap->size == heap->capacity && !reserve(heap, 2 * heap->capacity)) {
    return 0;
  }
  memcpy(
    get_ptr(heap, heap->size), 
    value,
    heap->element_size
  );
  ++heap->size;
  sift_up(heap, heap->size - 1);
  return 1;
}

int top(Heap* heap, void* value) {
  assert(heap);
  if (heap->size == 0) {
    return 0;
  }
  memcpy(value, heap->data, heap->element_size);
  return 1;
}

int pop(Heap* heap) {
  assert(heap);
  if (heap->size == 0) {
    return 0;
  }
  swap(heap, 0, heap->size - 1);
  memset(get_ptr(heap, heap->size - 1), 0xFA, heap->element_size);
  heap->size--;
  sift_down(heap, 0);

  return 1;
}
