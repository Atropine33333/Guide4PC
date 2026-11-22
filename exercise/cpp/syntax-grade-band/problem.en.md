Read an integer score and print the matching uppercase letter grade.

**Input**:

- One integer `score` on a single line (`0 ≤ score ≤ 100`).

**Output**: one uppercase letter.

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

- Scores outside `0`–`100` will not appear.
- The boundary values `90`, `80`, `70`, and `60` belong to the higher band.
