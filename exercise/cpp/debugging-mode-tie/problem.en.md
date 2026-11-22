(A candle flickers in the tent. The querent spreads the deck; each card bears an integer.)

The reader: The faces are these numbers. A negative is reversed; a zero is a blank card.

The querent: I want to know which card comes up most.

The reader: The card that comes up most is your answer. But the table often holds a tie: two cards come up equally often, and sometimes three.

The querent: What decides a tie?

The reader: The face. The smaller face is read first. This matters — many count only the occurrences, forget to compare the faces, and keep the larger one that came later.

The querent: And if every card comes up exactly once?

The reader: That is a tie as well, the whole table equal. By the rule, read the smallest face.

The querent: A hundred thousand cards at once — will the candle last?

The reader: It will. Gather the cards into the ledger and tally them one by one; then walk the ledger again, keep the higher count, and on equal counts keep the smaller face. The ledger may be sorted, or it may be a hash table; the choice is yours.

**In plain words**: read `n` integers and print the most frequent value; on equal frequencies print the smaller value, so when every value appears once the answer is the minimum. The starter updates its answer with `>=`, which lets a later, larger value take a tie; change it to `>`. Keep the input/output format and modify only `starter.cpp`.

**Input**:

- The first line holds an integer `n` (`1 ≤ n ≤ 10^5`).
- The second line holds `n` integers with absolute value at most `10^9`.

**Output**: one integer, the most frequent value; when several values tie, print the smallest one.

### Example

Input:

```text
4
1 1 2 2
```

Output:

```text
1
```

### Constraints

- Values may be negative as well as 0.
- Many candidates may share the maximum frequency, so the smallest one must win.
- Do not change the input/output format or rewrite it as a completely different program.
- Only modify `starter.cpp`.
