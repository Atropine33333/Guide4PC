This exercise reads no stdin and writes no stdout. Implement a username class that can only be built through a static factory, tested by a hidden harness.

```cpp
#include <optional>
#include <string>

class Username {
public:
    static std::optional<Username> create(const std::string& text);
    const std::string& value() const;
    std::size_t length() const;
    bool operator==(const Username& other) const;

private:
    explicit Username(std::string text);
    std::string text_;
};
```

Rules:

- A valid username has length between 3 and 16 inclusive.
- The first character must be an ASCII letter (`a`–`z` or `A`–`Z`).
- Every other character must be an ASCII letter, a digit, or an underscore.
- `create` returns `std::nullopt` when invalid and an optional holding the username when valid.
- `value()` returns the stored username and `length()` returns its length.
- `operator==` compares the two username strings.
- The constructor is private, so objects can only be created through `create`.
- Do not write `main`.

### Example

| Call | Result |
| --- | --- |
| `Username::create("alice_01")` | has a value, `length()` is 8 |
| `Username::create("ab")` | `std::nullopt` |
| `Username::create("1alice")` | `std::nullopt` |
| `Username::create("alice!")` | `std::nullopt` |

### Hint

Check the length first, then the first character, then walk the rest. A `std::optional` can be constructed directly from a `Username`, and `create` can reach the private constructor because it is a member. Beware that `std::isalpha` and `std::isdigit` are undefined for negative `char` values; compare ASCII ranges directly or cast to `unsigned char` first.
