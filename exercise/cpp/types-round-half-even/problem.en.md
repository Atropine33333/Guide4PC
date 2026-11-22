Read `n` real numbers and round each one to the nearest integer. When a value is exactly halfway between two integers, round to even (banker's rounding): choose the neighboring integer that is even.

For example:

- `2.5` rounds to `2`, and `3.5` rounds to `4`.
- `-2.5` rounds to `-2`, and `-3.5` rounds to `-4`.
- `2.4999` rounds to `2`, and `2.5001` rounds to `3`.

### Input

The first line contains an integer `n` (`1 <= n <= 1000`).

The second line contains `n` real numbers, each between `-1000000` and `1000000`, with at most three fractional digits.

### Output

`n` lines, each containing the rounded value. Never print a negative zero: print `0` when the result is zero.

### Example

Input:

```text
4
0.5 1.5 2.5 -2.5
```

Output:

```text
0
2
2
-2
```

### Constraints

- Do not use `std::round`; it rounds halfway cases away from zero.
- You may use `std::nearbyint`, whose default rounding mode rounds halfway cases to even, or decide the fractional part yourself.
- Use only the standard library and print nothing else.
