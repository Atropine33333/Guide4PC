Welcome to Room 32 of the unsigned corridor.

Every rule in this room is true. You need only decide whether to leave your common sense about signs at the door.

Rule one: every number in this room is a 32-bit unsigned integer, from 0 to 4294967295. There are no negative numbers and no sign bit.

Rule two: the binary form of `4294967295` is thirty-two 1s. Any visitor who reads it into a signed `int` voids rule one for himself and bears the consequences.

Rule three: count the 1s in each number's binary form. `0` has none. The trade calls this popcount.

Rule four: read `n` numbers and print one popcount per line, in input order. Print nothing else.

Rule five: the door opens from the outside. You may use `std::popcount` from `<bit>`, or count bit by bit. Reading into `unsigned long long` and then converting to `std::uint32_t` is the safest route.

If you begin to find these rules normal, return to the first sentence and read it again.

**In plain words**: Read `n` unsigned 32-bit integers and print the number of `1` bits in each binary representation (popcount). The values may exceed the positive range of a signed 32-bit `int`, so read into `unsigned long long` and convert to `std::uint32_t`; use `std::popcount` or a manual bit loop.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 20`).
- The second line contains `n` integers `v` (`0 ≤ v ≤ 4294967295`).

**Output**: `n` lines, each with the popcount of the corresponding value.

### Example

Input:

```text
2
0 4294967295
```

Output:

```text
0
32
```
