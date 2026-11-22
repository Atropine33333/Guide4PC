THE QIWU CHAPTER: EVEN AND ODD

Master Meng sat by the river Hao. A disciple asked: "Do numbers have even and odd?"

The Master said: "To know the even, first ask the odd; to know the odd, first ask the even. The even less one is odd, the odd less one is even, each begetting the other like a ring without end."

The disciple asked: "And when it comes to zero?"

The Master said: "Zero is even. Ask the even and the even answers; ask no further."

The disciple asked: "And a thousand?"

The Master said: "Ask odd of even and even of odd, peeling away layer by layer until it rests at zero. A thousand questions are still one question."

**In plain words**: implement two functions that call each other. `is_even(0)` returns `1` and `is_even(n)` returns `is_odd(n - 1)`; `is_odd(0)` returns `0` and `is_odd(n)` returns `is_even(n - 1)`. Read `n`; print `even` when `n` is even, `odd` otherwise.

**Input**: one line with an integer `n` such that `0 ≤ n ≤ 1000`.

**Output**: one line, either `even` or `odd`.

### Example

Input:

```text
7
```

Output:

```text
odd
```

### Contract

- The two functions must call each other: `is_odd` calls `is_even`, and `is_even` calls `is_odd`.
- The recursion goes at most `1001` levels deep, so plain recursion is fine.
- `0` counts as even.
- Lowercase output, no trailing spaces at the end of the line.
