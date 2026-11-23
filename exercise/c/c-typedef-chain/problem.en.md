`score` is the type name of one score. Someone found `int` too plain and gave it aliases:

```c
typedef int score;
typedef score *score_ptr;
```

`score_ptr` points at a score, and `*score_ptr` reads back as `score`. Alias upon alias — a tongue twister to say, yet the compiler follows it without confusion.

**In plain words**: read `n` scores, point a `score_ptr` at index `n / 2`, then print `sizeof(score)` and `*p` separated by one space.

**Input**: the first line holds an integer `n` (`1 ≤ n ≤ 1000`); the second line holds `n` integers, each between `-10^6` and `10^6`, one score apiece.

**Output**: one line with two integers — `sizeof(score)` and `*p` — separated by one space.

### Example

Input:

```text
5
10 20 30 40 50
```

Output:

```text
4 30
```

`n / 2` is `2`, and the element at index `2` is `30`.

### Contract

- Write `typedef int score;` first, then `typedef score *score_ptr;`, so the pointer type also goes through an alias.
- In this environment `int` occupies 4 bytes, so `sizeof(score)` is `4`; print it with `%zu`.
- How many bytes the pointer itself takes is not part of the output.
- Indices start at `0`, and `n / 2` rounds down.
- No trailing spaces at the end of the line.
