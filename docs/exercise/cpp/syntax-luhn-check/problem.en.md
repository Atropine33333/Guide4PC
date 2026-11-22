**The Code of the Check Digit**

Be it enacted by these presents, and in force from the day of publication: every holder of a number may use this Code to try its truth and to supply its final place.

**In plain words**: implement `luhn_valid` and `luhn_check_digit` — the first decides whether a string of digits passes the Luhn test, the second yields the check digit to append.

```cpp
#include <string>

bool luhn_valid(const std::string& digits);
char luhn_check_digit(const std::string& payload);
```

### The Articles

Article 1. Only ASCII digit characters are handled; spaces and hyphens `-` are skipped outright, and every other character is likewise ignored.

Article 2. `luhn_valid` begins at the rightmost digit and doubles the 2nd, 4th, 6th... positions counted from the right, the rightmost being position 1; if a doubled value exceeds 9, subtract 9. Summing all positions, it returns `true` when the sum is divisible by 10.

Article 3. When no digit character is present at all, `luhn_valid` returns `false`.

Article 4. `luhn_check_digit(payload)` returns one ASCII digit character which, appended to `payload`, makes `luhn_valid` return `true`. In this reckoning it starts at the rightmost digit of `payload` and doubles the 1st, 3rd, 5th... positions from the right, since the check digit appended later will occupy the rightmost place.

Article 5. When `payload` holds no digit at all, `luhn_check_digit` returns `'0'`.

Article 6. This Code grants functions only; `main` is not provided.

### Example

| Call | Result |
| --- | --- |
| `luhn_valid("79927398713")` | `true` |
| `luhn_valid("79927398710")` | `false` |
| `luhn_check_digit("7992739871")` | `'3'` |

### Hint

Write one helper that sums by the positional rule, and let both public functions call it. Remember that spaces and `-` may occur inside the `std::string`.
