Read integers until the input ends. The numbers may span any number of lines, or there may be none at all.

**Input**: several lines with integers separated by spaces; every value has absolute value at most `10^9` and there are at most `10^5` of them.

**Output**: three lines:

1. how many integers were read;
2. their total sum;
3. the largest integer.

When the input is empty, print `0`, `0`, `0`.

### Example

Input:

```text
1 2
3
4 5
```

Output:

```text
5
15
5
```

### Constraints

- Do not treat the number of lines as the number of values: one line may hold several integers, and the last line may lack a newline.
- Individual values are small, but the sum may exceed `int`; accumulate in a 64-bit integer.
- `while (std::cin >> value)` reads until end of input.
