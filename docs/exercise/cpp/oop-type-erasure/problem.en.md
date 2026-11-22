This exercise reads no stdin. Implement a value type that can hold "empty", "integer", or "string", managing copies and assignment correctly.

```cpp
#include <string>

class AnyValue {
public:
    AnyValue();
    AnyValue(int value);
    AnyValue(const std::string& value);
    AnyValue(const AnyValue& other);
    AnyValue& operator=(const AnyValue& other);
    ~AnyValue();

    bool empty() const;
    bool is_int() const;
    bool is_string() const;
    int as_int(int fallback) const;
    std::string as_string(const std::string& fallback) const;
    std::string describe() const;
};
```

Rules:

- Default construction gives the empty state; `describe()` returns `"empty"`.
- In the integer state, `describe()` returns `"int:"` followed by the decimal value, such as `"int:-7"`.
- In the string state, `describe()` returns `"string:"` followed by the contents; an empty string is still a valid string state whose `describe()` is `"string:"`.
- `as_int` returns the stored value only in the integer state (including `0`); otherwise it returns `fallback`.
- `as_string` returns the stored value only in the string state (including the empty string); otherwise it returns `fallback`.
- Copies must be deep: modifying a copy must not affect the original. Assignment must handle self-assignment.
- Do not write `main`.

### Hint

Keep the payload behind an abstract base class held by `std::unique_ptr`, and call a virtual `clone()` in the copy constructor and assignment operator; other type-erasure representations are equally acceptable. Because destruction, copying, and assignment are customised, this is a rule-of-five exercise.
