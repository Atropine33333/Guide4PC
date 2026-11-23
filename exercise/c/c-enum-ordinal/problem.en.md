THE ORDINANCE OF ENUM NUMBERS

Article 1 (Scope). This Ordinance governs one enumeration only: `enum E { A, B = 5, C, D = 2 };`

Article 2 (Starting value). An enumerator without an assignment takes the value of the previous enumerator plus one; the first one starts at `0`.

Article 3 (Explicit assignment). An enumerator with an assignment takes exactly the number written, regardless of the previous enumerator.

Article 4 (Continuing). After an explicit assignment, the enumerators that follow continue from that new number, one step at a time.

Article 5 (Query). A word made of `A`, `B`, `C` and `D` is submitted; report the number of each enumerator in the order the letters appear.

Article 6 (Format). The numbers are decimal integers on one line, separated by single spaces; no space before the first letter and none after the last.

Article 7 (Length). The word is one to four letters long, and a letter may repeat.

**In plain words**: work out the integer value of `A`, `B`, `C` and `D` under the enumeration above, then print those values in the order the input letters arrive, separated by single spaces.

**Input**: one line holding a word made of `A`, `B`, `C` and `D`, with length from `1` to `4`.

**Output**: one line with as many integers as the word has letters, each the value of the matching enumerator, separated by single spaces.

### Example

Input:

```text
ABCD
```

Output:

```text
0 5 6 2
```

`A` comes first and starts at `0`; `B` is written as `5`; `C` has no assignment and follows `5` as `6`; `D` is written as `2`, which moves the continuing chain to `2`.

Input:

```text
DCBA
```

Output:

```text
2 6 5 0
```

### Contract

- The value of an enumerator comes from its position and its assignment, not from the alphabet.
- An explicit number interrupts the increment; enumerators after it continue from the new number.
- No trailing spaces at the end of the line.
- Print nothing beyond the answer.
