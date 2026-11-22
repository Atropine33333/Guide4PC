The terminal of the starship *Farwatch* keeps scrolling. After the supply bay rebooted, the configuration table came back as raw lines: some carry a comment in front, some are nothing but blank space, some have whitespace clinging to both the key and the value. The duty officer wants a clean echo — every setting that actually takes effect, in the order it appeared.

**In plain words**: read `n` lines of configuration log, skip blank lines and comment lines; for every remaining line, split at the first `=` into a key and a value, trim both ends, and print `key: value`. Duplicate keys are not merged; print each one as it comes.

**Input**:

The first line holds an integer `n` (`1` ≤ `n` ≤ `100`).

Then come `n` lines of configuration log, each at most `200` characters long and containing at least one `=`.

**Output**:

For every surviving line, print `key: value` with exactly one space after the colon, in the order the lines appeared.

### Example

Input:

```text
4
# 补给舱重启
  port = 8080
# 频道已校准
mode=巡航
```

Output:

```text
port: 8080
mode: 巡航
```

### Contract

- A line whose trimmed form is empty counts as blank and is skipped.
- A line whose trimmed form starts with `#` counts as a comment and is skipped whole.
- Whitespace means spaces and tabs; trim the key and the value separately.
- If a line holds several `=`, split at the first one only; the value keeps the rest.
- In a surviving line, both the trimmed key and the trimmed value are non-empty.
- Duplicate keys are printed line by line; no overwriting and no de-duplication.
