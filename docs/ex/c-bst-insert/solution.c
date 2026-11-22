// 参考实现：c-bst-insert
#include <stddef.h>
#include <stdlib.h>

typedef struct BstNode {
    int key;
    struct BstNode *left;
    struct BstNode *right;
} BstNode;

BstNode *bst_insert(BstNode *root, int key) {
    if (root == NULL) {
        BstNode *node = (BstNode *)malloc(sizeof *node);
        if (node == NULL) return NULL;
        node->key = key;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    if (key < root->key) {
        root->left = bst_insert(root->left, key);
    } else if (key > root->key) {
        root->right = bst_insert(root->right, key);
    }
    return root;
}

int bst_search(const BstNode *root, int key) {
    const BstNode *cur = root;
    while (cur != NULL) {
        if (key == cur->key) return 1;
        cur = key < cur->key ? cur->left : cur->right;
    }
    return 0;
}

size_t bst_count(const BstNode *root) {
    if (root == NULL) return 0;
    return 1 + bst_count(root->left) + bst_count(root->right);
}

static void inorder_walk(const BstNode *node, int *out, size_t cap, size_t *pos) {
    if (node == NULL || *pos >= cap) return;
    inorder_walk(node->left, out, cap, pos);
    if (*pos < cap) out[(*pos)++] = node->key;
    inorder_walk(node->right, out, cap, pos);
}

size_t bst_inorder(const BstNode *root, int *out, size_t cap) {
    size_t pos = 0;
    inorder_walk(root, out, cap, &pos);
    return pos;
}

void bst_free(BstNode *root) {
    if (root == NULL) return;
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}
