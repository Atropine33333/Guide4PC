This exercise reads no stdin. Implement writing and parsing of one CSV line; a hidden harness calls both.

```cpp
#include <string>
#include <vector>

std::string csv_quote(const std::string& field);
std::vector<std::string> csv_split(const std::string& line);
```

### Writing (`csv_quote`)

- If the field contains none of a comma `,`, a double quote `"` or a newline `\n`, return it unchanged.
- Otherwise surround the field with one pair of double quotes and write every double quote inside the field as two double quotes.

### Parsing (`csv_split`)

- Commas separate fields; whatever lies between two commas (possibly empty) is a field.
- A field starting with a double quote enters quoted mode: commas inside are literal, and two consecutive double quotes mean one real double quote.
- An empty line yields one empty field; a trailing comma yields an empty final field.
- Input is always well-formed as produced by `csv_quote`; unclosed quotes need no handling.

### Example

| Call | Result |
| --- | --- |
| `csv_quote("abc")` | `abc` |
| `csv_quote("a,b")` | `"a,b"` |
| `csv_quote("say \"hi\"")` | `"say ""hi"""` |
| `csv_split("a,b,c")` | three fields `a`, `b`, `c` |
| `csv_split("\"a,b\",c")` | two fields `a,b`, `c` |
| `csv_split("a,")` | two fields `a`, empty |

### Hint

For writing, first decide whether quoting is needed, then build the string character by character. For parsing, keep an `in_quotes` flag and look at the next character when you meet a quote.
