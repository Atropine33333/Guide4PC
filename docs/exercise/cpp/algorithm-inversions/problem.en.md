**The Spread**

The querent shuffled three times and laid the cards in a row; the faces are the numbers. The reader looked them over and said:

"The place of a card is here, its face is there. When a card earlier in the row shows a greater face than one later, that is one misordered pair; the more such pairs, the more tangled the matter. Two cards showing the same face are but one card seen twice in a mirror, and count for nothing."

How many misordered pairs lie in the row? The row may hold a hundred thousand cards; faces may be negative and may repeat; the tally outgrows an `int`, so keep it in a `long long`. Nothing is read from stdin and nothing is written to stdout; a hidden harness runs the tests, so do not write `main`.

**In plain words**: implement `count_inversions`, which counts the index pairs `(i, j)` with `i < j` and `values[i] > values[j]`; an empty sequence and a one-element sequence both give 0, equal elements do not count, and the length is at most 100000.

```cpp
long long count_inversions(const std::vector<int>& values);
```

### Rules

- An empty sequence and a one-element sequence both give 0.
- Equal elements do not form an inversion; only strict greater-than counts.
- Values may be negative or duplicated; the length is at most 100000.
- The result may exceed the `int` range; the return type is `long long`.
- Do not write `main`.

### Hint

A double loop is too slow for 100000 elements. Count during a merge sort, or use a Fenwick tree with coordinate compression. During merging, taking an element from the right half makes it an inversion with every remaining element of the left half.
