# 同乡会登记须知

会馆墙上钉着一张纸，纸角发黄，字迹却很清楚：

一、本会号位自 `0` 编起，共 1024 个。进门先登记，登记之后才查得到人。

二、初来时一人一组，组长就是本人。

三、并组须递两张号位。若两人本已同组，文书当场作废，组数不变。

四、并组之后，组长取两组中号位较小者。此条不改。

五、查某人属哪一组，须报出组长号位。组长本人报自己。

六、问两人是否同乡，问的是两人的组长是否为同一人。

七、会中现有几组，须随时报得出来。

八、号位不存在的文书一律退回，会册不得改动。

**人话**：实现一个并查集，支持初始化、查组长、合并两组、判断同组、统计组数；每组的组长取组内号位最小者。

**接口**：

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

- `uf_init` 登记 `n` 个人，`0 ≤ n ≤ UF_CAP`。
- `uf_find` 返回 `x` 所在组的组长号位；`x` 越界返回 `-1`。
- `uf_union` 合并两组：合并成功返回 `1`，本已同组返回 `0`，任一越界返回 `-1`。
- `uf_connected` 同组返回 `1`，否则返回 `0`；越界返回 `0`。
- `uf_groups` 返回当前组数。

### 示例

`uf_init(&u, 4)` 之后共 4 组。`uf_union(&u, 2, 3)` 得 `1`，该组组长为 `2`；`uf_union(&u, 1, 2)` 得 `1`，此时 `uf_find(&u, 3)` 为 `1`，`uf_groups(&u)` 为 `2`。

### 约定

- 只实现这些函数，不要写 `main`。
- 不使用动态分配，容量就是 `UF_CAP`。
- 同一组的组长唯一，且为组内号位最小者。
- 越界调用不得改动已有数据。
