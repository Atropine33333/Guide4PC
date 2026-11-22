为自定义的二维点类型提供相等比较和 `std::hash` 特化，使它能在 `std::unordered_set` 中使用。

```cpp
#include <cstddef>
#include <functional>

struct Point {
    int x;
    int y;
    bool operator==(const Point& other) const;
};

namespace std {
template <>
struct hash<Point> {
    std::size_t operator()(const Point& point) const;
};
}
```

约定：

- 两个坐标都相同的点相等；坐标顺序不同则不相等。
- 相等的点必须得到相同的哈希值。
- 哈希值必须同时依赖 `x` 和 `y`：交换坐标、或只改变其中一个坐标时，哈希值应不同（允许极少数碰撞，但测试中的点必须区分开）。
- 支持负坐标。
- 不要写 `main`。

### 提示

一种简单的组合方式是 `std::hash<int>{}(x) * 1000003u ^ std::hash<int>{}(y)`，也可以用位移与异或混合。`std::hash<Point>` 是标准库模板的特化，必须写在 `namespace std` 中。
