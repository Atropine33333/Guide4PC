这一题不读 stdin。实现一个转发函数，观察左值被拷贝、右值被移动。

```cpp
#include <utility>

struct Tracked {
    static int copies;
    static int moves;
    int value;

    explicit Tracked(int value);
    Tracked(const Tracked& other);
    Tracked(Tracked&& other) noexcept;
    Tracked& operator=(const Tracked& other);
    Tracked& operator=(Tracked&& other) noexcept;
    ~Tracked() = default;
};

int Tracked::copies = 0;
int Tracked::moves = 0;

template <class T>
Tracked forward_to_tracked(T&& value);
```

约定：

- `Tracked` 的拷贝构造函数执行 `++copies`，移动构造函数执行 `++moves`；赋值运算符不增加计数。
- 移动构造后，源对象的 `value` 设为 `0`。
- `forward_to_tracked` 必须把参数**完美转发**给 `Tracked` 的构造函数：传入 `Tracked` 左值时发生一次拷贝，传入 `Tracked` 右值时发生一次移动，传入 `int` 或其他可转换为 `int` 的类型时直接构造、不增加拷贝/移动计数。
- 请直接 `return Tracked(std::forward<T>(value));`，不要先创建局部变量再返回，以保证返回时的拷贝/移动被省略。
- 不要写 `main`（`Tracked::copies` / `Tracked::moves` 的静态定义已经给出，不要重复定义）。
