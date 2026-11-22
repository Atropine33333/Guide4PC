*House Rules for Shifting*

Rule one. This room recognises unsigned numbers only. Before any left shift, write the thing being shifted as `1u`.

Rule two. The shift count `k` stays between `0` and `31`. At `32` or beyond, these rules no longer reach your result.

Rule three. If a signed `1` is shifted into the top bit, reread rule two: the result carries no guarantee from this room.

Rule four. Every shift copies out two lines. The first line holds `1u << k` and `(1u << k) - 1`, separated by one space. The second line holds the value of `1u << 31`.

**In plain words**: read `k`. On the first line print `1u << k` and `(1u << k) - 1`; on the second line print the value of `1u << 31`, which is `2147483648`.

**Input**: one line with a single integer `k` (`0 ≤ k ≤ 31`).

**Output**: two lines. The first line has two unsigned decimal numbers, `1u << k` and `(1u << k) - 1`; the second line has one unsigned decimal number, the value of `1u << 31`.

### Example

Input:

```text
5
```

Output:

```text
32 31
2147483648
```

### Contract

- `1u` is an `unsigned int` literal, so `1u << 31` yields `2147483648` and can be stored and printed normally.
- A signed `1 << 31` shifts into the sign bit, and a shift count of `32` or more is equally unusable; keep both out of the program.
- When `k` is `0`, `(1u << 0) - 1` is `0`.
- Print unsigned decimals with `%u`; no trailing spaces at the end of a line.
