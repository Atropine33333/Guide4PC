ORDINANCE ON TOPOLOGICAL ORDER

Chapter I. General Provisions

Article 1. A directed graph is given. Wherever an edge `u v` exists, the predecessor `u` points to the successor `v`; the vertices are numbered from 1 and number `n` in all, and the edges number `m`.

Article 2. A topological order of this graph is an arrangement of all `n` vertices in which every edge points from an earlier vertex to a later one. A graph may admit many valid arrangements; this ordinance recognises only the lexicographically smallest.

Chapter II. Input and Output

Article 3. The first line of the input holds `n m` (`1 ≤ n ≤ 10^5`, `0 ≤ m ≤ 2·10^5`); the following `m` lines each hold `u v` (`1 ≤ u, v ≤ n`), an edge from `u` to `v`.

Article 4. When a topological order exists, print `n` lines, each holding one vertex number; when none exists, print a single line `cycle`.

Chapter III. Details

Article 5. In building the sequence, choose each time the smallest-numbered vertex among those whose in-degree is zero, keeping them in a min-heap; arbitrary choosing is not permitted.

Article 6. Parallel edges accumulate in the in-degree and must be counted edge by edge; they shall not be merged into one.

Article 7. A self-loop is handled under the latter half of Article 4, as is any directed cycle.

Article 8. The graph may be disconnected; isolated vertices have in-degree zero and shall be printed as this ordinance requires, with none omitted.

This ordinance takes effect from the day of its promulgation.

**In plain words**: given a directed graph, print the lexicographically smallest topological order, or `cycle` if the graph has a cycle. Read `n m` and then `m` edges, with vertices numbered 1 to n; in the order every edge must point from an earlier vertex to a later one. Many valid orders may exist, so the smallest in lexicographic order is required — each time take the smallest-numbered vertex among those with in-degree zero, held in a min-heap. Parallel edges add to the in-degree several times and must be counted edge by edge; a self-loop is a cycle; the graph may be disconnected and isolated vertices must still be printed. Read stdin, write stdout.

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
