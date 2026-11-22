Given a camel-case identifier without spaces, split it into lowercase words separated by one space.

Splitting rules:

1. A run of consecutive uppercase letters is an acronym; when such a run is immediately followed by a lowercase letter, the last uppercase letter of the run starts the next word.
2. An uppercase letter that follows a lowercase letter or a digit starts a new word.
3. Digits stay with the preceding word.
4. All letters are converted to lowercase.

Examples:

| Input | Output |
| --- | --- |
| `camelCase` | `camel case` |
| `HTTPServer` | `http server` |
| `XMLHttpRequest` | `xml http request` |
| `getHTTPResponseCode` | `get http response code` |
| `parse2Json` | `parse2 json` |
| `JSON2XML` | `json2 xml` |

**Input**:

- One identifier on a single line, containing only ASCII letters and digits, starting with a letter, length `1` to `60`.

**Output**:

- One line with the words separated by a single space.

### Example

Input:

```text
getHTTPResponseCode
```

Output:

```text
get http response code
```

### Constraints

- The input is a single word: no spaces or punctuation.
- The output must not contain leading, trailing, or repeated spaces.
- Note `HTTPServer`: `S` ends the acronym `HTTPS` and starts the word `Server`, so `http` and `server` are two words.
