#include <stddef.h>

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

void uf_init(UnionFind *u, int n) {
    /* TODO: 记下人数，让每个人自成一户。 */
    (void)u;
    (void)n;
}

int uf_find(UnionFind *u, int x) {
    /* TODO: 返回 x 所在组的组长；越界返回 -1。 */
    (void)u;
    (void)x;
    return -1;
}

int uf_union(UnionFind *u, int a, int b) {
    /* TODO: 并组；已同组返回 0，越界返回 -1。 */
    (void)u;
    (void)a;
    (void)b;
    return -1;
}

int uf_connected(UnionFind *u, int a, int b) {
    /* TODO: 两人同组返回 1，否则返回 0。 */
    (void)u;
    (void)a;
    (void)b;
    return 0;
}

int uf_groups(const UnionFind *u) {
    /* TODO: 返回当前组数。 */
    (void)u;
    return 0;
}
