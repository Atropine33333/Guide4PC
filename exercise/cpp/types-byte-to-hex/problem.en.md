Read `n` byte values and print each one as a two-digit uppercase hexadecimal number.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 256`).
- The second line contains `n` integers `v` (`0 ≤ v ≤ 255`).

**Output**: `n` lines, each with exactly two uppercase hexadecimal characters. Values below `16` need a leading `0`.

### Example

Input:

```text
5
0 15 16 127 255
```

Output:

```text
00
0F
10
7F
FF
```

### Constraints

- Hexadecimal letters must be uppercase.
- Every line must contain exactly two digits; `0` must not become an empty line or a single character.
- Values are always in `0`–`255`, so an `unsigned char` can hold them.
