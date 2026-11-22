The outer `x` starts with the value read from the input. Step into a pair of braces and declare another `x` with the same name, one greater than the outer one. Inside those braces the new `x` is the one that counts. Step back out and the outer `x` is there again, untouched.

Report both: the inner value first, then the outer one.

**In plain words**: read an integer `x`; inside a block declare a variable with the same name and set it to the outer value plus `1`, print the inner value, then print the outer value from outside the block, separated by a single space.

**Input**: one line with an integer `x` such that `-1000000 ≤ x ≤ 1000000`.

**Output**: one line with two integers — the inner value, then the outer value.

### Example

Input:

```text
5
```

Output:

```text
6 5
```

### Contract

- After the inner block ends, the outer `x` keeps the value it was read with.
- In `int x = x + 1;` the `x` on the right names the newly declared variable, which has no value yet; store the outer value in another variable first, then declare the shadowing one.
- No trailing spaces at the end of the line.
