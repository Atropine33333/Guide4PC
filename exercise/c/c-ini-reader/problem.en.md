A simplified configuration file holds one `key=value` entry per line. Read it in, drop the comments and blank lines, flatten the whitespace around keys and values, keep only the last value of a repeated key, and print the entries sorted by key.

**Input**:

- Read from standard input until end of file; a line may carry leading or trailing spaces and tabs.
- A line whose first non-blank character is `#` is a comment and is skipped whole.
- A line without `=` is skipped.
- Split at the first `=` into key and value; remove the remaining spaces and tabs on either side of each.
- A line whose key is empty is skipped.
- When the same key appears more than once, the last value wins.
- At most 100 valid entries, keys up to 31 characters, values up to 63 characters, and lines up to 200 characters.

**Output**:

- One line per valid entry, written as `key=value`.
- Sort by key in ascending `strcmp` order.
- A value may be empty; the line then ends right after the `=`.
- When there is no valid entry, print nothing.

### Example

Input:

```
# note
b = 2
a = 1
a = 7
```

Output:

```
a=7
b=2
```

### Contract

- Use the standard library only and read line by line with `fgets`.
- Store keys and values in fixed-size arrays; no dynamic allocation.
- Print only the entry lines, each ending with a newline.
