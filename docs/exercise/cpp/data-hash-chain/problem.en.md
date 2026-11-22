B: Today I shall tell you a piece called "A Word Between Two Buckets". A: Wait — buckets? And what is kept in them?

B: Keys. Keys spelled from lowercase letters, one to eight letters long, each with its own weight. A: Where do the buckets come from?

B: The first line hands you a number `B`, from one to sixty-four, and that many buckets there are, numbered from 0. A: How does a key get in?

B: Add the ASCII codes of every character in the key, take the remainder upon division by `B`, and the result is the bucket number; `put k v` puts the key in together with its integer value `v`. A: In — but where inside?

B: At the **head** of that bucket. Whoever comes later stands in front, and the earlier ones step back. Put the same key in again and only its value changes, while its place stays; it has already taken its stand, and a change of clothes does not turn it out.

A: And `get`? B: `get k` reports the value of the key; if the table never had it, it reports `none`. A: And if one no longer wants a key? B: `erase k` takes it down and goes its way without a word; a key that was never there simply lets `erase` pass through. A: I should like a look inside. B: `dump` gives `B` lines; line `i` begins with `i:`, then writes the keys from the head of the chain onward with a single space between, and an empty bucket leaves only `i:` at the end.

A: Say something concrete. B: `ab` and `ba` both add up to 195; with `B` of 4 the remainder is 3, so both land in bucket 3; `a` alone is 97 and lands in bucket 1. A: Is that all? B: The commands number no more than two thousand, keys hold only lowercase letters, and no value exceeds a billion in absolute size; the results of `get` and `dump` come out one by one in the order the commands arrived.

**In plain words**: implement a chained hash table driven by commands that can also print each bucket's chain. The first line holds the bucket count `B`; `put k v` inserts a missing key at the **head** of its bucket's chain, and updates only the value of an existing key without moving it; `erase k` removes a key and does nothing when it is missing; `get k` prints the value or `none`; `dump` prints `B` lines, line `i` beginning with `i:` and then listing that bucket's keys from the head to the tail separated by single spaces. The hash is fixed: the sum of all character ASCII codes modulo `B`. Read until EOF.

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
