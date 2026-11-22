THE THIRTY-TWO BIT RULES

1. This door admits only integers from `0` to `4294967295`. On the way in each is kept as 32 unsigned bits, no more and no fewer.

2. Bits are numbered from `0`: bit `0` is the rightmost, bit `31` the leftmost.

3. Turn all 32 bits around: bit `0` moves to bit `31`, bit `31` moves to bit `0`, and every other bit folds the same way.

4. `1` has only bit `0` lit; turned around, only bit `31` is lit, which reads as `2147483648`.

5. All 32 bits of `0` are dark, so turned around it is still `0`. This rule has no exception.

6. All 32 bits of `4294967295` are lit; turned around they are still all lit, so it remains `4294967295`.

7. If you stuff the turned number into a signed `int` and print it directly, the `2147483648` of rule 4 shows up as `-2147483648`, and rule 4 is void for the moment; read it back as unsigned and rule 4 holds again at once.

8. Rules 1 to 6 hold for every legal input; only rule 7 is void or holds depending on how you read the result.

**In plain words**: read an integer and print the integer obtained by reversing its 32 binary bits.

**Input**: one line with an integer `x` such that `0 ≤ x ≤ 4294967295`.

**Output**: one line with one integer: the value of `x` with its 32 bits reversed, printed as unsigned decimal.

### Example

Input:

```text
1
```

Output:

```text
2147483648
```

`1` has only its lowest bit set; reversed, only the highest bit is set, which is `2^31`.

### Contract

- Reverse exactly 32 bits; leading zeros take part, and the result stays between `0` and `4294967295`.
- Keep the input and the result in `uint32_t`; do not store intermediate results in a signed type.
- Print unsigned decimal, with no plus sign and no leading zeros.
- Print nothing beyond the answer.
