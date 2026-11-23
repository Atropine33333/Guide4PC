MANUAL: HOW TO READ STATE NUMBERS

§1 Purpose
This page explains how to read the state numbers that a controller reports as names.

§2 The number table

```c
enum State {
    IDLE = 0,
    RUN  = 2,
    DONE = 5
};
```

The numbers are spaced out on purpose: `0` is idle, `2` is running, `5` is done.

§3 Reading rule
Take an integer and look it up in the table:

| Number | Name |
| ------ | ---- |
| `0`    | `IDLE` |
| `2`    | `RUN` |
| `5`    | `DONE` |
| others | `UNKNOWN` |

Numbering starts at `0`, and negative readings are legal; they fall under "others".

§4 Output
Write the name on one line, all capitals, with a single newline at the end.

**In plain words**: read an integer and print `IDLE`, `RUN` or `DONE` according to the values of `enum State`; print `UNKNOWN` for anything that is not `0`, `2` or `5`.

**Input**: one line with an integer, `-10^9 ≤ value ≤ 10^9`.

**Output**: one line with the state name or `UNKNOWN`.

### Example

Input:

```text
2
```

Output:

```text
RUN
```

`2` is the running state in the table, so it reads as `RUN`.

### Contract

- Only `0`, `2` and `5` are recognised; everything else is `UNKNOWN`.
- Numbers in the gaps (such as `1`, `3` and `4`) belong to no state.
- Names are all capitals, with no quotes or spaces.
