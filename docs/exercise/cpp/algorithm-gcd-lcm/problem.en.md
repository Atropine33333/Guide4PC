> A field to pave with tiles so neat,
> subtract the less, repeat the feat;
> two drums will beat their separate ways,
> when do they fall in step and praise?

The *Nine Chapters* has a rule: take the less from the greater, again and again, until the two are equal.

A field is `a` paces wide and `b` paces long. The magistrate wants to pave it with identical square tiles, as large as possible — how long is a tile's side? Two drums are also set up: drum A sounds once every `a` ticks, drum B once every `b` ticks. Both sound at tick zero; at which tick do they first sound together again?

**In plain words**: for each pair `a`, `b`, output their greatest common divisor (the tile side) and least common multiple (the first joint beat), in that order.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 1000`), the number of fields-and-drums.
- The next `n` lines each contain two integers `a` and `b` (`0 ≤ a, b ≤ 10^9`).

**Output**: `n` lines with two integers separated by a single space: the tile side (the largest square tile) and the first joint beat.

### Example

Input:

```text
2
12 18
7 5
```

Output:

```text
6 36
1 35
```

### Constraints

- If `a = 0` or `b = 0`, treat it as a field missing a side and a silent drum: the tile side is the other number; if both are 0, the tile side is 0.
- If either number is 0, the first joint beat is recorded as 0.
- The first joint beat may be close to `10^18`; store it in a 64-bit integer.
- Divide before multiplying to avoid overflowing an intermediate result.
