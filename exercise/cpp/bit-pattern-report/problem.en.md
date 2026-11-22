Circular on the Reporting of Bit-Level Conditions of Non-Negative Integers

To all divisions concerned:

In order to keep a complete picture of the bit-level condition of non-negative integers, the reporting of thirty-two-bit patterns is set out as follows.

1. Scope. Treat the input integer as a thirty-two-bit binary pattern and report three items.
2. Item one: the number of set bits, that is, the count of `1`s in the pattern (population count).
3. Item two: the weight of the highest set bit. If every bit is zero, report `0` for this item.
4. Item three: the value obtained by reading those 32 bits as a signed 32-bit two's-complement integer.
5. Format. The three items go on one line, separated by single spaces.
6. Instrumentation. The input may be as large as `4294967295`, which lies outside the range of `int`; it shall not be read into an `int` under any circumstances.

**In plain words**: read an integer `x`, treat its low 32 bits as a binary pattern, and print three integers: the number of `1` bits, the weight of the highest set bit, and the signed value of those 32 bits in two's complement.

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
