#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define INITIAL_CAPACITY 8

size_t hash_int(int x) {
  size_t result = 0;
  memcpy(&result, &x, sizeof(int));
  return result;
}

typedef struct Node {
  struct Node* next;
  int key;
} Node;

struct FindResult {
  Node* node;
  Node* prev;
};

Node* node_ctor(int key) {
  Node* node = (Node*) calloc(1, sizeof(Node));
  assert(node);
  node->key = key;
  return node;
}

struct FindResult list_find(Node* head, int key) {
  Node* node = head;
  Node* prev = NULL;
  struct FindResult res;
  while (node != NULL) {
    if (key == node->key) {
      res.node = node;
      res.prev = prev;
      return res;
    }
    prev = node;
    node = node->next;
  }
  res.node = node;
  res.prev = prev;
  return res;
}

typedef struct { 
  Node** buckets;
  size_t size;
  size_t capacity;
} HashTable;


double load_factor(HashTable* ht) {
  assert(ht);
  return (double)ht->size / (double)ht->capacity;
}

int ht_ctor(HashTable* ht) {
  assert(ht);
  ht->buckets = calloc(INITIAL_CAPACITY, sizeof(Node*));
  assert(ht->buckets);
  ht->size = 0;
  ht->capacity = INITIAL_CAPACITY;
  return EXIT_SUCCESS;
}

int ht_dtor(HashTable* ht) {
  for (size_t i = 0; i < ht->capacity; ++i) {
    Node* now = ht->buckets[i];
    while (now != NULL) {
      Node* prev = now;
      now = now->next;
      free(prev);
    }
  }
  return EXIT_SUCCESS;
}

struct FindResult node_find(HashTable* ht, int key) {
  assert(ht);
  size_t hash = hash_int(key) % ht->capacity;
  return list_find(ht->buckets[hash], key);
}


int ht_contains(HashTable* ht, int key) {
  assert(ht);
  struct FindResult res = node_find(ht, key);
  return res.node != NULL;
}

void ht_rehash(HashTable* ht) {
  size_t new_capacity = ht->capacity * 2;
  Node** new_buckets = calloc(new_capacity, sizeof(Node*));
  assert(new_buckets);
  for (size_t i = 0; i < ht->capacity; ++i) {
    Node* now = ht->buckets[i];
    while (now != NULL) {
      size_t hash = hash_int(now->key) % new_capacity;
      Node* next = new_buckets[hash];
      new_buckets[hash] = node_ctor(now->key);
      new_buckets[hash]->next = next;
      Node* prev = now;
      now = now->next;
      free(prev);
    }
  }
  free(ht->buckets);
  ht->capacity = new_capacity;
  ht->buckets = new_buckets;
}

int ht_insert(HashTable* ht, int key) {
  assert(ht);
  if (load_factor(ht) > 0.7) {
    ht_rehash(ht);
  }
  struct FindResult res = node_find(ht, key);
  if (res.node == NULL) {
    Node* new_node = node_ctor(key);
    if (res.prev == NULL) {
      size_t hash = hash_int(key) % ht->capacity;
      ht->buckets[hash] = new_node;
    } else {
      res.prev->next = new_node;
    }
    ++ht->size;
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

int ht_erase(HashTable* ht, int key) {
  assert(ht);
  struct FindResult res = node_find(ht, key);
  if (res.node != NULL) {
    if (res.prev == NULL) {
      size_t hash = hash_int(key) % ht->capacity;
      ht->buckets[hash] = res.node->next;
    } else {
      res.prev->next = res.node->next;
    }
    free(res.node);
    --ht->size;
  }
  return EXIT_FAILURE;
}

size_t ht_size(HashTable* ht) {
  return ht->size;
}

int main() {
  HashTable ht;
  ht_ctor(&ht);
  int n = 0;
  scanf("%d", &n);
  int x = 5;

  ht_insert(&ht, x);

  if (ht_contains(&ht, x)) {
    puts("YES");
  } else {
    puts("NO");
  }

  ht_erase(&ht, x);
  ht_dtor(&ht);
}
