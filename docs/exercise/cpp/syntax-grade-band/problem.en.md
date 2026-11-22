**The Ordinance of Grade Bands**

Article 1. Every candidate's paper is marked with an integer, one hundred being full marks.

Article 2. A boundary value falls into the higher band: ninety, eighty, seventy and sixty each take the band above, and are never pushed down.

Article 3. Values beyond the stated range fall outside this Ordinance and need not be considered.

Article 4. The input is one score; the output is one capital letter, and nothing else.

**In plain words**: read an integer `score`, look up its capital-letter grade in the table, and print that single character.

**Input**: one integer `score` on a line (`0 ≤ score ≤ 100`).

**Output**: one capital letter.

| Score | Grade |
| --- | --- |
| 90–100 | `A` |
| 80–89 | `B` |
| 70–79 | `C` |
| 60–69 | `D` |
| 0–59 | `F` |

### Example

Input:

```text
89
```

Output:

```text
B
```

### Constraints

- Scores outside `0`–`100` need no handling.
- The boundary values `90`, `80`, `70` and `60` belong to the higher band.
