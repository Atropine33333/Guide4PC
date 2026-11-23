FOUR CARDS

The fortune teller lays four cards on the table: `+`, `-`, `*` and `max`. Under each card sit two integers `a` and `b`.

The visitor names the card they drew, and the teller computes with that face: `+` adds the two numbers, `-` subtracts the second from the first, `*` multiplies them, and `max` keeps the larger one. If the named card is not on the table at all, the teller slides back a blank `?`.

**In plain words**: read a card name `op` and two integers `a` and `b`; for `+`, `-`, `*` or `max` print the result of that operation, and for any other name print `?`.

**Input**: one line holding `op`, `a` and `b`, separated by spaces. `op` is a short string without spaces, at most `15` characters; `|a| ≤ 10^6` and `|b| ≤ 10^6`.

**Output**: one line with a single result: the value of the operation, or `?` when the card name is unknown.

### Example

Input:

```text
* 6 7
```

Output:

```text
42
```

The card drawn is `*`, and the product is `42`.

### Contract

- `max` compares numeric values, negatives included: `max -9 -2` gives `-2`.
- A product may exceed the range of `int`; keep the result in a wider integer type before printing.
- Print one integer or one `?`, with no trailing space.
- Card names are compared as whole strings; `+` and `max` are separate cards.
