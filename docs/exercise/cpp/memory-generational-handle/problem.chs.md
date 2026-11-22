（桌上摊开八张牌。）

占卜师洗了牌，牌面朝下，一张一张推到你面前。

“牌桌有八席，从 `0` 数到 `7`。每席压着一个数，唤作代际，起手是 `1`。”

“抽牌时，你把值写进一席空位；我把该席当前的代际记在牌面 `{index, generation}` 上，这张牌便归你。八席坐满，我再发牌，牌面就是 `{kInvalidIndex, 0}`。”

“牌面能信几时？”

“牌在、代际相合、席位仍被占着，才算在。`valid` 只对这样的牌点头；`get` 对过期牌报空，对有效牌指向席中之值。”

“若是弃牌呢？”

“`destroy` 收走你手上的牌，把那一席的代际加一。旧牌从此作废；哪怕下一轮有人坐进同一席，你手上那张旧牌也不再作数。拿过期牌来弃，我只摇头，桌上的牌一张不动。”

“牌面就是这些数：`size()` 报占了几席，`capacity()` 报 `kCapacity`。别自己开牌局，也别擅改这副牌。记着，别写 `main`。”

**人话**：实现 `HandleStore`：八席，每席代际从 `1` 起。`create` 找空席存值，返回当前代际的句柄，没有空席返回 `{kInvalidIndex, 0}`；`destroy` 对有效句柄释放席位、代际加一、返回 `true`，对无效或过期句柄返回 `false` 且不改状态；`valid` 要求下标在范围内、代际匹配、席位被占用；`get` 对无效句柄返回 `nullptr`，对有效句柄返回席中值的指针；旧句柄在同下标复用时必须失效；`size()` 报占用数，`capacity()` 报 `kCapacity`。本题不读 stdin，不写 stdout。

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
