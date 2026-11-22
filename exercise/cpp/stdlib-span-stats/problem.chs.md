这一题不读 stdin。实现对一段连续内存的只读统计与裁剪。

```cpp
#include <cstddef>
#include <span>
#include <vector>

struct Stats {
    std::size_t count;
    long long sum;
    int min;
    int max;
};

Stats summarize(std::span<const int> values);
std::span<const int> trim_zeros(std::span<const int> values);
```

约定：

- `summarize` 统计元素个数、总和（用 `long long` 累加，避免溢出）、最小值与最大值。
- 空输入返回 `{0, 0, 0, 0}`。
- `trim_zeros` 去掉开头和结尾的连续零，返回原内存上的子视图；全是零或空输入时返回空视图。
- 返回的视图不能自己拥有数据，也不能指向新建的 `std::vector`。
- 不要写 `main`。

### 提示

`std::span` 的 `subspan(offset, count)` 返回的仍然是指向原缓冲区的视图。判断别名时，harness 会比较 `data()` 指针位置。
