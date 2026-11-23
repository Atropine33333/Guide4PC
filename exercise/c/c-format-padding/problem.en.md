[GOLD-LETTERED SIGNS] You name the position, you name the width, and not a copper more.

Our shop sets type for signboards. Hand over a width and a price and we lay the number into the frame by the rule of `printf("%*d", width, n)`: a positive width pushes the number to the right, a negative width pushes it to the left, and a width narrower than the number lets it run past the frame with every character kept.

**In plain words**: read two integers `width` and `n`, then print one line: an opening bracket, `n` laid out at that width, and a closing bracket.

**Input**: one line with two integers `width` and `n`, where `-30 ≤ width ≤ 30` and `-100000 ≤ n ≤ 100000`.

**Output**: one line shaped as `[` + the laid-out number + `]`; the middle part is exactly what `printf("%*d", width, n)` prints, and a `width` of `0` pads nothing. The brackets stay as they are, so the two ends of the line are visible.

### Example

Input:

```text
6 -42
```

Output:

```text
[   -42]
```

`-42` takes three characters; width `6` leaves three to spare, and the spaces go on the left.

### Contract

- A positive width pads on the left (right aligned); a negative width pads on the right (left aligned); the fill character is a space.
- When the number is longer than the width it is printed whole, never truncated.
- No trailing spaces outside the brackets; spaces inside the brackets are printed as they are.
