*Code of the Three-State Value*

Article 1 (Purpose). To govern the construction, copying and destruction of a value that may be empty, an integer, or a string, this code is enacted.

Article 2 (The Three States). Default construction gives the empty state, whose `describe()` reports `"empty"`. The integer state reports `"int:"` followed by the decimal value, as in `"int:-7"`. The string state reports `"string:"` followed by the contents; an empty string is a lawful string state and reports `"string:"`.

Article 3 (Extraction). `as_int` yields the stored value only in the integer state (including `0`) and yields `fallback` otherwise; `as_string` yields the stored value only in the string state (including the empty string) and yields `fallback` otherwise.

Article 4 (Copying). A copy is a deep copy; altering the copy shall not touch the original. Assignment shall dispose of self-assignment.

Article 5 (Execution). Destruction, copying and assignment are all custom, whence this code is also called the rule of five. The representation is free — an abstract base held by `std::unique_ptr`, or any other device — but the semantics may not be broken.

Article 6 (Supplement). No `main` shall be written.

**In plain words**: Implement the three-state value type `AnyValue`: default-constructed it is empty, and it can also hold an integer or a string; `describe()` reports `"empty"`, `"int:-7"` or `"string:abc"` in that shape, while `as_int` and `as_string` yield the stored value only in the matching state (including `0` and the empty string) and `fallback` otherwise. Copies must be deep, assignment must handle self-assignment, and destruction, copying and assignment are all custom — a rule-of-five exercise.

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

### Hint

Keep the payload behind an abstract base class held by `std::unique_ptr`, and call a virtual `clone()` in the copy constructor and assignment operator; other type-erasure representations are equally acceptable. Because destruction, copying, and assignment are customised, this is a rule-of-five exercise.
