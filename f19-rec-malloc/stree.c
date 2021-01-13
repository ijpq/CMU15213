/*
 * Splay tree implementation
 * Based on code in https://en.wikipedia.org/wiki/Splay_tree
 *
 * Students are welcome to borrow and adapt this code for any
 * assignment in 15-213/18-213/15-513
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "stree.h"
  
static void free_subtree(node_t *x, free_fun_t free_fun);
static void left_rotate(tree_t *tree, node_t *x);
static void right_rotate(tree_t *tree, node_t *x);
static void splay(tree_t *tree, node_t *x);
static void replace(tree_t *tree, node_t *u, node_t *v);
static node_t *subtree_minimum(node_t *u);
static node_t *subtree_maximum(node_t *u);
static void show_subtree(node_t *x, bool tree_mode);

tree_t *tree_new() {
    tree_t *tree = malloc(sizeof(tree_t));
    if (!tree) {
	fprintf(stderr, "ERROR.  Couldn't create range tree\n");
	exit(1);
    }
    tree->root = NULL;
    tree->node_count = 0;
    tree->comparison_count = 0;
    return tree;
}

void tree_free(tree_t *tree, free_fun_t free_fun) {
    if (tree->root)
	free_subtree(tree->root, free_fun);
    free(tree);
}

bool tree_insert(tree_t *tree, tkey_t key, void *record) {
    node_t *z = tree->root;
    node_t *p = NULL;
    
    while (z) {
	p = z;
	tree->comparison_count++;
	if (key == z->key)
	    /* Already have key in tree */
	    return false;
	tree->comparison_count++;
	if (key > z->key)
	    z = z->right;
	else
	    z = z->left;
    }
    
    z = malloc(sizeof(node_t));
    if (!z) {
	fprintf(stderr, "ERROR.  Couldn't create range tree node\n");
	exit(1);
    }
    z->key = key;
    z->record = record;
    z->parent = p;
    z->left = z->right = NULL;
    if (!p) tree->root = z;
    else if (p->key < z->key) p->right = z;
    else p->left = z;
    splay(tree, z);
    tree->node_count++;
    return true;
}
  
void *tree_find(tree_t *tree, tkey_t key) {
    node_t *z = tree->root;
    while (z) {
	tree->comparison_count++;
	if (key == z->key)
	    return z->record;
	tree->comparison_count++;
	if (key > z->key)
	    z = z->right;
	else
	    z = z->left;
    }
    return NULL;
}

void *tree_find_nearest(tree_t *tree, tkey_t key) {
    node_t *z = tree->root;
    node_t *n = NULL;
    while (z) {
	tree->comparison_count++;
	if (key == z->key)
	    return z->record;
	tree->comparison_count++;
	if (key > z->key) {
	    if (!n || n->key < z->key)
		n = z;
	    z = z->right;
	}
	else
	    z = z->left;
    }
    return n ? n->record : NULL;
}

        
void *tree_remove(tree_t *tree, tkey_t key) {
    node_t *z = tree->root;
    void *r = NULL;
    while (z && z->key != key) {
	tree->comparison_count++;
	if (key > z->key)
	    z = z->right;
	else
	    z = z->left;
    }
    if (!z)
	return r;
    splay(tree, z);
    if (!z->left) replace(tree, z, z->right);
    else if (!z->right) replace(tree, z, z->left);
    else {
	node_t *y = subtree_minimum(z->right);
	if (y->parent != z) {
	    replace(tree, y, y->right);
	    y->right = z->right;
	    y->right->parent = y;
	}
	replace(tree, z, y);
	y->left = z->left;
	y->left->parent = y;
    }
    r = z->record;
    tree->node_count--;
    free(z);
    return r;
}

void tree_show(tree_t *tree, bool tree_mode) {
    if (tree) {
	printf("[");
	show_subtree(tree->root, tree_mode);
	printf("] %ld nodes, %ld comparisons\n", tree->node_count, tree->comparison_count);
    } else {
	printf("NULL\n");
    }
}

/*** Helper functions ***/

static void free_subtree(node_t *x, free_fun_t free_fun) {
    if (!x)
	return;
    free_subtree(x->left, free_fun);
    free_subtree(x->right, free_fun);
    if (free_fun)
	free_fun(x->record);
    free(x);
}

static void left_rotate(tree_t *tree, node_t *x) {
    node_t *y = x->right;
    if (y) {
	x->right = y->left;
	if (y->left) y->left->parent = x;
	y->parent = x->parent;
    }
    if (!x->parent) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    if (y) y->left = x;
    x->parent = y;
}
  
static void right_rotate(tree_t *tree, node_t *x) {
    node_t *y = x->left;
    if (y) {
	x->left = y->right;
	if (y->right) y->right->parent = x;
	y->parent = x->parent;
    }
    if (!x->parent) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    if (y) y->right = x;
    x->parent = y;
}
  
static void splay(tree_t *tree, node_t *x) {
    while (x->parent) {
	if (!x->parent->parent) {
	    if (x->parent->left == x) right_rotate(tree, x->parent);
	    else left_rotate(tree, x->parent);
	} else if (x->parent->left == x && x->parent->parent->left == x->parent) {
	    right_rotate(tree, x->parent->parent);
	    right_rotate(tree, x->parent);
	} else if (x->parent->right == x && x->parent->parent->right == x->parent) {
	    left_rotate(tree, x->parent->parent);
	    left_rotate(tree, x->parent);
	} else if (x->parent->left == x && x->parent->parent->right == x->parent) {
	    right_rotate(tree, x->parent);
	    left_rotate(tree, x->parent);
	} else {
	    left_rotate(tree, x->parent);
	    right_rotate(tree, x->parent);
	}
    }
}
  
static void replace(tree_t *tree, node_t *u, node_t *v) {
    if (!u->parent) tree->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v) v->parent = u->parent;
}
  
static node_t* subtree_minimum(node_t *u) {
    while (u->left) u = u->left;
    return u;
}
  
static node_t* subtree_maximum(node_t *u) {
    while (u->right) u = u->right;
    return u;
}

static void show_subtree(node_t *x, bool tree_mode) {
    if (!x)
	return;
    if (tree_mode)
	printf("(");
    show_subtree(x->left, tree_mode);
    printf(" %ld ", x->key);
    show_subtree(x->right, tree_mode);
    if (tree_mode)
	printf(")");
}
