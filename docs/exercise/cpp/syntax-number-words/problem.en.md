"Oh, my dear," said the old dubbing actor, pressing the script to his chest, "the audience wants a voice. Type a row of Arabic figures on the screen and you have subtitles, not dialogue."

He cleared his throat. "Forty-two, you say `forty two`. One hundred, `one hundred`. One thousand and one, `one thousand one`. No `and` in between, no hyphens — good heavens, that rule was laid down the year I entered this trade."

"Tens and teens each have their own name, and not one may be mispronounced. Zero is `zero`. All lowercase, one space between words, no more and no less."

**In plain words**: read an integer `n` and print its English wording on one line: all lowercase, single spaces between words, no `and`, no hyphens.

**Input**: one integer `n` on a line, with `0 <= n <= 999999`.

**Output**: one line holding the English wording of `n`.

### Example

Input:

```text
123456
```

Output:

```text
one hundred twenty three thousand four hundred fifty six
```

### Constraints

- `0` prints as `zero`.
- `10` to `19` use `ten`, `eleven`, `twelve`, `thirteen`, `fourteen`, `fifteen`, `sixteen`, `seventeen`, `eighteen`, `nineteen`.
- Whole tens use `twenty`, `thirty`, `forty`, `fifty`, `sixty`, `seventy`, `eighty`, `ninety`.
- Exactly one space stands between the hundreds and the thousands, and between the hundreds and the units.
- The literal `100000` is `one hundred thousand`.
- Use the standard library only.
