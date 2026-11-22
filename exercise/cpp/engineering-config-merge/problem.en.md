Read several configuration lines, merge them, and print them sorted by key.

**Input**: lines until end of file. Each line is one of:

- An empty line or a line containing only whitespace: ignore it;
- A line whose first non-whitespace character is `#`: a comment, ignore it;
- `key=value`: a configuration entry.

`key` and `value` may have spaces or tabs around them; the value itself may contain `=` (split at the first `=` only). Keys are case-sensitive and values may be empty. If a key appears more than once, the later value wins.

**Output**: one line per key in the form `key=value`, sorted lexicographically by key (ASCII ascending). Do not add extra spaces; an empty value still prints `key=`.

### Example

Input:

```text
# settings
b=2
a=1
b=3
```

Output:

```text
a=1
b=3
```

### Constraints

- At most 200 configuration lines, each at most 200 characters;
- Do not assume the input is already sorted;
- Use `std::getline` for full lines and `std::map` to keep keys ordered.
