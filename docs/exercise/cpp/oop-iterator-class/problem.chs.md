实现一个支持 `range-for` 的整数区间迭代器。

```cpp
#include <iterator>

class IntRange {
public:
    IntRange(int start, int stop, int step = 1);

    class iterator {
    public:
        int operator*() const;
        iterator& operator++();
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
    };

    iterator begin() const;
    iterator end() const;
};
```

约定：

- 区间从 `start` 开始，每次加 `step`，生成严格小于 `stop` 的数；`step` 为负数时生成严格大于 `stop` 的数。
- 区间为空时，`begin()` 与 `end()` 相等，`range-for` 一次也不执行。
- 迭代器只需要支持 `range-for` 用到的四个操作：`*it`、`++it`、`it != end`、`it == end`。
- 迭代器按值拷贝；复制迭代器不应影响原迭代器。
- 当 `step` 不能整除 `stop - start` 时，越过 `stop` 就停止，不能因为永远碰不到 `stop` 而无限循环。
- 不要写 `main`。

### 示例

`IntRange(1, 10, 3)` 依次产生 `1, 4, 7`；`IntRange(5, 0, -2)` 依次产生 `5, 3, 1`。

### 提示

迭代器可以保存当前值、步长和 `stop`，并在内部记录一个 `done` 标志；`end()` 返回 `done` 为真的迭代器。这样即使步长不能整除区间长度，`!=` 也会在越界后变为假。
