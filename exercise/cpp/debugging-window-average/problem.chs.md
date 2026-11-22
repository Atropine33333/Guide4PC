这一题不读 stdin，也不写 stdout。实现下面的函数：

```cpp
#include <cstddef>
#include <vector>

std::vector<double> window_average(const std::vector<int>& values,
                                   std::size_t window);
```

它返回所有长度为 `window` 的连续窗口的平均值，按窗口起点从左到右排列。只统计完整窗口：如果 `window == 0` 或 `window > values.size()`，返回空 `vector`。

返回值中第 `i` 个元素是 `values[i] + ... + values[i + window - 1]` 除以 `window` 的结果，使用 `double` 计算。

`starter.cpp` 中已经有一个函数，但它的滑动和没有减去离开窗口的元素。请修复这个错误并保持接口不变。不要写 `main`。
