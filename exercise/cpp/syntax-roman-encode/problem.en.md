Read an integer `n` and write it as an uppercase Roman numeral.

**Input**: one integer `n` (`1 ≤ n ≤ 3999`).

**Output**: one line with the Roman numeral for `n`, using only the uppercase letters `I`, `V`, `X`, `L`, `C`, `D`, and `M`.

### Example

Input:

```text
1994
```

Output:

```text
MCMXCIV
```

### Constraints

- Symbol values are `I=1`, `V=5`, `X=10`, `L=50`, `C=100`, `D=500`, `M=1000`.
- Use subtractive notation for 4, 9, 40, 90, 400, and 900: `IV`, `IX`, `XL`, `XC`, `CD`, `CM`.
- No symbol repeats more than three times in a row (`M` appears at most three times here).
- Do not print leading or trailing spaces.
