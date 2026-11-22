Guide4PC Exercise Bureau

Document No. 9 of 2026

NOTICE ON ISSUING THE MEASURES FOR REGISTERING USERNAMES

To all implementing units:

　　To regulate the registration of usernames, the Measures for Registering Usernames are hereby issued. Please comply.

　　Article 1. A username is between 3 and 16 characters long, both ends included.

　　Article 2. The first character is an ASCII letter, `a`–`z` or `A`–`Z`.

　　Article 3. Every remaining character is an ASCII letter, a digit, or an underscore.

　　Article 4. For a name that fails these measures, `create` reports `std::nullopt`; for one that passes, it reports a `std::optional` holding the name.

　　Article 5. The constructor is private. No outside party may build an object directly; registration through `create` is the only way.

　　Article 6. `value()` reports the stored name, `length()` its character count, and `operator==` compares contents.

　　Article 7. These measures take effect on the day of issue.

**In plain words**: Implement the username class `Username`, creatable only through its static factory: `create` checks the length (3 to 16), then that the first character is an ASCII letter, then every remaining character; a valid name yields a `std::optional` holding it, an invalid one yields `std::nullopt`. The constructor is private; `value()` and `length()` read the contents and `operator==` compares the strings.

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

### Example

| Call | Result |
| --- | --- |
| `Username::create("alice_01")` | has a value, `length()` is 8 |
| `Username::create("ab")` | `std::nullopt` |
| `Username::create("1alice")` | `std::nullopt` |
| `Username::create("alice!")` | `std::nullopt` |

### Hint

Check the length first, then the first character, then walk the rest. A `std::optional` can be constructed directly from a `Username`, and `create` can reach the private constructor because it is a member. Beware that `std::isalpha` and `std::isdigit` are undefined for negative `char` values; compare ASCII ranges directly or cast to `unsigned char` first.
