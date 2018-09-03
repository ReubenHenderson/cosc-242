#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "tree.h"

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

static tree_t tree_type;

struct tree_node {
    colour_t colour;
    int frequency;
    char *key;
    tree left;
    tree right;
};




/**
 * Traverses the tree writing a DOT description about connections, and
 * possibly colours, to the given output stream.
 *
 * @param t the tree to output a DOT description of.
 * @param out the stream to write the DOT output to.
 */
static void tree_output_dot_aux(tree t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->frequency,
                (RBT == tree_type && RED == t->colour) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}



/**
 * Output a DOT description of this tree to the given output stream.
 * DOT is a plain text graph description language (see www.graphviz.org).
 * You can create a viewable graph with the command
 *
 *    dot -Tpdf < graphfile.dot > graphfile.pdf
 *
 * You can also use png, ps, jpg, svg... instead of pdf
 *
 * @param t the tree to output the DOT description of.
 * @param out the stream to write the DOT description to.
 */
void tree_output_dot(tree t, FILE *out) {
    fprintf(out, "digraph tree {\nnode [shape = Mrecord, penwidth = 2];\n");
    tree_output_dot_aux(t, out);
    fprintf(out, "}\n");
}



static tree left_rotate(tree b){
    tree r = b->right;
    tree temp = b;
    temp->right = r->left;
    r->left = temp;
    return b;
}



static tree right_rotate(tree b){
    tree r = b->left;
    tree temp = b;
    temp->left = r->right;
    r->right = temp;
    return b;
}



static tree tree_fix(tree b){
    if(IS_RED(b->left) && IS_RED(b->left->left)){
        if(IS_RED(b->right)){
            b->colour = RED;
            b->right->colour = BLACK;
            b->left->colour = BLACK;
        }
        else{
            b = right_rotate(b);
            b->colour = BLACK;
            b->right->colour = RED;
        }
    }
    
    else if (IS_RED(b->left) && IS_RED(b->left->right)){
        if(IS_RED(b->right)){
            b->colour = RED;
            b->right->colour = BLACK;
            b->left->colour = BLACK;
        }
        else{
            b->left = left_rotate(b->left);
            b = right_rotate(b);
            b->colour = BLACK;
            b->right->colour = RED;
        }
    }
    
    else if(IS_RED(b->right) && IS_RED(b->right->left)){
        if(IS_RED(b->left)){
            b->colour = RED;
            b->left->colour = BLACK;
            b->right->colour = BLACK;
        }
        else{
            b->right = right_rotate(b->right);
            b = left_rotate(b);
            b->colour = BLACK;
            b->left->colour = RED;
        }
    }
    
    else if(IS_RED(b->right) && IS_RED(b->right->right)){
        if(IS_RED(b->left)){
            b->colour = RED;
            b->left->colour = BLACK;
            b->right->colour = BLACK;
        }
        else {
            b = left_rotate(b);
            b->colour = BLACK;
            b->left->colour = RED;
        }
    }
    
    return b;
}




tree tree_new(tree_t type){
    tree_type = type;
    return NULL;
}



int tree_search(tree b, char *key){
    if(NULL == b){
        return 0;
    } else if(strcmp(b->key, key) == 0){
        return 1;
    } else if(strcmp(b->key, key) > 0){
        return tree_search(b->left, key);
    } else {
        return tree_search(b->right, key);
    }
}



tree tree_insert(tree b ,char *key){
    if(NULL == b){
        b = emalloc(sizeof *b);
        b->key = emalloc(strlen(key) * sizeof key[0] + 1);
        strcpy(b->key, key);
        
    } else if(strcmp(b->key, key) == 0){
        b->frequency++;
    }

    else if(strcmp(b->key, key) > 0){
        b->left = tree_insert(b->left, key);
    }

    else if(strcmp(b->key, key) < 0) {
        b->right = tree_insert(b->right, key);
    }
    
    if(tree_type == RBT){
        b = tree_fix(b);
    }
    return b;
}



void tree_inorder(tree b, void f(int freq, char *s)) {
    if(NULL == b)
        { return;
        } /* stopping case */
    tree_inorder(b->left, f);/*inorder traverse the left subtree*/
    f(b->frequency, b->key);/* apply f to b->key*/
    tree_inorder(b->right, f);/* inorder traverse the right subtree*/
}



void tree_preorder(tree b, void f(char *s)) {
    if(NULL == b)
        { return;
        } /* stopping case */
    f(b->key);/*apply f to b->key*/
    tree_preorder(b->left, f);/*preorder traverse the left subtree*/
    tree_preorder(b->right, f);/*preorder traverse the right subtree*/
}



/*tree tree_delete(tree b, char *str){
    tree temp;
 
    if(NULL == b){
        return b;
    }

    else if(strcmp(b->key, str)  == 0){
        if(b->left == NULL && b->right == NULL){
            free(b->key);
            free(b);
            return NULL;
        } else if(b->left != NULL && b->right == NULL){
            free(b->key);
            free(b);
            return b->left;
        } else if(b->left == NULL && b->right != NULL){
            free(b->key);
            free(b);
            return b->right;
            
        } else {
            temp = b->right;
            while(temp->left != NULL){
                temp = temp->left;
            }
            strcpy(b->key, temp->key);
            b->right = tree_delete(b->right, temp->key);
            return b;
        }
    }

    else if(strcmp(b->key, str) > 0){
        b->left = tree_delete(b->left, str);
    }

    else if(strcmp(b->key, str) < 0){
        b->right = tree_delete(b->right, str);
    }
    
    return b;
    }*/



tree tree_free(tree b){
    if(NULL == b){
        return b;
    } else {
        tree_free(b->left);
        tree_free(b->right);
        free(b->key);
        free(b);
        return b;
    }
}
