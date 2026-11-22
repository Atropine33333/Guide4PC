这一题不读 stdin，也不写 stdout。实现 Luhn 校验算法，测试在隐藏的 harness 里。

```cpp
#include <string>

bool luhn_valid(const std::string& digits);
char luhn_check_digit(const std::string& payload);
```

规则：

- 只处理 ASCII 数字字符；空格和连字符 `-` 直接跳过，其他字符也忽略。
- `luhn_valid` 从最右一位开始，把倒数第 2、4、6……位（从右数，右边第一位算第 1 位）乘以 2；如果乘 2 后大于 9，就减去 9。所有位求和，能被 10 整除时返回 `true`。
- 没有任何数字字符时，`luhn_valid` 返回 `false`。
- `luhn_check_digit(payload)` 返回一个 ASCII 数字字符，把它拼到 `payload` 末尾后，`luhn_valid` 必须返回 `true`。计算时从 `payload` 最右一位开始，把倒数第 1、3、5……位乘以 2（因为将来追加的校验位会占据最右边）。
- 当 `payload` 没有任何数字时，`luhn_check_digit` 返回 `'0'`。
- 不要写 `main`。

### 提示

先写一个辅助函数，按规定的位置规则求和；两个公开函数都可以调用它。注意 `std::string` 里可能出现空格和 `-`。
