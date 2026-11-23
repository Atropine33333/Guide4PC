// 参考实现：c-union-find
#include <stddef.h>

#define UF_CAP 1024

typedef struct {
    int parent[UF_CAP];
    int n;
    int groups;
} UnionFind;

void uf_init(UnionFind *u, int n) {
    u->n = n;
    u->groups = n;
    for (int i = 0; i < n; ++i) {
        u->parent[i] = i;
    }
}

int uf_find(UnionFind *u, int x) {
    if (x < 0 || x >= u->n) return -1;
    int root = x;
    while (u->parent[root] != root) {
        root = u->parent[root];
    }
    while (u->parent[x] != root) {
        int next = u->parent[x];
        u->parent[x] = root;
        x = next;
    }
    return root;
}

int uf_union(UnionFind *u, int a, int b) {
    int ra = uf_find(u, a);
    int rb = uf_find(u, b);
    if (ra < 0 || rb < 0) return -1;
    if (ra == rb) return 0;
    if (ra < rb) {
        u->parent[rb] = ra;
    } else {
        u->parent[ra] = rb;
    }
    u->groups -= 1;
    return 1;
}

int uf_connected(UnionFind *u, int a, int b) {
    int ra = uf_find(u, a);
    int rb = uf_find(u, b);
    return ra >= 0 && ra == rb;
}

int uf_groups(const UnionFind *u) {
    return u->groups;
}
