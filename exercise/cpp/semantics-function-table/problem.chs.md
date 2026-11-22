这一题不读 stdin。实现一张把名字映射到二元函数的表，隐藏的 harness 会调用这些函数。

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

约定：

- `lookup_op` 认识 `"add"`、`"sub"`、`"mul"`、`"min"`，返回对应的函数指针；其他名字返回 `nullptr`。
- `apply_op` 查到函数就用它计算 `a` 和 `b`；查不到时返回 `fallback`，绝不能解引用空指针。
- `apply_all` 按顺序调用 `ops` 中每个非空函数，把结果依次放入返回的数组；空指针跳过。
- 参数范围 `|a|, |b| ≤ 1000`，乘积不会超过 `10^6`。
- 不要写 `main`。

### 提示

可以把四个函数指针放进数组里线性查找，也可以直接写 `if` 链；注意函数名和函数指针的布尔判断。
