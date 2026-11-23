# 位图使用说明

机房墙上钉着一页说明，纸边卷起，正文分条。

**一、适用范围** 本说明用于把若干位按编号记下来：编号自 `0` 起，最多 256 位。每一位只有「置位」与「未置位」两种状态。

**二、初始化** `bitset_init` 记下总位数 `nbits`，并把所有位清成未置位。

**三、置位** `bitset_set` 把第 `i` 位置位，成功返回 `0`。位号越界返回 `-1`，内容不变。已经置位的位再置一次，仍返回 `0`。

**四、清位** `bitset_clear` 把第 `i` 位清成未置位，成功返回 `0`；越界返回 `-1`。已经未置位的位再清一次，仍返回 `0`。

**五、查询** `bitset_test` 对已置位返回 `1`，对未置位返回 `0`，对越界返回 `-1`。

**六、计数** `bitset_count` 返回 `[0, nbits)` 内已置位的位数；范围之外的填充位不计。

**七、注意** 不分配内存；每个字节装 8 位，位号小的在低位；相邻的位互不牵连。

**人话**：实现一个最多 256 位的位图，支持初始化、置位、清位、查询与计数；越界的置位、清位返回 `-1`，越界查询也返回 `-1`。

**接口**：

```c
#define BITSET_MAX_BITS 256

typedef struct {
    unsigned char words[BITSET_MAX_BITS / 8];
    int nbits;
} BitSet;

void bitset_init(BitSet *b, int nbits);
int bitset_set(BitSet *b, int i);
int bitset_clear(BitSet *b, int i);
int bitset_test(const BitSet *b, int i);
int bitset_count(const BitSet *b);
```

### 示例

`bitset_init(&b, 13)` 之后，置位 `7`、`8`、`12`，`bitset_count(&b)` 为 `3`；再把 `8` 清掉，计数为 `2`，`bitset_test(&b, 8)` 为 `0`。

### 约定

- 只实现这些函数，不要写 `main`。
- 不使用动态分配，容量就是 `BITSET_MAX_BITS`。
- 位号必须落在 `[0, nbits)` 内；`nbits` 最大为 `BITSET_MAX_BITS`。
- 越界调用不得改动位图。
