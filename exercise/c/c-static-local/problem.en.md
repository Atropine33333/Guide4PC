**COUNTER NO. 1: THE AUTOMATIC TICKET DISPENSER**

> Halfway through the queue the old machine forgets everything and hands out the same number twice.
> This one does not forget. Press once for a number; press again and it counts on. `1`, `2`, `3` — no repeats, no gaps.
> Shift change? Press reset and it starts again from `1`. Press reset twice in a row and it still starts from `1`, no more, no less.
> Unlimited tickets, any hour, the counter never closes.

**In plain words**: implement two functions. `next_id` returns a number one greater than the previous one each time it is called, starting at `1`; `reset_ids` sets the count back so the next `next_id` returns `1` again. The number must survive between calls.

**Interface**:

```c
int next_id(void);
void reset_ids(void);
```

- `next_id`: returns `1`, `2`, `3`, ... in order; the value fits in an `int`.
- `reset_ids`: sets the counter back to zero, so the very next `next_id` returns `1`. Calling it several times in a row is the same as calling it once.

### Example

Call `next_id` three times and get `1`, `2`, `3`; call it once more and get `4`. After `reset_ids`, `next_id` returns `1` again.

### Contract

- The counter outlives the function call: use static storage duration (a `static` local variable, or a file-scope `static` variable).
- Implement only these two functions; do not write `main`.
- The number grows within `int` between resets, so it cannot overflow.
