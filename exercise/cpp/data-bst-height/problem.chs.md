这一题不读 stdin，也不写 stdout。实现一棵存储 `int` 的二叉搜索树，测试在隐藏的 harness 里。

实现：

```cpp
class IntBST {
public:
    IntBST();
    ~IntBST();
    IntBST(const IntBST&) = delete;
    IntBST& operator=(const IntBST&) = delete;

    bool insert(int value);   // 新值返回 true；已存在返回 false
    bool contains(int value) const;
    int size() const;
    int height() const;       // 空树为 0，只有根为 1
    void inorder(std::vector<int>& out) const;  // 追加升序结果
};
```

要求：

- 不允许重复值；
- `height()` 返回结点层数，空树为 0，只有一个根结点时为 1；
- `inorder` 把中序遍历结果追加到 `out` 末尾，不先清空它；
- 自行管理结点内存，析构时释放全部结点；不要写 `main`。

提示：递归插入和递归求高度都很自然。树可能退化成一条链（例如按升序插入），此时高度等于结点数，这是允许的。
