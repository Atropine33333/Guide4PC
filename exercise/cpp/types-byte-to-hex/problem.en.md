Two in the morning. One voice left on television, over a gradient blue that is far too blue.

"Folks, are you still lying awake because `printf("%X", 7)` gives you a bare `7`? Look at this — two uppercase hexadecimal digits! `00`, `0F`, `FF`, not one missing, the leading zero always on duty! Whether your byte is `0` or `255`, it fills both characters, every single time!"

"But wait, there's more! Order now and we throw in the uppercase alphabet, `A` through `F`, never lowercase! One value per line, clean as a whistle! Just 19.99!"

**In plain words**: read `n` byte values `v` and print each as exactly two uppercase hexadecimal digits, one per line; pad values below `16` with a leading `0`.

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
