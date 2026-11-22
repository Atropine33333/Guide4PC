这一题不读 stdin，也不写 stdout。实现一个只能通过定位 `new` 构造元素的定长数组。

```cpp
#include <cstddef>

struct Tracker {
    explicit Tracker(int value);
    ~Tracker();

    int value() const;
    static int alive();
    static int constructions();
    static int destructions();

    Tracker(const Tracker&) = delete;
    Tracker& operator=(const Tracker&) = delete;
};

class TrackerBuffer {
public:
    explicit TrackerBuffer(std::size_t capacity);
    ~TrackerBuffer();

    TrackerBuffer(const TrackerBuffer&) = delete;
    TrackerBuffer& operator=(const TrackerBuffer&) = delete;

    bool push(int value);
    std::size_t size() const;
    Tracker& operator[](std::size_t index);
    const Tracker& operator[](std::size_t index) const;
};
```

规则：

- `Tracker` 构造时增加 `alive()` 和 `constructions()`；析构时减少 `alive()` 并增加 `destructions()`。
- `TrackerBuffer` 只分配原始存储，不得用 `new Tracker[n]` 或 `std::vector<Tracker>` 预先构造元素。
- `push` 在未满时用定位 `new` 构造一个元素并返回 `true`；已满时返回 `false`，不构造任何对象。
- `size()` 返回当前元素个数；`operator[]` 访问已构造元素。
- `TrackerBuffer` 析构时按任意顺序显式调用所有元素的析构函数；最后一个元素析构后 `alive()` 应恢复。
- 存储必须对齐到 `Tracker` 的对齐要求。
- 不要写 `main`，不要修改公开接口。
