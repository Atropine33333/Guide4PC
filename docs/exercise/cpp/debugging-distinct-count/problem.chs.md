下面这个函数想统计有序序列中不同值的个数，但结果总是差一个。请修改它，让所有输入都返回正确的个数。

```cpp
#include <vector>

int count_distinct(const std::vector<int>& sorted);
```

规则与约束：

- `sorted` 是非降序序列，可能有重复值，也可能为空。
- 返回值是不同值的个数。
- 不要改变函数签名，不要写 `main`。
- 只修改 `starter.cpp` 中的实现。
