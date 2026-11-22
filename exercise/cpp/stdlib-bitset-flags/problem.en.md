Gather round, honored guests, and hear of the sixty-four-bit tablet. On the storyteller's table it lies, its marks numbered 0 to 63, with 0 at the right hand, the least significant. At first the tablet is blank: all sixty-four cells hold 0.

Now `n` slips of paper come up from the audience, each bearing one order. The storyteller takes them in turn, and for each slip he acts:

- `set i`: turn cell `i` to 1.
- `reset i`: wipe cell `i` to 0.
- `flip i`: turn cell `i` over, 0 to 1 and 1 to 0.
- `test i`: call out whether cell `i` is now 0 or 1.
- `count`: count how many 1s the tablet bears.
- `first`: call the lowest lit index; if all are dark, call `-1`.
- `last`: call the highest lit index; if all are dark, call `-1`.
- `any`: call `1` if at least one cell is lit, otherwise `0`.
- `fill`: light all sixty-four cells.

But mark this: only `test`, `count`, `first`, `last`, and `any` speak aloud. The other four merely do their work and keep silent. Exactly one space separates a command from its index.

**In plain words**: Maintain a 64-bit flag register that starts at all zeros and execute `n` commands in order. `set`, `reset`, `flip`, and `test` take an index `i` (`0 <= i < 64`, with bit 0 the least significant); `test` prints that bit, `count` prints the number of 1s, `first` prints the lowest set index, `last` the highest, `any` prints whether a 1 exists, and `fill` sets every bit. When no bit is set, `first` and `last` both print `-1`. Print only results, never echo the commands. You may use `std::bitset<64>` or maintain a 64-bit unsigned integer yourself.

### Input

The first line contains an integer `n` (`1 <= n <= 1000`), the number of commands.

The next `n` lines each contain one command in the form above.

### Output

For every command that produces output, print one line in execution order. `test`, `count`, `first`, `last`, and `any` produce output; `set`, `reset`, `flip`, and `fill` do not.

### Example

Input:

```text
8
set 0
set 63
test 0
test 63
test 1
count
first
last
```

Output:

```text
1
1
0
2
0
63
```

### Constraints

- A command and its index are separated by exactly one space.
- You may use `std::bitset<64>` or maintain a 64-bit unsigned integer yourself.
- Print only the results; do not echo the commands.
