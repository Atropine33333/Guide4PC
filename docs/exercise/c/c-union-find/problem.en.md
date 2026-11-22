# Notice for the Hometown Society

A sheet of paper is pinned to the wall of the meeting hall. The corners have yellowed, but the writing is still clear.

1. Seats in this society are numbered from `0`, 1024 in all. Register at the door; only then can anyone look you up.
2. On arrival each person forms a group of one, and the head of that group is the person.
3. To merge two groups, two seat numbers must be filed. If the two people already share a group, the paper is void on the spot and the number of groups stays the same.
4. After a merge, the head is the smaller seat number of the two groups. This rule does not change.
5. To ask which group a person belongs to, report the seat number of the head. A head reports himself.
6. Asking whether two people are from the same hometown means asking whether their heads are the same person.
7. The number of groups currently on the roll must be reportable at any time.
8. Papers naming a seat that does not exist are returned, and the roll must not change.

**In plain words**: implement a union-find structure with initialisation, head lookup, union of two groups, a same-group test and a group count; the head of a group is its smallest seat number.

**Interface**:

```c
#define UF_CAP 1024

typedef struct {
    int parent[UF_CAP];
    int n;
    int groups;
} UnionFind;

void uf_init(UnionFind *u, int n);
int uf_find(UnionFind *u, int x);
int uf_union(UnionFind *u, int a, int b);
int uf_connected(UnionFind *u, int a, int b);
int uf_groups(const UnionFind *u);
```

- `uf_init` registers `n` people, `0 ≤ n ≤ UF_CAP`.
- `uf_find` returns the head's seat number for the group holding `x`; out-of-range `x` gives `-1`.
- `uf_union` merges two groups: `1` when merged, `0` when they already share a group, `-1` when either seat is out of range.
- `uf_connected` returns `1` for the same group, `0` otherwise, and `0` when out of range.
- `uf_groups` returns the current number of groups.

### Example

After `uf_init(&u, 4)` there are 4 groups. `uf_union(&u, 2, 3)` returns `1` and the head is `2`; `uf_union(&u, 1, 2)` returns `1`, after which `uf_find(&u, 3)` is `1` and `uf_groups(&u)` is `2`.

### Contract

- Implement only these functions; do not write `main`.
- No dynamic allocation; the capacity is `UF_CAP`.
- A group has exactly one head, and it is the smallest seat number in the group.
- An out-of-range call must not change existing data.
