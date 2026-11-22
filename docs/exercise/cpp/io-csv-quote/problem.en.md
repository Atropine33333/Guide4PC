*Code of CSV Field Writing and Parsing*

Article 1 (Scope) This Code governs the writing of a single field and the parsing of one CSV line. The required interface follows.

```cpp
#include <string>
#include <vector>

std::string csv_quote(const std::string& field);
std::vector<std::string> csv_split(const std::string& line);
```

Article 2 (Exemption from quotes) A field containing none of a comma `,`, a double quote `"`, and a newline `\n` is returned unchanged.

Article 3 (Quoting) A field containing any of the three is enclosed in one pair of double quotes, and every double quote already inside it is rewritten as two.

Article 4 (Separation) In parsing, commas separate fields; the content between two commas, possibly empty, is one field.

Article 5 (Quoted mode) A field beginning with a double quote enters quoted mode, where commas do not separate; two consecutive double quotes mean one real double quote.

Article 6 (Empty fields) An empty line yields one empty field; a trailing comma yields an empty final field.

Article 7 (Supplementary) Input is always well-formed as produced by Articles 2 and 3; unclosed quotes need no handling.

**In plain words**: implement `csv_quote` and `csv_split`; the former quotes a field when needed and doubles its inner quotes, the latter splits a line into fields according to commas and quoted mode.

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
