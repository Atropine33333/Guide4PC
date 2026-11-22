THE LAW OF THE FALLING LADDER

Article 1 (Scope). Whoever falls from rung `n` down to rung 1 and adds up the numbers of the rungs passed, falls under this Law.

Article 2 (Entry). Entry is made at `case n` of a `switch`. Entry at a lower rung is not permitted.

Article 3 (Falling). On reaching a rung, add that rung's number to the total; then, without stopping, fall to the next rung. No `break` stands between rungs.

Article 4 (Landing). Only at `case 1`, after its number has been added, may a `break` leave the ladder.

Article 5 (Order). The fall goes from high to low, one rung at a time, and each rung is added exactly once.

Article 6 (Result). Print the accumulated total on a line of its own.

**In plain words**: read `n`, let a `switch` fall from `case n` down to `case 1`, add the rungs passed on the way, and print the total.

**Input**: one line with a single integer `n` (`1 ≤ n ≤ 9`).

**Output**: one line with one integer: the sum `1 + 2 + … + n`.

### Example

Input:

```text
4
```

Output:

```text
10
```

### Contract

- Use the fall-through of `switch` for the whole sum; do not switch to a loop or a closed formula.
- No rung except `case 1` carries a `break`.
- Add each rung exactly once.
- No trailing spaces at the end of the line.
