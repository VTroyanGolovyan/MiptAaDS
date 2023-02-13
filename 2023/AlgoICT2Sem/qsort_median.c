#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int select_center(int* data, int start, int end) {
  return data[start + (end - start) / 2];
}

void bubble_sort(int* data, int start, int end, int shift) {
  for (int i = 0; i <= end - start + 1; i += shift) { 
    for (int j = start + shift; j <= end - i; j += shift) { 
      if (data[j] < data[j - shift]) {
        int temp = data[j];
        data[j] = data[j - shift];
        data[j - shift] = temp;
      }
    }
  }
}

inline int min(int a, int b) {
  return a < b ? a : b;
}

int median_of_medians_select(int* data, int start, int end, int shift) {
  for (int i = start; i <= end; i += shift) {
    bubble_sort(data, i, min(i + shift - 1, end), shift);
  }
  if ((end - start) / shift <= 1) {
    return data[start + (end - start) / 2];
  } 
  return median_of_medians_select(data, start + (shift + 1) / 2, end, shift * 5);
}

int hoar_partition(int* data, int start, int end) {
  int i = start;
  int j = end;
  int pivot = median_of_medians_select(data, start, end, 1);
  while (i <= j) {
    while (data[i] < pivot) {
      ++i;
    }
    while (data[j] > pivot) {
      --j;
    }

    if (i >= j) {
      return j;
    }

    int temp = data[i];
    data[i] = data[j];
    data[j] = temp;
    ++i;
    --j;
    
  }
  return j;
}

void quick_sort_(int* data, int start, int end) {
  if (start >= end) {
    return;
  }
  int partition_edge = hoar_partition(data, start, end);
  quick_sort_(data, start, partition_edge);
  quick_sort_(data, partition_edge + 1, end);
}


void quick_sort(int* data, int n) {
  quick_sort_(data, 0, n - 1);
}

int k_stat_(int* data, int start, int end, int k) {
  if (start >= end) {
    return data[k];
  }
  int partition_edge = hoar_partition(data, start, end);
  if (k <= partition_edge) {
   return k_stat_(data, start, partition_edge, k);
  }
  return k_stat_(data, partition_edge + 1, end, k);
}

int k_stat(int* data, int n, int k) {
  return k_stat_(data, 0, n - 1, k);
}

int main() {
  int n;
  assert(scanf("%d", &n));
  int k;
  assert(scanf("%d", &k));
  int* a = (int*)calloc(n, sizeof(int));
  assert(a);
  for (int i = 0; i < n; ++i) {
    assert(scanf("%d", a + i));
  }
  
  printf("%d", k_stat(a, n, k));

  return 0;
}
