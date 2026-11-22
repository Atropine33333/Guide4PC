Oh, my dear friend, there you are at last. Do sit down, make yourself at home — no ceremony, please.

Here is the situation: we have two old machines, one called `split_fields`, the other `join_fields`. The first takes a piece of text and a single character, and cuts the text into fields at that character. Every delimiter opens a new field; two delimiters side by side leave an empty field between them. A delimiter at the start makes the first field empty; one at the end makes the last field empty. But what if the whole text is empty? Then there are no fields at all: it hands back an empty box, absolutely empty, with not one empty field inside.

The second machine does the opposite: give it a list of fields and a separator, and it joins the fields one by one with the separator between them. Empty fields are joined all the same; an empty box yields an empty string; the separator itself may be empty, and God knows, it will not complain.

And do not write your own `main`.

**In plain words**: Implement `split_fields` and `join_fields`. `split_fields` splits on the single character `delimiter`; every delimiter starts a new field, and consecutive delimiters produce empty fields. An empty string splits into an empty `vector`; a delimiter at the start or end produces a leading or trailing empty field; with no delimiter the whole string is the only field, and spaces, tabs, newlines and every other character inside a field are kept as-is. `join_fields` joins the fields with `separator`; an empty `vector` gives an empty string, empty fields are kept, and `separator` may be empty. Do not write `main`.

```cpp
std::vector<std::string> split_fields(const std::string& text, char delimiter);
std::string join_fields(const std::vector<std::string>& fields,
                        const std::string& separator);
```

### Example

| Call | Result |
| --- | --- |
| `split_fields("a,b", ',')` | `{"a", "b"}` |
| `split_fields("", ',')` | `{}` |
| `split_fields(",a,", ',')` | `{"", "a", ""}` |
| `join_fields({"a", "", "b"}, "-")` | `"a--b"` |

### Hint

Use `std::string::find` to scan from left to right, or `std::getline` with a `std::stringstream`; either way handle the empty input and the trailing delimiter explicitly.
