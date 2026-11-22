判断无向图是否是二分图，并给出一种二染色。

```cpp
#include <vector>

bool is_bipartite(const std::vector<std::vector<int>>& graph);
std::vector<int> two_coloring(const std::vector<std::vector<int>>& graph);
```

- `graph` 使用 `0` 到 `n - 1` 的顶点编号，`graph[v]` 是顶点 `v` 的邻接表。
- 图是无向图：一条边会在两个端点的邻接表中各出现一次。
- 允许重边和自环；自环会使图不是二分图。
- `is_bipartite` 返回图能否用两种颜色染色，使每条边的两个端点颜色不同。
- 若图是二分图，`two_coloring` 返回长度为 `n` 的数组，每个元素是 `0` 或 `1`，且对每条边满足 `colors[u] != colors[v]`；孤立顶点可以染成 `0`。
- 若图不是二分图，`two_coloring` 返回空数组。
- 不要写 `main`。

### 提示

从每个未访问的顶点出发做 BFS 或 DFS，给起点染 `0`，邻居染相反颜色；如果遇到已经在队列中且颜色相同的邻居，说明存在奇环。
