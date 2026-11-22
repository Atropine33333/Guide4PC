《有序树使用说明书》

一、用途：本产品保存一组整数，每个节点左子树里的键都更小，右子树里的键都更大，查找时逐层比较，一步向左或向右。

二、安装：调用 `root = bst_insert(root, key);` 并保存返回值。树为空时它会建出根节点；键已存在时只保留一份，节点数不变；内存不足时返回原根。

三、查询：`bst_search(root, key)`，命中返回 `1`，未命中返回 `0`。空树一律返回 `0`。

四、清点：`bst_count(root)` 返回节点总数，空树返回 `0`。

五、导出：`bst_inorder(root, out, cap)` 按从小到大的顺序把键写进 `out`，最多写 `cap` 个，返回实际写入的个数；`cap` 不够时只导出前 `cap` 个。

六、卸载：`bst_free(root)` 释放全部节点；传入 `NULL` 也不出错。释放后请把根指针置为 `NULL`。

**人话**：实现一棵二叉查找树——插入、查找、统计节点数、中序导出、整棵释放，五个函数各司其职。

**接口**：

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

### 示例

依次插入 `50`、`30`、`70`、`20`、`40`、`60`、`80`：`bst_count` 为 `7`；`bst_inorder` 写出的顺序是 `20 30 40 50 60 70 80`；再插入一次 `50`，节点数仍是 `7`。

### 约定

- 只实现这些函数，不要写 `main`。
- 节点用 `malloc` 分配，不要用固定大小的数组来假装树。
- 重复的键不新建节点；空树上的任何查询都不能崩溃。
- 结构体保持题面给出的字段，函数签名不变。
