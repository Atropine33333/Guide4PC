#include <stddef.h>
#include <stdlib.h>

typedef struct BstNode {
    int key;
    struct BstNode *left;
    struct BstNode *right;
} BstNode;

BstNode *bst_insert(BstNode *root, int key);
int bst_search(const BstNode *root, int key);
size_t bst_count(const BstNode *root);
size_t bst_inorder(const BstNode *root, int *out, size_t cap);
void bst_free(BstNode *root);

BstNode *bst_insert(BstNode *root, int key) {
    /* TODO: 小于根走左边，大于根走右边，重复键忽略。 */
    (void)key;
    return root;
}

int bst_search(const BstNode *root, int key) {
    /* TODO: 找到返回 1，找不到返回 0。 */
    (void)root;
    (void)key;
    return 0;
}

size_t bst_count(const BstNode *root) {
    /* TODO */
    (void)root;
    return 0;
}

size_t bst_inorder(const BstNode *root, int *out, size_t cap) {
    /* TODO: 中序遍历，最多写 cap 个，返回写入个数。 */
    (void)root;
    (void)out;
    (void)cap;
    return 0;
}

void bst_free(BstNode *root) {
    /* TODO: 后序释放全部节点。 */
    (void)root;
}
