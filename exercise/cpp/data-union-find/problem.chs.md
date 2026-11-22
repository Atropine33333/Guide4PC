这一题不读 stdin，也不写 stdout。实现一个并查集，测试在隐藏的 harness 里。

实现：

```cpp
class DisjointSet {
public:
    explicit DisjointSet(int n);   // 元素编号 0..n-1，n 可以为 0
    int find(int x);               // 返回 x 所在组的代表元
    bool unite(int a, int b);      // 合并成功返回 true，本来同组返回 false
    bool connected(int a, int b);
    int components() const;        // 当前组数
};
```

要求：

- 初始时每个元素各自成组，组数为 `n`；
- `unite` 只在两组确实不同时减少组数；
- 调用参数保证在 `[0, n)` 范围内；
- 建议使用路径压缩和按秩合并，但正确性是第一位的。

不要写 `main`。
