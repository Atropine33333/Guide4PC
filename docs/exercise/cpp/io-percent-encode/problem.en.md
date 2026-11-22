Read one whole line of text (it may contain spaces or be empty) and print its percent-encoded form.

Encoding rule: print these characters unchanged:

- uppercase `A`–`Z` and lowercase `a`–`z`
- digits `0`–`9`
- `-`, `_`, `.`, `~`

For every other character, print its byte as `%` followed by two uppercase hexadecimal digits (a space becomes `%20`, `+` becomes `%2B`).

### Input

One line of text; the trailing newline is not part of the content. The input contains printable ASCII only.

### Output

The encoded line, followed by a newline.

### Example

Input:

```
a b/c?
```

Output:

```
a%20b%2Fc%3F
```

### Constraints

- Case matters: hex letters must be uppercase.
- Print no extra spaces or prompts.
