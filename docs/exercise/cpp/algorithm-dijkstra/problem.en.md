Given a directed graph with `n` vertices and `m` edges with non-negative weights, find the shortest path length from vertex `0` to every vertex. Print `-1` for unreachable vertices.

### Input

The first line contains two integers `n` and `m` (`1 <= n <= 2000`, `0 <= m <= 10000`).

The next `m` lines each contain three integers `u v w` (`0 <= u, v < n`, `0 <= w <= 1000000000`), representing a directed edge from `u` to `v` with weight `w`.

The graph may contain parallel edges and self-loops.

### Output

One line with `n` integers: the shortest distance from vertex `0` to vertices `0, 1, ..., n-1`, using `-1` for unreachable vertices. Separate adjacent integers with one space.

### Example

Input:

```text
3 3
0 1 10
0 2 1
2 1 2
```

Output:

```text
0 3 1
```

The direct distance to vertex `1` is 10, while going through vertex `2` gives `1 + 2 = 3`.

### Constraints

- Edge weights are non-negative, so Dijkstra's algorithm applies.
- Path lengths can exceed the range of a signed 32-bit integer; use 64-bit integers.
- Use only the standard library.
