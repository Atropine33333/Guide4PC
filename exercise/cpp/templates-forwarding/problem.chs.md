话说这一日，`forward_to_tracked` 门前来了三位客人。

头一位是左值，姓 `Tracked`，带着一箱家当。门房接了名帖，照着规矩把家当原样誊抄一份，原件仍归客人——这一手叫拷贝，`copies` 加一。

第二位是右值，也姓 `Tracked`，家当却不必誊抄，连箱子一并抬进里间——这一手叫移动，`moves` 加一。搬完之后，原箱子里的 `value` 归零，算是空了。

第三位是个 `int`，家当轻省，进门直接造一个 `Tracked`，既不誊抄也不搬箱，两个计数器纹丝不动。

列位看官，奥妙只在 `T&&` 与 `std::forward<T>`。形参一律按右值引用接下来，转发时却要还原它本来的身份：左值仍是左值，右值仍是右值。这一步做不到，左值也要被当成右值搬走，账就乱了。

还有一节：`return Tracked(std::forward<T>(value));` 要一气呵成。莫先造个局部变量再回身去取，编译器一犹豫，多算一次拷贝，账上又要多一笔。

**人话**：实现函数模板 `forward_to_tracked`，把实参完美转发给 `Tracked` 的构造函数：左值一次拷贝，右值一次移动，`int` 之类直接构造、两个计数都不增加。`Tracked` 的定义已经给出。

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
