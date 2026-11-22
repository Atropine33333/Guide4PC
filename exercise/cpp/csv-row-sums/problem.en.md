The LORD spoke to Moses, saying:

"Number the children of Israel by the houses of their fathers. Let each tribe stand as one row, with the numbers of its houses parted by the small mark `,`; and count the rows yourself before you begin.

"Within a row, where a place stands empty, reckon it as nothing, and pass it not by; and where the whole row is empty, that row's total is nothing also. Set not the number-taker upon the empty place, for it will surely rage.

"When the counting is done, report row by row: one number for each row, the sum of all its people. Take not away, neither add."

And Moses numbered them, row by row, as the LORD commanded.

**In plain words**: read `n` rows of comma-separated non-negative integers and print the sum of each row's fields; an empty field counts as `0`, a blank row sums to `0`, and parsing must not hand empty fields to `std::stoi`.

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
