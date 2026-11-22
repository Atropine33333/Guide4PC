Read `n` unsigned 32-bit integers and print the number of `1` bits in each binary representation.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 20`).
- The second line contains `n` integers `v` (`0 ≤ v ≤ 4294967295`).

**Output**: `n` lines, each with the popcount of the corresponding value.

### Example

Input:

```text
2
0 4294967295
```

Output:

```text
0
32
```

### Hint

`std::popcount` from `<bit>` does this directly; a manual bit loop also works. The inputs exceed the positive range of a 32-bit signed `int`, so read into `unsigned long long` first and then convert to `std::uint32_t`.
