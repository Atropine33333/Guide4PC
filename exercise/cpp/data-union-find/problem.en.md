This exercise reads no stdin and writes no stdout. Implement a disjoint-set structure; a hidden harness runs the tests.

Implement:

```cpp
class DisjointSet {
public:
    explicit DisjointSet(int n);   // elements 0..n-1; n may be 0
    int find(int x);               // representative of x's set
    bool unite(int a, int b);      // true if merged, false if already together
    bool connected(int a, int b);
    int components() const;        // current number of sets
};
```

Requirements:

- Initially every element is alone and there are `n` sets;
- `unite` lowers the set count only when the two sets differ;
- Arguments are guaranteed to be in `[0, n)`;
- Path compression and union by rank are recommended, but correctness comes first.

Do not write `main`.
