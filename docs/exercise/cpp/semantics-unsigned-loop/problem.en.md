Welcome to the Unsigned Corridor.

Every rule here is true. The corridor does not lie. You only have to decide whether you still believe the common sense you brought in.

Rule One: In the corridor, no number is less than zero. The `-1` you see is 4294967295 wearing an old coat. Treat it as that very large number.

Rule Two: Common sense says one step back from 0 reaches -1. There is no -1 in the corridor. `wrap_decrement(value, steps)` walks back `steps` steps from `value`; past zero it reappears from the 4294967295 end.

Rule Three: Common sense says counting down must reach 0 and then stop. In `countdown_loops(start)` there is exactly one brake: `i > 0`. When `start` is 0, this rule holds: the brake is pressed from the very start, and not one turn is made.

Rule Four: Before `unsigned_less(a, b)` compares anything, look at the sign of the left number. If `a` is negative, the answer is true and the comparison ends there; Rule Five does not apply.

Rule Five: If `a` is non-negative, Rule Four's sign shortcut is void. Put both `a` and `b` into the unsigned world and compare them by magnitude.

Rule Six: The corridor's door opens from the outside. Do not write `main`.

If you find these rules beginning to feel normal, return to the beginning and read the first sentence again.

**In plain words**: implement three functions: a safe comparison between signed and unsigned values, unsigned subtraction with wrap-around, and the number of iterations of an unsigned countdown loop. `start` is at most `INT_MAX`, so the count fits in `int`.

```cpp
bool unsigned_less(int a, unsigned b);
unsigned wrap_decrement(unsigned value, unsigned steps);
int countdown_loops(unsigned start);
```

### Example

| Call | Result |
| --- | --- |
| `unsigned_less(-1, 0)` | `true` |
| `unsigned_less(INT_MAX, 2147483648u)` | `true` |
| `wrap_decrement(0, 1)` | `4294967295` |
| `wrap_decrement(5, 3)` | `2` |
| `countdown_loops(5)` | `5` |
| `countdown_loops(0)` | `0` |

### Hint

When comparing a signed value with an unsigned one, handle `a < 0` first and then compare both as unsigned. Unsigned subtraction already wraps modulo 2³², so no manual branch is needed.
