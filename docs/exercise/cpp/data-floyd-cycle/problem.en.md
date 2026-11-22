Given a directed graph where every node has at most one outgoing edge, find the cycle reachable from a start node.

```cpp
#include <vector>

struct CycleInfo {
    bool has_cycle;
    int entry;
    int length;
};

CycleInfo analyze(const std::vector<int>& next, int start);
```

- The graph has `n` nodes numbered `0` to `n - 1`; `next[v]` is the only outgoing edge of `v`, and `-1` means there is no outgoing edge.
- Start at `start` and follow `next` until reaching `-1` or revisiting a node.
- If a cycle is reached, `has_cycle` is `true`, `entry` is the **first repeated node** (the cycle entrance), and `length` is the number of nodes on the cycle.
- If there is no cycle, return `{false, -1, 0}`.
- If `start` is out of range (less than `0` or not less than `n`), also return `{false, -1, 0}`.
- Do not write `main`.

### Example

With `next = {1, 2, 3, 1}` and start `0`: `0 -> 1 -> 2 -> 3 -> 1`. The first repeated node is `1`, the cycle is `1 -> 2 -> 3 -> 1`, so `entry = 1` and `length = 3`.

### Hint

Record the step at which each node is first visited. When you reach an already visited node `x`, the cycle length is the current step minus the step at which `x` was first visited.
