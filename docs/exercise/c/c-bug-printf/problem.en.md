The program in `starter.c` reads two real numbers and should print their sum with two decimal places. As it stands it prints the `double` result with `%d`, so the number on the screen is nonsense. Fix the format.

**Input**: one line with two real numbers `a` and `b`, each with at most two decimal places and absolute value at most `10000`, separated by a single space.

**Output**: one line, the value of `a + b` with two decimal places.

### Example

Input:

```text
1.25 2.50
```

Output:

```text
3.75
```

### Contract

- Read and compute with `double`; print with `%.2f`.
- Change only the broken spot; keep the input and output format as it is.
- Print one line, ending with a newline.
