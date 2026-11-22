Now hear this. One day an old clockmaker came to the bell tower and took on a single piece of work: to build a clock that fears no midnight.

Good people, an ordinary clock stops at 23:59:59. This one does not; one second more and it turns itself back to 00:00:00 as though nothing had happened. Ask it to run backwards and it runs just the same: one second back is 23:59:59. Send it forward a whole day and it goes round once more, leaving no mark on the face.

Said the old clockmaker: "The face shows only three numbers — hour, minute, second. The belly holds only one — the seconds counted from zero hour. The two tell the same moment."

**In plain words**: implement a `Clock` class on a 24-hour cycle, where construction and advancing both wrap across midnight.

Implement `Clock`:

- `Clock(int hour, int minute, int second)`: the three parts may be any integers, negatives included; after construction the time must be normalised into `00:00:00`–`23:59:59`.
- `void advance(int seconds)`: move `seconds` seconds forward; `seconds` may be negative.
- `int hour() const`, `int minute() const`, `int second() const`: return the normalised parts.
- `int total_seconds() const`: return the seconds elapsed since `00:00:00`, in `[0, 86399]`.
- `bool operator==(const Clock& other) const`: returns `true` when both denote the same moment.

All arithmetic runs on the 24-hour cycle. For instance `-1` second is `23:59:59`, and `25:00:00` is `01:00:00` on the next day. Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `Clock(25, 0, 0)` | `01:00:00` |
| `Clock(0, 0, -1)` | `23:59:59` |
| `Clock(12, 30, 0).advance(86400)` | still `12:30:00` |

### Hint

Fold the three parts into a total number of seconds and normalise with a modulo into `[0, 86399]`. For negatives, C++ `%` yields a negative remainder, so add one more period.
