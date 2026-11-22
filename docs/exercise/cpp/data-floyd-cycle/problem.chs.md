这一题不读 stdin。给定一个“每个结点最多一条出边”的有向图，从指定起点出发，找出路径上的环。

```cpp
#include <vector>

struct CycleInfo {
    bool has_cycle;
    int entry;
    int length;
};

CycleInfo analyze(const std::vector<int>& next, int start);
```

- 图有 `n` 个结点，编号 `0` 到 `n - 1`；`next[v]` 是 `v` 的唯一出边终点，`-1` 表示没有出边。
- 从 `start` 开始沿 `next` 前进，直到遇到 `-1` 或重复访问某个结点。
- 如果路径上出现了环，`has_cycle` 为 `true`，`entry` 是**第一次重复出现的结点**（环入口），`length` 是环上的结点数。
- 如果没有环，返回 `{false, -1, 0}`。
- `start` 越界（小于 `0` 或不小于 `n`）时也返回 `{false, -1, 0}`。
- 不要写 `main`。

### 示例

`next = {1, 2, 3, 1}`，从 `0` 出发：`0 → 1 → 2 → 3 → 1`，第一次重复的是 `1`，环是 `1 → 2 → 3 → 1`，所以 `entry = 1`、`length = 3`。

### 提示

记录每个结点第一次被访问时的步数。走到已经访问过的结点 `x` 时，环长等于当前步数减去 `x` 首次被访问的步数。
