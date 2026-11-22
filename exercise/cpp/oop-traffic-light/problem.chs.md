话说这十字路口，一盏灯，三般颜色。列位看官，且听我慢慢道来。

这灯自打立起来，便是红的。一扳机关，红变绿；再扳，绿变黄；再扳，黄又回红——周而复始，不曾错乱。扳一下，账上记一笔；扳 `steps` 下，就记 `steps` 笔。账上的笔数与眼下什么颜色，原是两个账本，不可混为一谈。

有那性急的，一口气扳三下，灯转了一圈，又回原处，账上却明明白白记着三笔。

灯的名号也要报得清楚：`"Red"`、`"Green"`、`"Yellow"`，一字不差。报字号这桩事，不必先有灯，静态函数即可。

**人话**：实现 `TrafficLight` 类：新建时为红灯，`ticks()` 为 0；`advance()` 按 `Red -> Green -> Yellow -> Red` 切换一次并让计数加一；`advance(steps)` 连续切换 `steps` 次（`steps >= 0`），计数加 `steps`；`current()` 报当前颜色；`name(Light)` 是静态函数，返回三色名号。`ticks()` 只记累计切换次数，与当前颜色没有固定关系。

```cpp
enum class Light { Red, Yellow, Green };

class TrafficLight {
public:
    TrafficLight();
    Light current() const;
    void advance();
    void advance(int steps);
    int ticks() const;
    static const char* name(Light light);
};
```

### 规则

- 新建的交通灯是红灯，`ticks()` 为 0。
- 每次 `advance()` 按 `Red -> Green -> Yellow -> Red` 的顺序切换一次，并让 `ticks()` 增加 1。
- `advance(steps)` 连续切换 `steps` 次，`steps >= 0`；`ticks()` 增加 `steps`。
- `name` 返回 `"Red"`、`"Green"`、`"Yellow"`。
- 不要写 `main`。

### 示例

| 调用序列 | `current()` / `ticks()` |
| --- | --- |
| 构造后 | `Red` / 0 |
| `advance()` | `Green` / 1 |
| 再 `advance(2)` | `Red` / 3 |

### 提示

把状态存成 `Light`，切换时用 `switch` 或取模。`ticks()` 只是累计切换次数，和当前颜色没有固定关系。`name` 是静态成员函数，不需要对象也能调用。
