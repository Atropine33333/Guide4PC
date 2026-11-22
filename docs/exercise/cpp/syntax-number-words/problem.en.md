Read an integer and write it in English words. Use lowercase words separated by single spaces.

For example, `42` is `forty two`, `100` is `one hundred`, and `1001` is `one thousand one`. Do not use `and` and do not use hyphens.

### Input

One line with an integer `n` such that `0 <= n <= 999999`.

### Output

One line containing the English name of `n`.

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

- `0` is written as `zero`.
- `10` through `19` use `ten`, `eleven`, `twelve`, `thirteen`, `fourteen`, `fifteen`, `sixteen`, `seventeen`, `eighteen`, and `nineteen`.
- Multiples of ten use `twenty`, `thirty`, `forty`, `fifty`, `sixty`, `seventy`, `eighty`, and `ninety`.
- Use exactly one space between the thousands part and the hundreds part, and one space between the hundreds part and the last two digits.
- `100000` is `one hundred thousand`.
- Use only the standard library.
