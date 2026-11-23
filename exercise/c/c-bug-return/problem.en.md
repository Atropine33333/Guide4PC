(Above the Hao bridge. Zhuangzi and Huizi sit by the water.)

Huizi: I have a reckoning: one times two, then times three, on up to nine; the product is three hundred sixty-two thousand eight hundred eighty. How is that?

Zhuangzi: The number is right. But where have you put it?

Huizi: Here, closed in my hand.

Zhuangzi: Held in the hand is not handed over. A sum kept back is a sum not done.

Huizi: Then what should I do?

Zhuangzi: Give it back the way it came.

**In plain words**: read one integer `n` and print `n!`. The `factorial` in `starter.c` works out the product but never hands the result back; supply that one step and the program is right.

**Input**: one line with a single integer `n` (`0 ≤ n ≤ 12`).

**Output**: one line with the value of `n!`.

### Example

Input:

```text
5
```

Output:

```text
120
```

### Contract

- Both `0!` and `1!` equal `1`.
- `12! = 479001600`, which fits in `int`.
- Change only the faulty spot in `factorial`; keep the input and output formats as they are.
- Print one line ending in a newline.
