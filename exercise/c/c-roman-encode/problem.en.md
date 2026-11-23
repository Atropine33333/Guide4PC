Now hear this. In old Rome a stonecutter carved numbers into monuments, and he had only seven letters to work with: `I` for one, `V` for five, `X` for ten, `L` for fifty, `C` for a hundred, `D` for five hundred, `M` for a thousand.

He had a rule: ordinarily the letters stand from largest to smallest and their values add; but for numbers like four or nine, the smaller letter is carved to the left of the larger one and its value is subtracted. One thousand nine hundred eighty-seven he carved as `MCMLXXXVII`.

The listener asks: given any number, how does the craftsman cut it?

**In plain words**: read an integer `n` from `1` to `3999` and print its Roman numeral, in uppercase letters only.

**Input**: one line with an integer `n` (`1 ≤ n ≤ 3999`).

**Output**: one line with the Roman numeral of `n`, built from `I`, `V`, `X`, `L`, `C`, `D` and `M`.

### Example

Input:

```text
1987
```

Output:

```text
MCMLXXXVII
```

`M` is a thousand, `CM` nine hundred, `LXXX` eighty, and `VII` seven — one thousand nine hundred eighty-seven in all.

### Contract

- The only subtractive pairs are `IV`, `IX`, `XL`, `XC`, `CD` and `CM`.
- A letter never repeats more than three times — `III`, `XXX`, `CCC`, `MMM`.
- `n` never exceeds `3999`, so at most three `M`s appear and no five-thousand form is needed.
- No trailing spaces at the end of the line.
