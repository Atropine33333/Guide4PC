欢迎来到左旋走廊。

这里每一条规矩都是真的。走廊不骗人，它只在你递进一个负数时，换一只更大的手来接。

规矩一：走廊里的 `size_t` 没有负号。你把 `-1` 递过去，它回你一个一万八千多京的数。取模之后，那个数落在哪里，全看运气。

规矩二：要让负数过去，先把它送回正的一边。取模之前，或者取模之后，把 `k` 调进 `0` 到 `n - 1` 之间；走廊认这个范围。

规矩三：长度为 0 的数组是一扇锁着的门。任何位移都不发生，门后也没有东西可以少。

规矩四：长度为 1 的数组，门里门外是同一个人。位移多少次，还是他。

规矩五：走廊的回声只认 `values`。函数返回之时，调用者手上那份数组必须已经转好。

规矩六：门从外面开。不要写 `main`，也不要改函数签名。

如果你发现自己开始计算“一万八千多京对五取模是多少”，请放下笔，先回到规矩二。

**人话**：补完 `rotate_left(values, k)`：`k` 为正时左移，为负时右移，`|k|` 可以很大。先把位移归到 `[0, n)`：对 `n` 取模之后若为负，再补一个 `n`。数组为空或只有一个元素时原样返回。starter 直接把 `k` 转成 `size_t` 再取模，负数在这一步变成天文数字；修好这段取模，不写 `main`。

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

**要求**：

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
