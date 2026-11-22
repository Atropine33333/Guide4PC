`int arr[10]` is a ruler of fixed length: fill one slot or all ten and it still measures ten. Read some integers into the ruler, then report how many slots it has and how wide one slot is.

**Input**: one line with an integer `n` (`1 ≤ n ≤ 10`) followed by `n` integers, placed into the first `n` slots of `int arr[10]`.

**Output**: one line with two integers separated by a single space: the value of `sizeof(arr) / sizeof(arr[0])`, then the value of `sizeof(int)`.

### Example

Input:

```text
3 5 -1 0
```

Output:

```text
10 4
```

### Contract

- The number of slots comes from the declaration, not from how many were filled this time.
- Apply `sizeof` where `arr` is still an array; do not pass it to a function and measure there.
- Convert both results to `int` and print them with `%d`.
- The expected output assumes an `int` of 4 bytes; on another platform the second number follows the compiler.
- No trailing spaces at the end of the line.
