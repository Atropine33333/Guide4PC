(Candlelight sways in the tent. The querent shuffles, draws, and spreads a stack of numbered cards on the table. Duplicates are allowed.)

Querent: I wish to ask about the card that is `k`-th smallest.

Diviner: Patience. The cards arrive one at a time. When you say `add x`, I place `x` in the pile; when you say `erase x`, I take one `x` out of the pile, only one. If no `x` is there, I leave the pile alone.

Querent: And `kth k`?

Diviner: I lay the cards out from smallest to largest and count to the `k`-th. Counting starts at 1. If `k` is less than 1, or greater than the number of cards now in the pile, the card reads `none`. One question, one answer.

Querent: How are duplicates counted?

Diviner: Every copy is a card of its own. `erase` takes one copy; the rest stay in the pile.

Querent: The pile holds at most two thousand cards. What if I count from the beginning every time?

Diviner: A clumsy method, yet it counts. For speed, use a container that sorts, removes by value, and returns the `k`-th in order. When fate has it, it is there; when fate lacks it, write `none`.

**In plain words**: Maintain a multiset of integers that allows duplicates and process `q` operations in input order: `add x` inserts one `x`; `erase x` removes one element equal to `x`, doing nothing if none exists; `kth k` asks for the current `k`-th smallest element, printing `none` when `k < 1` or `k` exceeds the current count. Print one line per `kth` and nothing for the other operations. `q ≤ 2000` and `|x| ≤ 10^9`.

**Input**:

- The first line holds one integer `q` (`1 <= q <= 2000`), the number of operations.
- The next `q` lines each contain one of:
  - `add x`: insert one `x` (`|x| <= 10^9`);
  - `erase x`: remove one element equal to `x`; do nothing if none exists;
  - `kth k`: query the `k`-th smallest element (`k` is an ordinary integer and may be less than `1` or larger than the current count).

**Output**:

- For every `kth` operation, print one line: the `k`-th smallest element, or `none` when `k < 1` or `k` exceeds the current count.
- Other operations produce no output.

### Example

Input:

```text
6
add 5
add 1
add 5
kth 1
erase 5
kth 2
```

Output:

```text
1
5
```

### Constraints

- Duplicate values are allowed; `erase` removes just one copy.
- Queries are not nested; process them in input order.
- The `k` of a `kth` query may be `0` or negative.
- Output contains only lowercase letters and decimal digits.
