(The cloth is spread; the deck is shuffled and cut.)

The querent says nothing, pushing the cards across the table one at a time. Face up, all integers, every absolute value at most `10^9`. The cards come in no fixed rows, and nobody tells you how many there are; you take them one after another until the table is bare.

When the dealing stops, three things must be read:

1. how many cards were pushed across;
2. what all the face values add up to;
3. the largest card of them.

If not one card came, write three zeros and ask nothing further.

One old rule remains: the faces are small, but their sum may not fit a 32-bit pocket; keep it in a 64-bit integer. And if you counted lines, you counted wrong: one line may carry several cards, and the last line need not end with a newline.

**In plain words**: read integers from stdin until EOF and print, on three lines, their count, their sum and the maximum; empty input prints `0`, `0`, `0`, and the sum accumulates in a 64-bit integer.

**Input**: several lines with integers separated by spaces; every value has absolute value at most `10^9` and there are at most `10^5` of them.

**Output**: three lines:

1. how many integers were read;
2. their total sum;
3. the largest integer.

When the input is empty, print `0`, `0`, `0`.

### Example

Input:

```text
1 2
3
4 5
```

Output:

```text
5
15
5
```

### Constraints

- Do not treat the number of lines as the number of values: one line may hold several integers, and the last line may lack a newline.
- Individual values are small, but the sum may exceed `int`; accumulate in a 64-bit integer.
- `while (std::cin >> value)` reads until end of input.
