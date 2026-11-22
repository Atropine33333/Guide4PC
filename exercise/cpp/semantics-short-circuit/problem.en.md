Read two integers and print five boolean lines that show how logical operators short-circuit.

**Input**: one line with two integers `a b` (`|a|, |b| ≤ 10^9`).

**Output**: five lines, each `true` or `false`, describing in order:

1. `a < b`
2. `a == b`
3. `a % 2 == 0 && b % 2 == 0` (both numbers are even)
4. `a != 0 && b / a >= 2`
5. `a == 0 || b % a == 0`

### Example

Input:

```text
2 3
```

Output:

```text
true
false
false
false
false
```

### Constraints

- Lines 4 and 5 must rely on the short-circuit behavior of `&&` and `||`: when `a == 0`, neither `b / a` nor `b % a` may be evaluated.
- The division on line 4 is C++ integer division (truncation toward zero).
- Use `std::boolalpha` or an equivalent approach to print lowercase `true`/`false`.
