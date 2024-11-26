#include "rbtree.h"
#include "rbtree.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void test_init(void) {
  rbtree *t = new_rbtree();
  delete_rbtree(t);
}

// root node should have proper values and pointers
void test_insert_single(const key_t key) {
  rbtree *t = new_rbtree();
  node_t *p = rbtree_insert(t, key);  // 
  delete_rbtree(t);
}

static void insert_arr(rbtree *t, const key_t *arr, const size_t n) {
  for (size_t i = 0; i < n; i++) {
    rbtree_insert(t, arr[i]);
  }
}

static int comp(const void *p1, const void *p2) {
  const key_t *e1 = (const key_t *)p1;
  const key_t *e2 = (const key_t *)p2;
  if (*e1 < *e2) {
    return -1;
  } else if (*e1 > *e2) {
    return 1;
  } else {
    return 0;
  }
};


void test_to_array(rbtree *t, const key_t *arr, const size_t n) {

  insert_arr(t, arr, n);
  qsort((void *)arr, n, sizeof(key_t), comp);

  key_t *res = calloc(n, sizeof(key_t));
  rbtree_to_array(t, res, n);
  for (int i = 0; i < n; i++) {

  }
  free(res);
}

void test_to_array_suite() {
  rbtree *t = new_rbtree();

  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_to_array(t, entries, n);

  delete_rbtree(t);
}

// min/max should return the min/max value of the tree
void test_minmax(key_t *arr, const size_t n) {
  // null array is not allowed

  rbtree *t = new_rbtree();

  insert_arr(t, arr, n);

  qsort((void *)arr, n, sizeof(key_t), comp);
  node_t *p = rbtree_min(t);

  node_t *q = rbtree_max(t);

  rbtree_erase(t, p);
  p = rbtree_min(t);

  if (n >= 2) {
    rbtree_erase(t, q);
    q = rbtree_max(t);


  }

  delete_rbtree(t);
}

void test_minmax_suite() {
  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_minmax(entries, n);
}

// find should return the node with the key or NULL if no such node exists
void test_find_single(const key_t key, const key_t wrong_key) {
  rbtree *t = new_rbtree();
  node_t *p = rbtree_insert(t, key);

  node_t *q = rbtree_find(t, key);

  q = rbtree_find(t, wrong_key);

  delete_rbtree(t);
}

void test_erase_root(const key_t key) {
  rbtree *t = new_rbtree();
  node_t *p = rbtree_insert(t, key);

  rbtree_erase(t, p);
#ifdef SENTINEL
#else
#endif

  delete_rbtree(t);
}

void test_find_erase(rbtree *t, const key_t *arr, const size_t n) {
  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_insert(t, arr[i]);
  }

  // for (int i = 0; i < n; i++) {
  //   node_t *p = rbtree_find(t, arr[i]);
  //   printf("arr[%d] = %d\n", i, arr[i]);
    
    
  //   rbtree_erase(t, p);
  // }

  // for (int i = 0; i < n; i++) {
  //   node_t *p = rbtree_find(t, arr[i]);
    
  // }

  // for (int i = 0; i < n; i++) {
  //   node_t *p = rbtree_insert(t, arr[i]);
    
  //   node_t *q = rbtree_find(t, arr[i]);
    
    
    
  //   rbtree_erase(t, p);
  //   q = rbtree_find(t, arr[i]);
    
  // }
}
void test_find_erase_fixed() {
  const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(arr) / sizeof(arr[0]);
  rbtree *t = new_rbtree();
  

  test_find_erase(t, arr, n);

  delete_rbtree(t);
}

int main()
{
  // test_init();
  // test_insert_single(1024);
  // test_find_single(512, 1024);
  // test_erase_root(128);
  test_find_erase_fixed();
  // test_minmax_suite();
  // test_to_array_suite();
}