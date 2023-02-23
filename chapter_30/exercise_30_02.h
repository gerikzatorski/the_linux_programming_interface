#ifndef EXERCISE_30_02_H
#define EXERCISE_30_02_H

#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>

#define KEY_MAX 127

typedef struct binary_search_tree BinarySearchTree;
typedef struct node_bst NodeBST;

struct binary_search_tree {
  NodeBST *root;
  pthread_mutex_t *mtx;
};

struct node_bst {
  char *key;
  void *value;
  NodeBST *left;
  NodeBST *right;
};

/* Library API */

int initialize(BinarySearchTree *tree);
int add(BinarySearchTree *tree, char *key, void *value);
int delete (BinarySearchTree *root, char *key);
bool lookup(BinarySearchTree *tree, char *key, void **value);

#endif /* EXERCISE_30_02_H */
