Two unsigned numbers are subtracted and the result stays unsigned. Read `a` and `b`, compute `a - b` as `unsigned`, then convert the same result to `int`, and put both values on one line separated by a single space.

**In plain words**: when `a - b` would drop below zero, the subtraction borrows from the top, so the unsigned result wraps modulo 2^32; the second value reads that wrapped result as a 32-bit signed integer, which is negative when its top bit is set.

**Input**: one line with two integers `a` and `b`, with `0 ≤ a, b ≤ 100`.

**Output**: one line with the unsigned value of `a - b` and the value of `(int)(a - b)`, separated by a single space.

### Example

Input:

```text
3 5
```

Output:

```text
4294967294 -2
```

`3 - 5` has nothing to take, so it wraps to `2^32 - 2 = 4294967294`; reading that value as a 32-bit two's complement integer gives `-2`.

### Contract

- Unsigned subtraction wraps modulo `2^32`; the shortfall is at most `100`, so it never wraps more than once.
- Print the first value with `%u` and the second with `%d`.
- On this machine `unsigned` and `int` are both 32 bits, so the converted value is negative when the top bit is set.
- No trailing spaces at the end of the line.
- Print nothing beyond the answer.
