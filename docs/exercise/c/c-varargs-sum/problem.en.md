> Numbers come without a name,
> only how many, and the same;
> take them one by one and bind
> the total; zero leaves none behind.

A surviving rule from the *Nine Chapters*, Broad Measurement: set down a company of numbers, do not name them, only say how many. The method: first ask the count, then take them one at a time and join them, and the total is had. If the count is zero, nothing is taken, and the total is zero as well.

A reckoner keeps numbers in a sleeve, announces the count `count`, then recites the integers one by one. What is their sum?

**In plain words**: implement `int sum_ints(int count, ...)`, which takes `count` integers and returns their sum; when `count` is `0`, return `0`.

**Interface**:

```c
int sum_ints(int count, ...);
```

- `count` is the number of arguments that follow, from `0` to `8`.
- The return value is the sum of those integers; negatives join the sum as usual.
- Implement this single function and do not write `main`.
- These functions are called directly, with every argument passed as an `int`.

### Example

`sum_ints(3, 4, -1, 2)` returns `5`; `sum_ints(0)` returns `0`; `sum_ints(2, -7, -8)` returns `-15`.

### Contract

- Take the arguments with `va_list`, `va_start`, `va_arg`, and `va_end` from `<stdarg.h>`.
- Read every argument as an `int`.
- `count` is never negative.
- Do not park the arguments in an array or a global variable.
