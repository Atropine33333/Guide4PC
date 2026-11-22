> Three, then four, then five in line;
> what product, saith the clerk, is thine?
> The middle product breaks thirty-two bits,
> so fetch a sixty-four-bit vessel.

The *Nine Chapters* gives the method: set the three numbers down, multiply two of them to get the middle product, then multiply by the third to get the whole.

Now there are three numbers, each up to a thousand million (`10^9`). What is their product? The method says: the moment the middle product leaves a thirty-two-bit vessel it collapses; change to a sixty-four-bit vessel first, and the product will fit. Multiply in the narrow vessel and what you hold is wreckage, not a product.

**In plain words**: read three integers `a`, `b`, `c` and print `a × b × c`. Each has absolute value at most `10^9`, and the product is guaranteed to lie in `[-9 × 10^18, 9 × 10^18]`. Two of them multiplied already exceed a 32-bit `int`, so keep the intermediate in a signed 64-bit integer.

**Input**: one line with three integers, each with absolute value at most `10^9`. The product is guaranteed to lie in `[-9 × 10^18, 9 × 10^18]`.

**Output**: one line with the integer `a × b × c`.

### Example

Input:

```text
2 3 4
```

Output:

```text
24
```

### Constraints

- Multiplying two values near `10^9` already exceeds 32-bit `int`; do not multiply as `int` and convert afterward.
- The product may be negative or zero.
- Use a signed 64-bit integer for the intermediate result.
