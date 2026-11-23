> A field to pave in square-cut stone,
> one edge for all, and one alone;
> subtract the less until they meet,
> that equal number gives the stone.
>
> One channel opens every third day,
> another every fifth, they say;
> both stood open at this morning's bell —
> which dawn finds both wide open, tell?

The *Nine Chapters*, in its Field chapter, has a rule: halve whatever can be halved; what cannot, set the two numbers side by side, take the less from the greater, and keep subtracting until the two are equal.

A field is `a` paces wide and `b` paces long, and the magistrate wants it paved with identical square stones, as large as possible — what is the longest edge? Two channels are also set: channel A opens its sluice once every `a` days, channel B once every `b` days. Both open today; on which day do they first open together again?

**In plain words**: for `a` and `b`, output their greatest common divisor (the stone's edge) and least common multiple (the first day both channels open), in that order.

**Input**: one line with two integers `a` and `b` (`1 ≤ a, b ≤ 10^9`).

**Output**: one line with two integers separated by a single space: the longest edge (the GCD) and the first shared opening day (the LCM).

### Example

Input:

```text
12 18
```

Output:

```text
6 36
```

### Contract

- The side of the largest square stone that paves the field is the GCD of `a` and `b`.
- The first shared opening day is the LCM of `a` and `b`; it can approach `10^18`, so store it in a `long long`.
- Divide before multiplying: `a / g * b`, so no intermediate value overflows.
- Both numbers are at least `1`, so there is no division by zero.
- No trailing spaces at the end of the line.
