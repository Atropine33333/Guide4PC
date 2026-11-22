(Late at night, an office buried in printouts. The telephone rings.)

"Oh, my dear, there you are at last. Listen: there is a stack of configuration on the table. Read it from the first line to the last, until the file ends."

"What does a line look like?"

"Three kinds. An empty line, or a line of nothing but spaces and tabs — good heavens, ignore it. A line whose first non-blank character is `#` — that is a remark, ignore it too. The rest is `key=value`. Simple as that."

"What if there are spaces around the key and the value?"

"Allowed, my dear, spaces and tabs both. The value itself may contain an equals sign; split at the first one only. Keys are case-sensitive, and a value may be empty."

"And if the same key appears twice?"

"The later one wins. Remember: the later one."

"And the output?"

"One line per key, `key=value`, sorted by key in lexicographic order, ASCII ascending. No extra spaces, my dear. An empty value still prints as `key=`. That is all. Do not bungle it."

**In plain words**: Read configuration lines until end of file, skip blank lines and `#` comments, split each entry at the first `=` and trim both sides, let the later value of a key win, and print `key=value` with `std::map` in ASCII order.

### Input

Lines until end of file. Each line is one of:

- An empty line or a line containing only whitespace: ignore it;
- A line whose first non-whitespace character is `#`: a comment, ignore it;
- `key=value`: a configuration entry.

`key` and `value` may have spaces or tabs around them; the value itself may contain `=` (split at the first `=` only). Keys are case-sensitive and values may be empty. If a key appears more than once, the later value wins.

### Output

One line per key in the form `key=value`, sorted lexicographically by key (ASCII ascending). Do not add extra spaces; an empty value still prints `key=`.

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
