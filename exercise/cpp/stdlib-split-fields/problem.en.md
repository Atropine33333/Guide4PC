This exercise reads no stdin and writes no stdout. Implement two delimiter-based string functions; a hidden harness runs the tests.

```cpp
std::vector<std::string> split_fields(const std::string& text, char delimiter);
std::string join_fields(const std::vector<std::string>& fields,
                        const std::string& separator);
```

Rules:

- `split_fields` splits on the single character `delimiter`; every delimiter starts a new field, so consecutive delimiters produce empty fields.
- An empty string splits into an **empty vector**, not a vector holding one empty string.
- A delimiter at the start or the end produces a leading or trailing empty field.
- When the delimiter never occurs, the whole string is the only field.
- Spaces, tabs, newlines, and every other character inside a field are kept as-is.
- `join_fields` concatenates the fields with `separator` between neighbours; an empty vector gives an empty string; empty fields are kept; `separator` may be empty.
- Do not write `main`.

### Hint

Use `std::string::find` to scan from left to right, or `std::getline` with a `std::stringstream`; either way handle the empty input and the trailing delimiter explicitly.
