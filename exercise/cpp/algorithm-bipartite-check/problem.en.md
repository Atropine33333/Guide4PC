Decide whether an undirected graph is bipartite and produce a two-colouring.

```cpp
#include <vector>

bool is_bipartite(const std::vector<std::vector<int>>& graph);
std::vector<int> two_coloring(const std::vector<std::vector<int>>& graph);
```

- `graph` uses vertex ids `0` to `n - 1`; `graph[v]` is the adjacency list of vertex `v`.
- The graph is undirected: each edge appears once in the adjacency list of both endpoints.
- Parallel edges and self-loops are allowed; a self-loop makes the graph non-bipartite.
- `is_bipartite` reports whether the graph can be coloured with two colours so that the endpoints of every edge differ.
- When the graph is bipartite, `two_coloring` returns an array of length `n` whose entries are `0` or `1`, with `colors[u] != colors[v]` for every edge; isolated vertices may be coloured `0`.
- When the graph is not bipartite, `two_coloring` returns an empty array.
- Do not write `main`.

### Hint

Run BFS or DFS from every unvisited vertex, colour the start `0`, and colour neighbours the opposite colour. Meeting a visited neighbour with the same colour means the graph contains an odd cycle.
