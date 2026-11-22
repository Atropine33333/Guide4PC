A: They tell me you can draw a progress bar.

B: I can. Give me a percentage and a width, and I'll give you a thing like `[###-------] 30%`.

A: How do you count the cells?

B: `filled = percent * width / 100`, integer division, rounded down. Thirty percent of ten cells is three `#`, the other seven `-`, brackets around it, one space, then the percentage and `%`.

A: And ninety-nine percent in ten cells?

B: You'd guess ten? Wrong. Ninety-nine times ten is nine hundred ninety; over a hundred that's nine point nine; rounded down, nine. One `-` stays in the bar. That's the rule.

A: And a width of one?

B: Only a hundred percent earns a `#`; anything less runs empty.

A: Leading zeros on the percentage?

B: None. Write `30` and print `30`. Don't get clever with `030`. And no trailing spaces.

**In plain words**: read `percent` and `width`, compute `filled = percent * width / 100` rounded down, and print `[`, `filled` copies of `#`, the rest as `-`, `] `, then `percent%`, with no trailing spaces.

**Input**:

- One line with two integers `percent` and `width`, separated by one space.
- `0 <= percent <= 100`, `1 <= width <= 50`.

**Output**:

- One line such as `[###-------] 30%`, containing exactly `width` cells and one percent sign.
- Do not print trailing spaces.

### Example

Input:

```text
30 10
```

Output:

```text
[###-------] 30%
```

### Constraints

- Round down rather than to nearest: `99 10` fills only `9` cells.
- With `width = 1`, only `percent = 100` shows `#`.
- Print the percentage exactly as given, without leading zeros.
