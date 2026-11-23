Storyteller's tale: the row of stone locks.

Word has it that one day a training ground was lined with stone locks, from the first to the `n`-th, one fathom apart, in a neat row.

The instructor called two attendants: one beside the first lock, one beside the last. “How far apart are you two?” he asked.

The first bowed. “From the first to the last, `n` minus one fathoms, sir.”

The instructor waved his flag. “Now take the one in the middle and read out the number carved on it.”

(A rap of the gavel; the tale goes on another day.)

**In plain words**: read `n` and `n` integers, then print two values — `&arr[n - 1] - &arr[0]` (a pointer subtraction giving the number of elements between them) and `arr[n / 2]` — separated by one space.

**Input**: the first line holds an integer `n` (`1 ≤ n ≤ 100`); the second line holds `n` integers, each between `-10^6` and `10^6`.

**Output**: one line with two integers — the distance between the two pointers and the value of `arr[n / 2]` — separated by one space.

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

The first and last locks are `5 - 1 = 4` positions apart; `n / 2` is `2`, and the lock at index `2` is carved with `30`.

### Contract

- Pointer subtraction yields a `ptrdiff_t` whose value is a count of elements; print it with `%td`.
- Both pointers refer to elements of the same array, so the subtraction cannot overflow.
- Indices start at `0`, and `n / 2` rounds down.
- When `n` is `1` the first and last lock are the same one, so the distance is `0`.
- No trailing spaces at the end of the line.
