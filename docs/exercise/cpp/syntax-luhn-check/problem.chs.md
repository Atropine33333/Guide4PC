**校验位条例**

兹公布《校验位条例》，自公布之日起施行。凡持编号者，皆可依本条例验其真伪、补其末位。

**人话**：实现 `luhn_valid` 与 `luhn_check_digit` 两个函数，前者判断一串数字是否通过 Luhn 校验，后者给出应补的校验位。

```cpp
#include <string>

bool luhn_valid(const std::string& digits);
char luhn_check_digit(const std::string& payload);
```

### 条例

第一条 只处理 ASCII 数字字符；空格和连字符 `-` 直接跳过，其他字符也忽略。

第二条 `luhn_valid` 从最右一位开始，把倒数第 2、4、6……位（从右数，右边第一位算第 1 位）乘以 2；如果乘 2 后大于 9，就减去 9。所有位求和，能被 10 整除时返回 `true`。

第三条 没有任何数字字符时，`luhn_valid` 返回 `false`。

第四条 `luhn_check_digit(payload)` 返回一个 ASCII 数字字符，把它拼到 `payload` 末尾后，`luhn_valid` 必须返回 `true`。计算时从 `payload` 最右一位开始，把倒数第 1、3、5……位乘以 2（因为将来追加的校验位会占据最右边）。

第五条 当 `payload` 没有任何数字时，`luhn_check_digit` 返回 `'0'`。

第六条 本条例只授函数，不设 `main`。

### 示例

| 调用 | 结果 |
| --- | --- |
| `luhn_valid("79927398713")` | `true` |
| `luhn_valid("79927398710")` | `false` |
| `luhn_check_digit("7992739871")` | `'3'` |

### 提示

先写一个辅助函数，按规定的位置规则求和；两个公开函数都可以调用它。注意 `std::string` 里可能出现空格和 `-`。
