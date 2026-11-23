A: Keeper of the ledger, there is only one number `i` on the page. How do you write three entries from it?

B: The first entry, `a = i++`: hand the current `i` to `a`, then let `i` grow by one.

A: Record first, age later.

B: The second entry, `b = ++i`: age `i` first, then hand the aged `i` to `b`.

A: Age first, record later.

B: The third entry, `c = i--`: hand `i` to `c`, then let it step back by one.

A: And after those three entries, what is left on the page?

B: That is for `i` itself to say.

**In plain words**: read an integer `i`, run `a = i++;`, `b = ++i;`, `c = i--;` in that order, then print `a`, `b`, `c` and the final `i`, separated by single spaces.

**Input**: one line with a single integer `i` (`-1000 ≤ i ≤ 1000`).

**Output**: one line with four integers: `a`, `b`, `c`, and the final `i`.

### Example

Input:

```text
5
```

Output:

```text
5 7 7 6
```

### Contract

- The three statements take three lines and keep this exact order; do not merge them.
- Prefix and postfix differ only in when the value is read: one reads before the change, the other after.
- Do not modify `i` twice inside one expression.
- No trailing spaces at the end of the line.
