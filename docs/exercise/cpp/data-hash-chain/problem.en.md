Implement a chained hash table driven by the given commands, with the ability to print the chains.

### Input

The first line holds the number of buckets `B`. Each following line is a command, until EOF:

- `put k v`: insert key `k` with integer value `v`. If `k` already exists, only update its value and keep its position in the chain; otherwise insert it at the **head** of its bucket's chain.
- `erase k`: remove key `k`; do nothing if it is missing.
- `get k`: print the value of `k`, or `none` if it is missing.
- `dump`: print `B` lines. Line `i` starts with `i:` and then lists the keys of that bucket from the head of the chain to the tail, separated by single spaces; an empty bucket prints just `i:`.

The hash function is fixed: the sum of the ASCII codes of all characters, modulo `B`.

### Output

For every `get` and `dump` command, in order.

### Example

```
4
put ab 1
put ba 2
put a 5
dump
```

Both `ab` and `ba` have character-code sum 195, so they land in bucket 3; `a` has code 97 and lands in bucket 1. Output:

```
0:
1: a
2:
3: ba ab
```

### Constraints

- `1 <= B <= 64`
- At most 2000 commands
- Keys are lowercase letters, length 1 to 8
- `|v| <= 1000000000`
