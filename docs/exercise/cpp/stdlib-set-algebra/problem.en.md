(Two in the morning, channel seven. The lights are glaring.)

"Friends, look at these two stacks of sorted cards: stack A has `n`, stack B has `m`, both ordered from small to large. What would you like?"

"The union? We have it! Two lists become one, each value kept once, from small to large."

"The intersection? Of course! Only values on both lists remain."

"A minus B? Hand it over! Everything in A that B lacks is yours."

"Buy the whole set: three lines of output, each ascending, no repeated values. If a line is empty, write `empty`."

"Two hundred thousand cards between the stacks? Don't compare them pair by pair! Walk two pointers once, or use the standard set algorithms — so fast the neighbours will think you cheated."

"Not sold in stores. Call now for just $19.99 — but wait, there's more! Order today and we'll throw in a duplicate-free output!"

"Operators are standing by! Call 676 - 6767 - 6767 today!"

**In plain words**: Given two non-decreasing integer sequences A and B, each possibly containing duplicates. Treat each sequence as a set and output the union, the intersection, and A minus B; each result is sorted ascending with duplicates removed. Print three lines in that order, integers separated by single spaces within a line; print `empty` for a result that is the empty set. The combined length may reach 200000, so do not compare every pair of elements.

### Input

- The first line holds two integers `n` and `m` (`0 ≤ n, m ≤ 100000`), the lengths of the sequences.
- The second line holds `n` non-decreasing integers, forming sequence A.
- The third line holds `m` non-decreasing integers, forming sequence B.

When a length is 0, its line is empty.

### Output

Three lines: the union, the intersection, and A minus B. Integers in each line are ascending, separated by one space. If the result is empty, print `empty`.

### Example

Input:

```
3 3
1 3 5
2 3 4
```

Output:

```
1 2 3 4 5
3
1 5
```

### Constraints

- Both sequences are already non-decreasing.
- A sequence may contain duplicates; each value appears once in the output.
- `n + m` can reach `200000`; do not compare every pair.
