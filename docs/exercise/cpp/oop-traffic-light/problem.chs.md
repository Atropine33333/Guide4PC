这一题不读 stdin，也不写 stdout。用一个类模拟交通灯的状态循环，测试在隐藏的 harness 里。

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

规则：

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
