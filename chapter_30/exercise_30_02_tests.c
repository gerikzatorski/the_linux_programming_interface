/* exercise_30_02_tests.c */

/*********************************************************************
Some simple tests.

Functions prefixed with "test_" should return 1 if passed. They are
passed into a macro to print the function name and assert the return
value.
*********************************************************************/

#include <assert.h>
#include "exercise_30_02.h"

/* Printing errors */

static void thread_errno(int errnum, const char *funcname) {
  fprintf(stderr, "%s: %s\n", funcname, strerror(errnum));
}

/* Generic verification tool */

static int irecord; /* array of recorded keys */
static char **record; /* current index when recording */

static void record_bst_inorder(NodeBST *node) {
  if (node == NULL) {
    return;
  }
  record_bst_inorder(node->left);
  strcpy(record[irecord++], node->key);
  record_bst_inorder(node->right);
}

static int verify_bst_inorder(BinarySearchTree *tree, int num_results) {
  int i, res = 1;
  
  /* Allocate memory */

  record = calloc(num_results, sizeof(char*));
  for (i = 0; i < num_results; i++)
    record[i] = malloc(sizeof(char) * (KEY_MAX + 1));

  /* Build array of results inorder */

  irecord = 0;
  record_bst_inorder(tree->root);

  /* Test ordering */

  for (i = 1; i < num_results; i++)
    if (strcmp(record[i], record[i-1]) < 0)
        res = 0;

  /* Free memory */

  for (i = 0; i < num_results; i++)
    free(record[i]);
  free(record);
  
  return res;
}

/*
   Write keys of tree to stdout given the root.
*/
static void display_bst_inorder(const NodeBST *root) {
  if (root == NULL)
    return;

  display_bst_inorder(root->left);
  printf("%s %p\n", root->key, root);
  display_bst_inorder(root->right);

  return;
}

/* Threaded helpers */

/* Alphabet randomly sampled from for keys */
static const char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";

/*
   Add node with random key to the given tree.
*/
static void *add_thread(void *arg) {
  const int lenkey = 10;
  char k[KEY_MAX];
  int i, val = 1;

  for (i = 0; i < lenkey; i++)
    k[i] = chars[(int) ((rand() / ((double) RAND_MAX + 1)) * strlen(chars))];

  k[lenkey] = '\0';

  add((BinarySearchTree *) arg, k, &val);
}

/* test_ functions */

/*
   Add node to empty tree.
   Verify tree root contains new node with no children.
*/
bool test_add_root() {
  BinarySearchTree tree;
  char *key = "Person";
  int value = 4;

  initialize(&tree);

  add(&tree, key, &value);

  if (strcmp(tree.root->key, "Person") == 0 &&
      *((int *) tree.root->value) == 4 &&
      tree.root->left == NULL &&
      tree.root->right == NULL)
    return 1;
  else
    return 0;
}

/*
   Add five nodes.
   Verify ordering via inorder traversal.
*/
bool test_add_small() {
  BinarySearchTree tree;
  char *keys[5] = { "Jack", "Bobby", "Barbara", "Maya", "Val" };
  int values[5] = { 8, 3, 10, 1, 15 };

  initialize(&tree);

  add(&tree, keys[0], &values[0]);
  add(&tree, keys[1], &values[1]);
  add(&tree, keys[2], &values[2]);
  add(&tree, keys[3], &values[3]);
  add(&tree, keys[4], &values[4]);
    
  return verify_bst_inorder(&tree, 5);
}

/*
   Lookup all keys and verify values match.
   Also verify lookup() returns 0 (false) if key not present.
*/
bool test_lookup() {
  BinarySearchTree tree;
  char *keys[6] = { "M", "G", "X", "Q", "A", "J" };
  int values[6] = { 1, 2, 3, 4, 5, 6 };
  int i, v = 4;
  void *retval = &v;
  
  initialize(&tree);

  add(&tree, keys[0], &values[0]);
  add(&tree, keys[1], &values[1]);
  add(&tree, keys[2], &values[2]);
  add(&tree, keys[3], &values[3]);
  add(&tree, keys[4], &values[4]);
  add(&tree, keys[5], &values[5]);

  for (i = 0; i < 6; i++) {
    if (lookup(&tree, keys[i], &retval) == 0 ||
        (*((int *) retval) != values[i])) {
      return 0;
    }
  }
  
  if (lookup(&tree, "Z", &retval) == 1) /* Not in tree */
    return 0;
  
  return 1;
}

/*
   Delete root A.
   Verify tree is empty.
*/
bool test_delete_root() {
  BinarySearchTree tree;
  char *key = "A";
  int value = 4;

  initialize(&tree);

  add(&tree, key, &value);

  delete(&tree, "A");
  
  if (tree.root == NULL)
    return 1;
  else
    return 0;
}

