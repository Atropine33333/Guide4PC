Oh, my friend, there you are at last. Step under the lamp, don't be shy — the whole street is saying you can stack letters one on top of another into a pyramid.

Listen, here is how it goes. The very top holds a single `A`. The second layer down, read from left to right, is `ABA`: an `A` on each side and a `B` in the middle. The third is `ABCBA`, the fourth is `ABCDCBA`. Below that the rule simply carries on: the letters climb from `A` upward to the one that stands dead centre in that layer, then walk back down the same path. Every layer must sit squarely on the centre line, with just enough room to the left and not one space left over on the right — not one, I give you my word.

**In plain words**: read an integer `n` and print an `n`-layer letter pyramid. Line `i` (counting from `1`) carries `2i-1` letters: write `A` up to the `i`-th letter, then back down again; pad the line with `n-i` leading spaces and leave no trailing space.

**Input**: one line, one integer `n`, `1 ≤ n ≤ 9`.

**Output**: `n` lines, layer `i` laid out as described.

### Example

Input:

```text
3
```

Output:

```text
  A
 ABA
ABCBA
```

### Contract

- `n` never exceeds `9`; the centre letter of the bottom layer is at most `I`, so the alphabet never wraps around.
- Line `i` has exactly `n-i` leading spaces.
- No trailing spaces on any line, and no blank line after the last one.
