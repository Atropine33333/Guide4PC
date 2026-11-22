这一题不读 stdin，也不写 stdout。实现一个固定容量、用代际编号发现悬空句柄的槽位表。

```cpp
#include <cstddef>
#include <cstdint>

struct Handle {
    std::uint32_t index;
    std::uint32_t generation;
};

class HandleStore {
public:
    static constexpr std::uint32_t kCapacity = 8;
    static constexpr std::uint32_t kInvalidIndex = 0xffffffffu;

    HandleStore();

    Handle create(int value);
    bool valid(Handle handle) const;
    int* get(Handle handle);
    const int* get(Handle handle) const;
    bool destroy(Handle handle);
    std::size_t size() const;
    std::size_t capacity() const;
};
```

规则：

- 槽位下标为 `0` 到 `kCapacity - 1`，每个槽位有一个从 `1` 开始的代际编号。
- `create` 找一个空闲槽位，存入值，返回当前代际的句柄；没有空槽时返回 `{kInvalidIndex, 0}`。
- `destroy` 对有效句柄释放槽位、把该槽位代际加一，返回 `true`；对无效或已过期的句柄返回 `false`，不改变状态。
- `valid` 仅在句柄的下标在范围内、代际匹配且槽位已占用时返回 `true`。
- `get` 对无效句柄返回 `nullptr`；对有效句柄返回指向槽内值的指针。
- 旧句柄在槽位被复用后必须失效，即使新句柄使用同一下标。
- `size()` 返回当前占用的槽位数，`capacity()` 返回 `kCapacity`。
- 不要写 `main`，不要修改公开接口。
