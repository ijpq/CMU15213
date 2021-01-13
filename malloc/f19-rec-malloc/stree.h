/*
 * Splay tree implementation
 * Based on code in https://en.wikipedia.org/wiki/Splay_tree
 *
 * Students are welcome to borrow and adapt this code for any
 * assignment in 15-213/18-213/15-513
 */

typedef long tkey_t;

typedef void (*free_fun_t)(void *r);

typedef struct node {
    struct node *left, *right;
    struct node *parent;
    tkey_t key;
    void *record;  // Points to user data */
} node_t;
    
typedef struct {
    node_t *root;
    size_t node_count;
    size_t comparison_count;
} tree_t;

tree_t *tree_new();

/* Delete all nodes in tree, applying free_fun to each record */
void tree_free(tree_t *tree, free_fun_t free_fun);

/* Insertion function returns false if already have key in tree */
bool tree_insert(tree_t *tree, tkey_t key, void *record);

void *tree_find(tree_t *tree, tkey_t key);

/* Find element with largest key <= given key */
void *tree_find_nearest(tree_t *tree, tkey_t key);

void *tree_remove(tree_t *tree, tkey_t key);

/* Print keys in tree */
void tree_show(tree_t *tree, bool tree_mode);