/*
   A
    \
     B
      \
       C
 
   Delete C.
   Verify B has no children.
*/
bool test_delete_leaf() {
  BinarySearchTree tree;
  char *keys[3] = { "A", "B", "C" };
  int values[3] = { 1, 2, 3 };

  initialize(&tree);

  add(&tree, keys[0], &values[0]);
  add(&tree, keys[1], &values[1]);
  add(&tree, keys[2], &values[2]);

  delete(&tree, "C");

  if (strcmp(tree.root->right->key, "B") == 0 &&
      tree.root->right->right == NULL &&
      tree.root->right->left == NULL) {
    return 1;
  } else {
    return 0;
  }
}

/*
   A
    \
     B
      \
       C
 
   Delete Z.
   Verify delete() returns 0 (false).
*/
bool test_delete_false() {
  BinarySearchTree tree;
  char *keys[3] = { "A", "B", "C" };
  int values[3] = { 1, 2, 3 };

  initialize(&tree);

  add(&tree, keys[0], &values[0]);
  add(&tree, keys[1], &values[1]);
  add(&tree, keys[2], &values[2]);

  if (delete(&tree, "Z") == 0)
    return 1;
  else
    return 0;
}

/*
   A
    \
     B
      \
       C
 
   Delete B.
   Verify C takes its place and has no children.
*/
bool test_delete_single_parent() {
  BinarySearchTree tree;
  char *keys[3] = { "A", "B", "C" };
  int values[3] = { 1, 2, 3 };

  initialize(&tree);

  add(&tree, keys[0], &values[0]);
  add(&tree, keys[1], &values[1]);
  add(&tree, keys[2], &values[2]);

  delete(&tree, "B");

  if (strcmp(tree.root->right->key, "C") == 0 &&
      tree.root->right->right == NULL &&
      tree.root->right->left == NULL) {
    return 1;
  } else {
    return 0;
  }
}

/*
      M
     /
    G
   / \
   A J

   Delete root M.
   Verify bst inorder.
*/
bool test_delete_root_parent_single() {
  BinarySearchTree tree;
  char *keys[4] = { "M", "G", "A", "J" };
  int values[4] = { 1, 2, 3, 4 };

  initialize(&tree);

  add(&tree, keys[0], &values[0]);
  add(&tree, keys[1], &values[1]);
  add(&tree, keys[2], &values[2]);
  add(&tree, keys[3], &values[3]);

  delete(&tree, "M");
    
  return verify_bst_inorder(&tree, 3);
}

/*
      M
     / \
    G   X
   / \ /
   A J Q

   Delete root M.
   Verify bst inorder.
*/
bool test_delete_root_parent_double() {
  BinarySearchTree tree;
  char *keys[6] = { "M", "G", "X", "Q", "A", "J" };
  int values[6] = { 1, 2, 3, 4, 5, 6 };

  initialize(&tree);

  add(&tree, keys[0], &values[0]);
  add(&tree, keys[1], &values[1]);
  add(&tree, keys[2], &values[2]);
  add(&tree, keys[3], &values[3]);
  add(&tree, keys[4], &values[4]);
  add(&tree, keys[5], &values[5]);

  delete(&tree, "M");
  return verify_bst_inorder(&tree, 5);
}

/*
   Create many nodes via threads.
   Verify bst inorder.
*/
bool test_threaded_add() {
  const int nnodes = 100; /* number of nodes / threads */

  int i, s;
  BinarySearchTree tree;
  pthread_t threads[nnodes];
  
  initialize(&tree);
  
  for (i = 0; i < nnodes; i++) {
    s = pthread_create(&threads[i], NULL, add_thread, &tree);
    if (s != 0) {
      thread_errno(s, "pthread_create");
      exit(EXIT_FAILURE);
    }
  }
  
  for (i = 0; i < nnodes; i++) {
    pthread_join(threads[i], NULL);
  }
  
  return verify_bst_inorder(&tree, nnodes);
}

/*
   Macro makes for nicer stdout
*/
#define RUN_TEST(f, s) {           \
  printf("Running %s ...\n", s);   \
  assert(f());                     \
}

int main(int argc, char *argv[]) {
  srand(0);

  RUN_TEST(test_add_root, "test_add_root");
  RUN_TEST(test_add_small, "test_add_small");
  RUN_TEST(test_lookup, "test_lookup");
  RUN_TEST(test_delete_root, "test_delete_root");
  RUN_TEST(test_delete_false, "test_delete_false");
  RUN_TEST(test_delete_leaf, "test_delete_leaf");
  RUN_TEST(test_delete_single_parent, "test_delete_single_parent");
  RUN_TEST(test_delete_root_parent_single, "test_delete_root_parent_single");
  RUN_TEST(test_delete_root_parent_double, "test_delete_root_parent_double");

  RUN_TEST(test_threaded_add, "test_threaded_add");
  
  exit(EXIT_SUCCESS);
}
