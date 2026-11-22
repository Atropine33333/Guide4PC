耶和华晓谕摩西说：“你要数点会众，从 `first` 起，直到 `stop` 以先；凡在这中间的，一个也不可漏下。到了 `stop`，就停住，不可越过。

“数点的次序不可更改，每次只向前行一步。会众若本就是从大到小站的，或起数已过了尽头，那一次便算是空的，不可强数。

“数点的人可以各拿各的册子，分行分列地数；册子彼此不相扰，也不可因数点而移动会众的营盘。”

摩西便照耶和华所吩咐的行了。

**人话**：实现整数区间类 `IntRange`，配一个输入迭代器：区间含从 `first` 开始、到 `stop` 之前（不含 `stop`）的每个整数，步长为 1；`first >= stop` 时区间为空、`begin() == end()`。它要能用于范围 `for`、`std::distance`、`std::accumulate`，也能用迭代器构造 `std::vector`；迭代器支持前置与后置 `++`、解引用、`==` 和 `!=`，并按值拷贝。

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

### 规则

- 区间包含从 `first` 开始、到 `stop` 之前（不包含 `stop`）的所有整数，步长为 1。
- 当 `first >= stop` 时区间为空，`begin() == end()`。
- 支持范围 `for`、`std::distance`、`std::accumulate` 和用迭代器构造 `std::vector`。
- 迭代器必须支持前置和后置 `++`、解引用、`==` 和 `!=`。
- 区间对象可以按值拷贝，迭代过程不能修改区间本身。
- 不要写 `main`，不要修改公开接口。
