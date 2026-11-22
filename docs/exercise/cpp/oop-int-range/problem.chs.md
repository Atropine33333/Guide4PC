这一题不读 stdin，也不写 stdout。实现一个可用于范围 `for` 和标准算法的整数区间。

```cpp
#include <cstddef>
#include <iterator>

class IntRange {
public:
    class iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = const int*;
        using reference = int;

        iterator();
        explicit iterator(int value);

        int operator*() const;
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
    };

    IntRange(int first, int stop);
    iterator begin() const;
    iterator end() const;
};
```

规则：

- 区间包含从 `first` 开始、到 `stop` 之前（不包含 `stop`）的所有整数，步长为 1。
- 当 `first >= stop` 时区间为空，`begin() == end()`。
- 支持范围 `for`、`std::distance`、`std::accumulate` 和用迭代器构造 `std::vector`。
- 迭代器必须支持前置和后置 `++`、解引用、`==` 和 `!=`。
- 区间对象可以按值拷贝，迭代过程不能修改区间本身。
- 不要写 `main`，不要修改公开接口。
