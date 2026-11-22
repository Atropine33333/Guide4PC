INSTRUCTIONS FOR THE ORDERED TREE

1. Purpose. This product stores a set of integers. Every key in a node's left subtree is smaller than the node's key, and every key in its right subtree is larger, so a lookup compares and then steps left or right.

2. Installation. Call `root = bst_insert(root, key);` and keep the returned value. It creates the root when the tree is empty; a key that is already present is kept once and the node count does not change; if memory runs out, the original root is returned.

3. Lookup. `bst_search(root, key)` returns `1` on a hit and `0` on a miss. An empty tree always returns `0`.

4. Counting. `bst_count(root)` returns the total number of nodes, or `0` for an empty tree.

5. Export. `bst_inorder(root, out, cap)` writes the keys into `out` from smallest to largest, at most `cap` of them, and returns how many were written; when `cap` is smaller than the tree, only the first `cap` keys are exported.

6. Uninstallation. `bst_free(root)` releases every node; passing `NULL` is harmless. Set the root pointer to `NULL` afterwards.

**In plain words**: implement a binary search tree — insert, search, count the nodes, export in order, and free the whole tree, one job per function.

**Interface**:

```c
#include <stddef.h>

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
```

### Example

Insert `50`, `30`, `70`, `20`, `40`, `60`, `80` in that order: `bst_count` is `7`; `bst_inorder` writes `20 30 40 50 60 70 80`; inserting `50` again leaves the count at `7`.

### Contract

- Implement only these functions; do not write `main`.
- Nodes come from `malloc`; a fixed-size array dressed up as a tree is not acceptable.
- A duplicate key creates no new node; no query on an empty tree may crash.
- Keep the struct fields as given and the function signatures unchanged.
