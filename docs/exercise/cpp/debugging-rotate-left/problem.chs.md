下面的函数本应将 `values` 循环左移 `k` 次：`k` 为正时往左移，为负时往右移，`|k|` 可以很大。但它在某些输入下结果是错的。请修好它。

```cpp
void rotate_left(std::vector<int>& values, int k) {
    const std::size_t n = values.size();
    if (n == 0) return;
    const std::size_t shift = static_cast<std::size_t>(k) % n;
    std::vector<int> rotated(values.begin() + static_cast<std::ptrdiff_t>(shift), values.end());
    rotated.insert(rotated.end(), values.begin(),
                   values.begin() + static_cast<std::ptrdiff_t>(shift));
    values = std::move(rotated);
}
```

要求：

- `{1,2,3,4,5}` 左移 2 次得到 `{3,4,5,1,2}`；
- `k` 为 `-1` 时相当于右移 1 次；
- 位移次数要先对数组长度取模；
- 空数组任何位移都不改变；单元素数组任何位移都不改变。

评测代码会直接包含你的实现并检查这些边界。**不要写 `main`。**

### 输入

无。

### 输出

无。

### 示例

```cpp
std::vector<int> a{1, 2, 3, 4, 5};
rotate_left(a, 2);   // {3, 4, 5, 1, 2}
rotate_left(a, -1);  // {2, 3, 4, 5, 1}
```

### 约束

- 数组长度不超过 `2 * 10^5`。
- `k` 在 `int` 范围内；不要用 `long long` 之外的假设。
