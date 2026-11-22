（深夜演播厅。穿米色西装的男子举起三件不锈钢小玩意。）

还在为空指针提心吊胆吗？指针三件套，一次拥有，只要 19.99 元！

`sum_or_zero`：递上指针和长度，它把和算给你；指针是 `nullptr` 或 `count <= 0`，它干净利落地报 `0`，一根毫毛不碰。

不止如此！`max_or` 找出最大的那个；空手而来时，把你的 `fallback` 原样奉还。不崩溃，不半夜打电话。

还有 `count_even`：数偶数。负偶数也算，`-4` 算，`-3` 不算。

现在下单，附赠守卫一行：`if (data == nullptr || count <= 0) return ...;`，写在循环之前，分文不取。

接线员已就位。请勿编写 `main`。本广告在禁止地区无效。

**人话**：实现三个“指针 + 长度”函数；`data == nullptr` 或 `count <= 0` 时分别返回 `0`、`fallback`、`0`，且不访问 `data`。`sum_or_zero` 用 `long long` 求和；`max_or` 取最大；`count_even` 数偶数，负偶数也算。

```cpp
long long sum_or_zero(const int* data, int count);
int max_or(const int* data, int count, int fallback);
int count_even(const int* data, int count);
```

### 示例

| 调用 | 结果 |
| --- | --- |
| `sum_or_zero(nullptr, 5)` | `0` |
| `max_or(nullptr, 5, -7)` | `-7` |
| `sum_or_zero(values, 3)`，`values = {2, -4, 7}` | `5` |
| `count_even(values, 3)`，`values = {2, -4, 7}` | `2` |

### 提示

把守卫写成 `if (data == nullptr || count <= 0) return ...;`，再进入循环。即使调用方传入 `(nullptr, 0)`，也不要把空指针送进循环体或 `std::max_element`。
