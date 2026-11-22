Two numbers stand written on paper in base ten, each at most two hundred digits long. How great is their product?

The rule: multiply from right to left, digit by digit. Take the last digit of the second number and multiply it through every digit of the first, laying the result below; then take the next-to-last digit and repeat, shifted one column to the left; and so on to the head of the second number, row upon row. Sum each column, and when it reaches ten carry one into the column ahead; if that column reaches ten, carry again. Continue to the far left. Strike out any leading zeros. If either number is `0`, write `0` and spare the counting rods.

A certain reckoner multiplied but never carried: he kept only the last digit of each column and threw the carry away. The product has the right number of columns and the wrong value in every one. Correct the carrying.

**In plain words**: read two non-negative integers of at most 200 digits each (decimal strings, no leading zeros) and print their product as a decimal string with no leading zeros; if either operand is `0`, print `0`. Multiply digit by digit as in long multiplication and add each carry back into the next column. The starter keeps only the units digit of each column and drops the carry; restore it.

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
