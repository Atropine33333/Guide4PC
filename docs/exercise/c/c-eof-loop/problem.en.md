(The wooden clapper strikes; the house falls quiet.)

Storyteller: down by the bridge sits a tea stall whose keeper keeps his books by one odd rule. A guest walks in, he adds a stroke to the ledger; who the guest is and what he pays, he never asks, only whether another guest is coming. When the street packs up and nobody else walks in, he shuts the ledger and calls out one line: how many guests today, and how much coin.

Apprentice: and if the stall stays empty, not one guest?

Storyteller: then he calls out "none, and nothing".

**In plain words**: read integers from the input until no more remain, then print how many integers were read and their total.

**Input**: several lines, each holding zero or more integers. Between `0` and `100000` integers appear in total, each with `|x| ≤ 1000000000`. The input may be empty.

**Output**: one line with two integers, the count first and the total second, separated by a single space.

### Example

Input:

```text
1 2 3
4 5
```

Output:

```text
5 15
```

Five integers were read — `1`, `2`, `3`, `4`, `5` — and they add up to `15`.

### Contract

- Stop when the input ends; the loop has to close itself when nothing is left to read.
- An empty input prints `0 0`.
- The total may leave the `int` range; keep it in a 64-bit signed integer.
- No trailing spaces at the end of the line, and nothing outside the requested output.
