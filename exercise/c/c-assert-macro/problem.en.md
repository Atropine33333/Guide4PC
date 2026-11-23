(Two performers come on stage. A wooden block sits on the table.)

A: I have a rule here. Before anyone speaks, he must shout once.

B: Shout what?

A: If the condition holds, he shouts "OK". If it does not, he shouts "FAIL:line" and names the line where the trouble sits.

B: With a line number, no less.

A: Of course. The number must come from where the call is written. I put `#line 40` above, so the two comparisons below count from line 40 and line 41.

B: And if the first one is already wrong?

A: He shouts `FAIL:40` first, then gives the second shout.

B: And if both are wrong?

A: Then `FAIL:40` and `FAIL:41`, with no shout missing.

B: Right, I shall remember the rule.

**In plain words**: Define a `CHECK(cond)` macro: print `OK` when the condition holds, and `FAIL:<line>` when it does not, taking the line number from `__LINE__` at the call site. Read two integers `a` and `b`; check `a < b` and then `a == b`, printing one line per check.

**Input**: one line with two integers `a` and `b`, where `-1000 ≤ a ≤ 1000` and `-1000 ≤ b ≤ 1000`.

**Output**: two lines. The first is the result of `CHECK(a < b)`; the second is the result of `CHECK(a == b)`. Print `OK` when the condition holds and `FAIL:<line>` when it does not.

### Example

Input:

```text
3 5
```

Output:

```text
OK
FAIL:41
```

`3 < 5` holds, so the first shout is `OK`; `3 == 5` does not hold, so the second shout names line 41.

### Contract

- Put the macro body in `do { ... } while (0)`, and write a semicolon at the end of each call.
- Evaluate the condition exactly once; do not write it twice.
- The line number comes from `__LINE__`; the two calls sit on the two lines after `#line 40`, so they are lines 40 and 41.
- Do not move the two calls to other lines, and do not change `#line 40`.
- No trailing spaces at the end of the line.
