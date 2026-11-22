Now hear the tale of the night the graph grew restless. There were `n` cities, numbered zero through `n-1`, and `m` roads, every one of them one-way; some were short, some were long, none ran backwards.

The storyteller rapped his table on the zero city. "City zero: fare zero," he said, and sent a scout down every road out of it. The scout came back with news: this city is near, that one is far. Another rap of the table: the nearest city still unclaimed was settled, and from it a fresh scout set out; wherever a detour came out cheaper, the old fare was crossed out and the new one written. Rap by rap, scout by scout, until no city was left to explore and every fare stood final. Cities with no road to them got a single word — "none" — written `-1`.

**In plain words**: given a directed graph with `n` vertices and `m` edges of non-negative weight, find the shortest path length from vertex `0` to every vertex; print `-1` for unreachable vertices. Use Dijkstra with a priority queue, and store distances in 64-bit integers.

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
