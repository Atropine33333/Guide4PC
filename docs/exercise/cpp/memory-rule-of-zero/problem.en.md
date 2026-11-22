This exercise reads no stdin. Implement a class that owns several lines of text and relies on compiler-generated special members.

```cpp
#include <string>
#include <vector>

class Notebook {
public:
    void add(const std::string& line);
    std::size_t size() const;
    std::string at(std::size_t index) const;
    std::string join(const std::string& separator) const;
};
```

Rules:

- Store exactly one `std::vector<std::string>` internally.
- Do **not** declare a destructor, copy constructor, copy assignment operator, move constructor, or move assignment operator; let the compiler generate them (the rule of zero).
- `add` appends a line.
- `at` returns line `index` (zero-based), or `"?"` when out of range.
- `join` connects all lines with `separator`; with no lines it returns an empty string.
- Do not write `main`.

The harness checks that a copy is independent of the original and that a moved-from notebook leaves the target with the original content.
