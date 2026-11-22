轨道货运站把货箱码成一座能量金字塔：塔尖永远是最轻的一箱，每一个舱位都不比它下面两个舱位重。新货从底部装船，塔基一松，塔尖那箱就沿着更轻的一侧一路下沉，直到落稳。

**人话**：用一个小顶堆维护一组整数——`heap_build` 把一个数组整理成小顶堆，`heap_sift_down` 让某个位置上的元素和更小的孩子交换着往下沉，`heap_extract_min` 每次取走最小的那个。

**接口**：

```c
#include <stddef.h>

#define HEAP_CAP 64

typedef struct {
    int data[HEAP_CAP];
    size_t len;
} MinHeap;

void heap_sift_down(MinHeap *h, size_t i);
void heap_build(MinHeap *h, const int *values, size_t n);
int heap_extract_min(MinHeap *h, int *out);
```

- 下标 `i` 的两个孩子是 `2 * i + 1` 与 `2 * i + 2`，父亲是 `(i - 1) / 2`；小顶堆要求每个孩子都不小于父亲。
- `heap_build` 把 `values` 的前 `n` 个元素抄进 `h->data`，令 `h->len = n`，再从最后一个非叶节点往前逐个下沉；`n` 为 `0` 时 `h->len` 为 `0`。
- `heap_sift_down` 假定左右子树都已经是合法小顶堆，把 `i` 处的元素与更小的孩子交换，一路沉到合适位置。
- `heap_extract_min` 在堆空时返回 `-1`，`*out` 不动；否则把堆顶写进 `*out`，把最后一个元素搬到堆顶，下沉一次，`len` 减一，返回 `0`。

### 示例

用 `5 3 8 1 9 2 7` 建堆后，连续取出七次，依次得到 `1 2 3 5 7 8 9`；再取一次返回 `-1`，此时 `len` 为 `0`。

### 约定

- 只实现这些函数，不要写 `main`。
- 元素个数不会超过 `HEAP_CAP`，数组按层序存放。
- 重复值照常入堆，取出顺序只需保证非降。
- 结构体保持题面给出的字段，函数签名不变。
