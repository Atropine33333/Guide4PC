This task reads no stdin and writes no stdout. Implement the Luhn algorithm; a hidden harness will call your functions.

```cpp
#include <string>

bool luhn_valid(const std::string& digits);
char luhn_check_digit(const std::string& payload);
```

Rules:

- Only ASCII digit characters are processed. Spaces and hyphens `-` are skipped, and any other character is also ignored.
- `luhn_valid` scans from the rightmost digit. Every second digit counted from the right (the rightmost digit is position 1, then 3, 5, ...) has already been doubled when it was part of the full number; equivalently, double the digits at positions 2, 4, 6, ... from the right. If the doubled value exceeds 9, subtract 9. The number is valid when the total is divisible by 10.
- If there is no digit at all, `luhn_valid` returns `false`.
- `luhn_check_digit(payload)` returns one ASCII digit such that appending it to `payload` makes `luhn_valid` return `true`. While computing it, double the rightmost payload digit, then every second digit moving left, because the future check digit occupies the rightmost position.
- If `payload` contains no digits, `luhn_check_digit` returns `'0'`.
- Do not write `main`.

### Hint

Write one helper that sums the digits using the required position rule; both public functions can use it. Remember that the string may contain spaces and `-`.
