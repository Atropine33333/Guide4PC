This exercise reads no stdin and writes no stdout. Implement a clock that wraps around midnight automatically; a hidden harness runs the tests.

Implement `Clock`:

- `Clock(int hour, int minute, int second)`: the three fields may be any integers, including negative ones; after construction the clock must be normalized to `00:00:00`–`23:59:59`.
- `void advance(int seconds)`: move forward by `seconds`; `seconds` may be negative.
- `int hour() const`, `int minute() const`, `int second() const`: return the normalized fields.
- `int total_seconds() const`: return the seconds elapsed since `00:00:00`, in `[0, 86399]`.
- `bool operator==(const Clock& other) const`: return `true` when both clocks denote the same instant.

Every operation runs modulo 24 hours. For example, `-1` second is `23:59:59`, and `25:00:00` is `01:00:00` the next day. Do not write `main`.
