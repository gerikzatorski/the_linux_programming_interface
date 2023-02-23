/* exercise_30_02.c */

/*********************************************************************
Implement a set of thread-safe functions that update and search an
unbalanced binary tree. This library should include functions (with
the obvious purposes) of the following form:

initialize(tree);
add(tree, char *key, void *value);
delete(tree, char *key)
Boolean lookup(char *key, void **value)

In the above prototypes, tree is a structure that points to the root
of the tree (you will need to define a suitable structure for this
purpose). Each element of the tree holds a key-value pair. You will
also need to define the structure for each element to include a mutex
that protects that element so that only one thread at a time can
access it. The initialize(), add(), and lookup() functions are
relatively simple to imple- ment. The delete() operation requires a
little more effort.

TODO: More granular parallelism. Currently implemented coarse mutex on
BinarySearchTree struct. See add(), delete(), and lookup().
*********************************************************************/

#include "exercise_30_02.h"

/* Static prototypes */

static void add_by_node(NodeBST *root, NodeBST *new_node);
static int delete_by_node(BinarySearchTree *tree,
                          NodeBST *root,
                          char *key,
                          NodeBST *parent);
static bool lookup_by_node(const NodeBST *root,
                           const char *key,
                           void **value);
static int max_descendent(NodeBST *root,
                          NodeBST **max_addr,
                          NodeBST **max_parent_addr);
static void free_node(NodeBST *node);

/* Library API  */

int initialize(BinarySearchTree *tree) {
  int s;
  
  tree->root = NULL;
  tree->mtx = malloc(sizeof(pthread_mutex_t));
  if (tree->mtx == NULL) {
    free(tree);
    return -1;
  }

  s = pthread_mutex_init(tree->mtx, NULL);
  if (s != 0)
    return s;
  
  return 0;
}

int add(BinarySearchTree *tree, char *key, void *value) {
  NodeBST *new_node;
  
  /* Allocate memory for node and its members */

  new_node = malloc(sizeof(NodeBST));
  if (new_node == NULL) {
    return -1;
  }
  new_node->key = malloc(sizeof(char) * (KEY_MAX + 1));
  if (new_node->key == NULL) {
    free(new_node);
    return -1;
  }

  /* Set member values */

  strcpy(new_node->key, key);
  new_node->value = value;
  new_node->left = NULL;
  new_node->right = NULL;

  /* Empty tree simply designates root */

  if (tree->root == NULL) {
    tree->root = new_node;
    return 0;
  }

  pthread_mutex_lock(tree->mtx);
  add_by_node(tree->root, new_node);
  pthread_mutex_unlock(tree->mtx);

  return 0;
}

int delete(BinarySearchTree *tree, char *key) {
  int res;
  pthread_mutex_lock(tree->mtx);
  res = delete_by_node(tree, tree->root, key, NULL);
  pthread_mutex_unlock(tree->mtx);
  return res;
}

bool lookup(BinarySearchTree *tree, char *key, void **value) {
  bool res;
  pthread_mutex_lock(tree->mtx);
  res = lookup_by_node(tree->root, key, value);
  pthread_mutex_unlock(tree->mtx);
  return res;
}

/* Helpers */

/*
   Recursive function to insert new node below root.
*/
static void add_by_node(NodeBST *root, NodeBST *new_node) {
  /* Assumed that root is not NULL */

  if (strcmp(new_node->key, root->key) < 0) {
    if (root->left)
      add_by_node(root->left, new_node);
    else
      root->left = new_node;
  } else {
    if (root->right)
      add_by_node(root->right, new_node);
    else
      root->right = new_node;
  }
    
  return;
}

/*
   Recursive function to delete descendent of node with given key.
   Returns 0 if node is NULL, 1 on success.
*/
static int delete_by_node(BinarySearchTree *tree,
                          NodeBST *root,
                          char *key,
                          NodeBST *parent) {
  int s;

  if (root == NULL) {
    return 0;
  }

  /* Recursive paths left and right */

  if (strcmp(key, root->key) < 0)
    return delete_by_node(tree, root->left, key, root);
  if (strcmp(key, root->key) > 0)
    return delete_by_node(tree, root->right, key, root);

  if (root->left == NULL && root->right == NULL) {  /* No children */

    /* Redirect parent node (or tree struct) */

    if (parent == NULL)
      tree->root = NULL;
    else
      if (parent->left == root)
        parent->left = NULL;
      else
        parent->right = NULL;

    /* Delete the leaf node */

    free_node(root);

  } else if (root->left != NULL &&
             root->right != NULL) {                 /* Two children */

    NodeBST **interest_addr = malloc(sizeof(void *));
    NodeBST **interest_parent_addr = malloc(sizeof(void *));

    /* Copy key and value of max descendent in left subtree */

    max_descendent(root->left, interest_addr, interest_parent_addr);
    strcpy(root->key, (*interest_addr)->key);
    memcpy(root->value, (*interest_addr)->value, sizeof(*((*interest_addr)->value)));
    
    /* Delete same descendent and redirect it's parent to NULL */    

    (*interest_parent_addr)->right = NULL;
    free_node(*interest_addr);
    free(interest_addr);
    free(interest_parent_addr);

  } else {                                          /* One child */

    NodeBST **child_addr;

    /* Replace node with child node */
    
    if (root->right != NULL)
      child_addr = &(root->right);
    else
      child_addr = &(root->left);
    
    free_node(root);

    /* Redirect parent of deleted node */

    if (parent == NULL) {
      tree->root = *child_addr;
    } else if (root == parent->right) {
      parent->right = *child_addr;
    } else {
      parent->left = *child_addr;
    }
  }
  return 1;
}

/*
   Recursive function to find a descendent of node with given key.
   Return true if found and modify value of argument value.
   Return false if not found.
*/
static bool lookup_by_node(const NodeBST *root,
                           const char *key,
                           void **value) {
  if (root == NULL)
    return false;

  /* Recursive paths left and right */

  if (strcmp(key, root->key) < 0)
    return lookup_by_node(root->left, key, value);
  if (strcmp(key, root->key) > 0)
    return lookup_by_node(root->right, key, value);

  (*value) = root->value;
  return true;

}

/*
   Find the descendent of root with the largest key.
   On success, return 0 and store the descendent's address in max_addr
   and it's parent in max_parent_addr.
   On failure, return 1.
*/
static int max_descendent(NodeBST *root,
                          NodeBST **max_addr,
                          NodeBST **max_parent_addr) {
  if (root == NULL)
    return 1;
  
  for (;;) {
    if (root->right == NULL) {
      *max_addr = root;
      return 0;
    }
    *max_parent_addr = root;
    root = root->right;
  }
}

/*
   Deallocate memory for given node and it's members.
*/
static void free_node(NodeBST *node) {
  free(node->key);
  free(node);
}
