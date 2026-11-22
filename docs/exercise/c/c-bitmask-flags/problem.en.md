Eight small lamps hang on a wall, numbered `0` to `7`, all dark at first. Commands arrive one by one: `set k` lights lamp `k`, `clear k` puts it out, `toggle k` flips it, and `test k` only asks whether it is lit. The eight lamps together form an 8-bit mask.

**In plain words**: keep a mask that starts at `0`, with bit `k` standing for lamp `k`. `set k` turns that bit to `1`, `clear k` turns it to `0`, `toggle k` flips it, and `test k` leaves the mask alone while reporting whether the bit is `0` or `1`.

**Input**: several lines, each one of `set k`, `clear k`, `toggle k`, `test k`, where `0 ≤ k ≤ 7`. There are between `1` and `1000` commands, read until the end of input.

**Output**: one line per command. `set`, `clear` and `toggle` print the mask after the change, in decimal; `test` prints the current mask, a space, and the bit's `0` or `1`.

### Example

Input:

```text
set 3
set 0
test 3
clear 3
toggle 7
test 2
```

Output:

```text
8
9
9 1
1
129
129 0
```

### Contract

- The mask starts at `0`; every lamp is dark.
- Only the low 8 bits are used, so the mask never exceeds `255`; `unsigned int` is enough.
- `test` does not change the mask; setting the same bit twice and clearing an already clear bit must still print the usual line.
- No trailing spaces at the end of a line.
