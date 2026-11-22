Treat a non-negative integer as a 32-bit binary pattern and report three facts:

1. how many bits are `1` (population count);
2. the value of the highest set bit (`0` if every bit is `0`);
3. the value of those 32 bits interpreted as a signed 32-bit two's-complement integer.

**Input**: one line with an integer `x` such that `0 ≤ x ≤ 4294967295`.

**Output**: one line with three integers — population count, weight of the highest set bit, and the signed interpretation — separated by single spaces.

### Example

Input:

```text
4294967295
```

Output:

```text
32 2147483648 -1
```

### Constraints

- `x` may not fit in `int`; do not read it into an `int`.
- For the third value you can use a 64-bit intermediate: if the top bit is set, the result is `x - 4294967296`.
