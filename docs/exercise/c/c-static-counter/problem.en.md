(The storyteller raps the table. On stage there is only a desk and a ledger.)

Storyteller: At the corner of the street stands a small counting house. A clerk sits behind the counter, and he keeps one ledger. Every visitor reads the same ledger.

Listener: How does he enter a sum?

Storyteller: For each arrival he adds one and announces the new total. For each refund he subtracts that amount and announces what remains. Someone who only asks gets the current total, and the ledger stays as it is.

Listener: And when the house opens anew?

Storyteller: He says one word, "clear", and the ledger returns to zero. Four clerks take turns at the same counter, and they all write in the same ledger.

**In plain words**: Implement four functions that share one `static` counter. The counter is `0` when the program starts: `counter_next` adds one and returns the new value, `counter_add` adds `delta` and returns the new value, `counter_get` returns the current value without changing it, and `counter_reset` sets it back to zero.

**Interface**:

```c
void counter_reset(void);
int counter_next(void);
int counter_add(int delta);
int counter_get(void);
```

- `counter_reset`: set the counter to `0`.
- `counter_next`: add `1` and return the new value.
- `counter_add`: add `delta` (positive, negative or zero) and return the new value.
- `counter_get`: return the current value and change nothing.

### Example

The counter starts at `0`: `counter_get()` returns `0`. After two calls to `counter_next()` it is `2`. After `counter_add(-5)` it is `-3`. After `counter_reset()`, `counter_get()` returns `0` again.

### Contract

- Keep the count in a file-scope `static int`; all four functions read and write the same state.
- The counter is `0` when the program starts and keeps its value between calls.
- `delta` may be positive, negative or `0`; the arithmetic does not overflow.
- Implement only these four functions; do not write `main`, and print nothing beyond what the task asks.
