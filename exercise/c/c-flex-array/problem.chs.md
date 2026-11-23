（法条体，摘自《地块申请章程》。）

第一条　凡需一排整数者，向本营申请。营地按数目 `n` 一次划足：行首记 `n`，其后紧接 `n` 个整数，末尾不留余量。

第二条　新划之地，格子一律为零。

第三条　可自他处搬入，数目须与来处相符；来处为空者，只许搬零个。

第四条　取用格子须报下标。下标越界者，取回 `0`。

第五条　改写格子亦须报下标。下标越界者不予受理，原地退回。

第六条　求和以 `long long` 计，免得中途溢出。

第七条　退营整块归还；空手退营，不算违例。

**人话**：`IntFlex` 的最后一行是 `int a[];`，格子数目不写在结构体里，所以要用 `sizeof(IntFlex) + n * sizeof(int)` 一次申请整块内存；`n` 记在行首，格子紧跟在后面。

**接口**：

```c
typedef struct {
    size_t n;
    int a[];
} IntFlex;

IntFlex *flex_new(size_t n);
IntFlex *flex_from(const int *src, size_t n);
int flex_get(const IntFlex *v, size_t i);
void flex_set(IntFlex *v, size_t i, int value);
long long flex_sum(const IntFlex *v);
void flex_free(IntFlex *v);
```

- `flex_new` 申请 `sizeof(IntFlex) + n * sizeof(int)` 字节，`n` 记下数目，所有格子置 `0`；`n` 为 `0` 时也返回有效对象；申请失败返回 `NULL`。
- `flex_from` 新建 `n` 格并逐个搬入 `src` 的值；`src` 为 `NULL` 且 `n` 不为 `0` 时返回 `NULL`。
- `flex_get` 返回第 `i` 格的值；`i` 不小于 `n` 或 `v` 为 `NULL` 时返回 `0`。
- `flex_set` 改写第 `i` 格；越界时什么也不做。
- `flex_sum` 返回全部格子之和，空地块返回 `0`。
- `flex_free` 归还整块地块；`flex_free(NULL)` 什么也不做。

### 示例

```c
int src[3] = {3, -1, 7};
IntFlex *v = flex_from(src, 3);   /* n == 3，格子为 3、-1、7 */
long long s = flex_sum(v);        /* 9 */
flex_set(v, 1, 10);               /* 格子变成 3、10、7 */
flex_set(v, 3, 0);                /* 越界，不受理 */
flex_free(v);
```

### 约定

- 只需要实现这六个函数，不要写 `main`。
- 一次申请、一次归还，不额外分配格子数组。
- 结构体末行保持 `int a[];` 的写法，格子与 `n` 在同一块内存里。
