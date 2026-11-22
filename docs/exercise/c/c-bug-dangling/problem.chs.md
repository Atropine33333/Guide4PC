星港第七码头有一间影子寄存处。八只影子舱，每只放一个名字，长不过三十一个字符。旅客登船前留下名字，影子便住进舱里；回来取走影子，舱位才重新空出来。值夜的人把名字抄进舱位，就等于把影子按了进去——舱位一直占着，直到旅客本人来取。

上一班的看守抄完名字，顺手就把舱位交还了。名字还留在舱里，可下一班旅客一登记，住的就是同一只舱。你手里那张写着名字的条子，指向的已经是别人的影子。

**人话**：实现这八只影子舱。`pool_register` 把名字抄进第一只空舱并占住它，返回舱里的地址；`pool_release` 交还舱位。`starter.c` 里 `pool_register` 抄完立刻交还了舱位，于是返回的地址随时会被下一位旅客覆盖。把交还的时机改对。

**接口**：

```c
#define POOL_CAP 8
#define SLOT_LEN 32

typedef struct {
    char slots[POOL_CAP][SLOT_LEN];
    unsigned char used[POOL_CAP];
} NamePool;

void pool_init(NamePool *p);
const char *pool_register(NamePool *p, const char *name);
void pool_release(NamePool *p, const char *slot);
```

- `pool_init`：把八只舱位全部置空。
- `pool_register`：找第一只空舱，把 `name` 抄进去，最多 31 个字符，末尾补 `'\0'`；占住舱位并返回舱里的地址。没有空舱时返回 `NULL`。
- `pool_release`：交还 `slot` 指向的舱位；只认舱位起点，别的地址一概不动。
- 交还之后舱里的字符仍留在原处，但舱位随时可能被下一次登记覆盖。

### 示例

先登记 `Kepler`，再登记 `Void`。两次登记各占一只舱，第一次返回的地址读出来仍是 `Kepler`；调用 `pool_release` 交还第一只舱之后，新的登记才可能住进它。

### 约定

- 这些函数会被直接调用，不要写 `main`。
- 不要动态分配；舱位就是结构体里的数组。
- 名字超过 31 个字符时只留前 31 个，其余截掉。
- 八只舱都占满时返回 `NULL`，已有的名字保持原样。
- 八只舱可以反复登记、交还，交还两只就又能登记两只。
