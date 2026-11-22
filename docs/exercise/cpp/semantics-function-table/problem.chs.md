《评书·函数指针表》

话说这一日，说书人一拍醒木，道：「列位看官，今日不讲刀马，单讲一张表。此表四行，行行是一位好汉：一名 `add`，惯使加法；一名 `sub`，专管减法；一名 `mul`，力能相乘；一名 `min`，只取小的。名号写在左边，本事记在右边，这本事唤作函数指针。」

「看官要问：名字对不上，如何是好？答曰：递回一个空指针，权当此人不在册上。若有人不辨名号，硬把空指针请去办事，那便是自寻烦恼。」

说到此处，醒木再响：「且听下回分解。」

**人话**：用函数指针实现一张名字到二元函数的表：`lookup_op` 按名字返回 `op_add`、`op_sub`、`op_mul`、`op_min`，查不到返回 `nullptr`；`apply_op` 查到就调用，查不到返回 `fallback`，不许解引用空指针；`apply_all` 依序调用非空函数指针并收集结果，空指针跳过。

```cpp
#include <string>
#include <vector>

using BinOp = int (*)(int, int);

int op_add(int a, int b);   // a + b
int op_sub(int a, int b);   // a - b
int op_mul(int a, int b);   // a * b
int op_min(int a, int b);   // 较小者

BinOp lookup_op(const std::string& name);
int apply_op(const std::string& name, int a, int b, int fallback);
std::vector<int> apply_all(const std::vector<BinOp>& ops, int a, int b);
```

### 约定

- `lookup_op` 认识 `"add"`、`"sub"`、`"mul"`、`"min"`，返回对应的函数指针；其他名字返回 `nullptr`。
- `apply_op` 查到函数就用它计算 `a` 和 `b`；查不到时返回 `fallback`，绝不能解引用空指针。
- `apply_all` 按顺序调用 `ops` 中每个非空函数，把结果依次放入返回的数组；空指针跳过。
- 参数范围 `|a|, |b| ≤ 1000`，乘积不会超过 `10^6`。
- 不要写 `main`。

### 提示

可以把四个函数指针放进数组里线性查找，也可以直接写 `if` 链；注意函数名和函数指针的布尔判断。
