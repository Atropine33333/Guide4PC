Read `n` rows of comma-separated non-negative integers and print the sum of the fields in each row.

**Input**:

- The first line contains an integer `n` (`1 ≤ n ≤ 100`).
- The next `n` lines consist of digits `0`–`9` and commas. An empty field means the value `0`; a completely blank row sums to `0`.
- Each row has at most 30 fields, and each field has at most 6 digits.

**Output**: `n` lines; line `i` is the sum of the fields of input row `i`.

### Example

Input:

```text
3
1,2,3
4,5
6
```

Output:

```text
6
9
6
```

### Hint

After reading `n`, a newline remains at the end of the line; consume it with one `std::getline` before reading the rows. Do not feed empty fields to `std::stoi`.
