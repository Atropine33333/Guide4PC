Simulate a stream of commands on a double-ended queue. The deque starts empty; execute the commands in order.

**Input**:

- The first line contains an integer `m` (`1 ≤ m ≤ 1000`), the number of commands.
- The next `m` lines each contain one command in one of these forms:
  - `push_front x`: put integer `x` at the front;
  - `push_back x`: put integer `x` at the back;
  - `pop_front`: remove the front element;
  - `pop_back`: remove the back element;
  - `size`: query the number of elements.

The absolute value of `x` is at most `10^9`.

**Output**: for every `pop_front` or `pop_back`, print the removed integer, or `EMPTY` if the deque is empty. For every `size`, print the current element count. Each answer goes on its own line; other commands produce no output.

### Example

Input:

```text
6
push_back 1
push_back 2
push_front 0
size
pop_front
pop_back
```

Output:

```text
3
0
2
```

### Constraints

- A pop command may arrive while the deque is empty.
- `std::vector::pop_back` is handy but cannot remove from the front efficiently; use `std::deque`.
- Elements may repeat; do not assume they are distinct.
