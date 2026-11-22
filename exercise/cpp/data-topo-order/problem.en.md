Given a directed graph, print the lexicographically smallest topological order, or `cycle` if the graph has a cycle.

**Input**:

- The first line holds two integers `n m` (`1 ≤ n ≤ 10^5`, `0 ≤ m ≤ 2·10^5`).
- The next `m` lines each hold two integers `u v` (`1 ≤ u, v ≤ n`), a directed edge from `u` to `v`.

**Output**:

- When a topological order exists: `n` lines, each holding one vertex number.
- Otherwise: a single line with `cycle`.

### Example

Input:

```text
4 4
1 2
1 3
2 4
3 4
```

Output:

```text
1
2
3
4
```

### Constraints

- In a topological order every edge points from an earlier vertex to a later one.
- Many orders may be valid; this problem asks for the lexicographically smallest one, so repeatedly pick the smallest-numbered vertex among those with in-degree 0, using a min-heap.
- Parallel edges add to the in-degree several times and must be counted edge by edge; a self-loop is a cycle.
- The graph may be disconnected; isolated vertices still have to be printed.
