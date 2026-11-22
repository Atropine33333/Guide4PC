Maintain a 64-bit flag register whose bits all start at 0. Read and execute a sequence of commands.

The commands are:

- `set i`: set bit `i` to 1.
- `reset i`: clear bit `i` to 0.
- `flip i`: toggle bit `i`.
- `test i`: print the value of bit `i` (`0` or `1`).
- `count`: print the number of bits currently set to 1.
- `first`: print the lowest index whose bit is 1, or `-1` if no bit is set.
- `last`: print the highest index whose bit is 1, or `-1` if no bit is set.
- `any`: print `1` if at least one bit is set, otherwise `0`.
- `fill`: set every bit to 1.

The index `i` satisfies `0 <= i < 64`, and bit 0 is the least significant bit.

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
