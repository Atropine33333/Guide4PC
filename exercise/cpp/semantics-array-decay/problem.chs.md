这一题不读 stdin，也不写 stdout。补全下面五个函数；隐藏的 harness 会检查它们。

```cpp
#include <cstddef>
#include <string>

std::size_t array_bytes(const int (&values)[6]);
std::size_t pointer_bytes(const int* values);
std::string which_overload(int (&values)[6]);
std::string which_overload(const int* values);
int sum_array(const int (&values)[6]);
int sum_range(const int* first, const int* last);
```

要点：

- `array_bytes` 的形参是对长度为 6 的 `int` 数组的引用，因此 `sizeof(values)` 得到整个数组的字节数，应直接返回它。
- `pointer_bytes` 的形参是指针，数组传入时会退化；`sizeof(values)` 得到的是指针本身的大小。
- 两个 `which_overload` 构成重载。数组实参（包括长度为 6 的具名数组）选择数组引用版本并返回 `"array"`；指针实参（如 `values + 0`、`&values[0]`）选择指针版本并返回 `"pointer"`。
- `sum_array` 返回 6 个元素之和。
- `sum_range` 返回 `[first, last)` 区间内元素之和；当 `first == last` 时返回 0。
- 不要写 `main`，也不要修改函数签名。
