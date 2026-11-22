The program below multiplies two non-negative integers, but it produces wrong results whenever a carry is needed. Fix it.

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

Both numbers are given as decimal strings without leading zeros (except a lone `0`) and are at most 200 digits long. Print their product with no leading zeros; if either operand is `0`, the result is `0`.

### Input

Two lines, each with one non-negative integer.

### Output

One line with the decimal product.

### Example

Input:

```text
12
12
```

Output:

```text
144
```

### Constraints

- Each number has at most 200 digits.
- The product has at most 400 digits; use big-integer multiplication instead of converting to `long long`.
