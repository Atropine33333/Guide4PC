And the LORD spoke unto Moses, saying: number the congregation by their tribes; let there be `n` men, each in his father's house, each under his own banner. At the first every man stood alone, and the number of the tribes was `n`.

So Moses called the elders together and said unto them: if two men be of one blood, join them into one tribe; and when they are joined, write one of them as the head of the tribe, and let the count of tribes fall by one. If the two are already of one tribe, the count shall not fall. Whosoever asks of a man's tribe shall be answered with the head of that tribe; and whosoever asks whether two men are of one blood shall be answered by their heads. And the congregation shall be told at any hour how many tribes remain.

And the LORD spoke further: in the numbering, let every guide press flat the scrolls along the road, so that every man points straight to the head, lest the way grow long in the days to come. And Moses did as the LORD commanded.

**In plain words**: implement disjoint-set union. `DisjointSet(int n)` puts each element `0..n-1` in its own set, and `n` may be 0; `find(x)` returns the representative of `x`'s set; `unite(a, b)` merges and returns `true` only when the two sets differ, and returns `false` with the count unchanged when they are already together; `connected(a, b)` says whether they share a set; `components()` returns the current number of sets. Arguments are guaranteed to be in `[0, n)`. Path compression and union by rank are recommended, but correctness comes first. Do not write `main`.

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
