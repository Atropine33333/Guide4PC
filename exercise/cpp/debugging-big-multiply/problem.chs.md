下面的程序计算两个非负整数的乘积，但结果在需要进位时会出错。请修好它。

```cpp
std::string multiply(const std::string& a, const std::string& b) {
    if (a == "0" || b == "0") return "0";
    std::vector<int> digits(a.size() + b.size(), 0);
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
        for (int j = static_cast<int>(b.size()) - 1; j >= 0; --j) {
            digits[i + j + 1] =
                (digits[i + j + 1] + (a[i] - '0') * (b[j] - '0')) % 10;
        }
    }
    std::string out;
    for (int d : digits) {
        if (out.empty() && d == 0) continue;
        out.push_back(static_cast<char>('0' + d));
    }
    return out.empty() ? "0" : out;
}
```

两个数都以十进制字符串给出（没有前导零，除了单独的 `0`），长度不超过 200。输出乘积，同样不能有前导零；只要有一个数为 `0`，结果就是 `0`。

### 输入

两行，每行一个非负整数。

### 输出

一行，乘积的十进制表示。

### 示例

输入：

```text
12
12
```

输出：

```text
144
```

### 约束

- 每个数最多 200 位。
- 乘积最多 400 位，需要使用大整数乘法，不能转换成 `long long`。
