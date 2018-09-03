#ifndef TREE_H_
#define TREE_H_

#include "mylib.h"

typedef struct tree_node *tree;
typedef enum tree_e { BST, RBT } tree_t;
typedef enum colour_e {RED,BLACK} colour_t;

extern tree   tree_delete(tree b, char *str);
extern tree   tree_free(tree b);
extern void   tree_inorder(tree b, void f(int freq, char *str));
extern tree   tree_insert(tree b, char *str);
extern tree   tree_new(tree_t type);
extern void   tree_preorder(tree b, void f(char *str));
extern int    tree_search(tree b, char *str);

#endif
