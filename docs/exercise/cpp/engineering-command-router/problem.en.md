Implement a small command routing table.

```cpp
#include <functional>
#include <string>

class CommandRouter {
public:
    using Handler = std::function<std::string(const std::string&)>;

    void add(const std::string& name, Handler handler);
    bool has(const std::string& name) const;
    std::string dispatch(const std::string& name, const std::string& argument) const;
    std::size_t size() const;
};
```

Rules:

- `add` registers a command; registering the same name again replaces the handler and does not increase `size()`.
- `has` reports whether a command is registered.
- `dispatch(name, argument)` calls the matching handler and returns its result; `dispatch` is a `const` member function.
- An unknown command returns the string `"unknown"` and calls no handler.
- A handler may capture external state (a counter, for instance); `dispatch` being `const` does not prevent the handler from modifying what it captured.
- Do not write `main`.

### Example

| Call sequence | Result |
| --- | --- |
| `dispatch("nope", "")` | `unknown` |
| `dispatch("echo", "hi")` after `add("echo", ...)` | `hi` |
| `add` the same name twice | `size()` is still 1 and the new handler runs |

### Hint

Store a `std::map<std::string, Handler>` and let `add` do `handlers_[name] = std::move(handler);`. In `dispatch`, `find` first and return `"unknown"` when missing, otherwise call `it->second(argument)`. Even though a `const` member function sees `handlers_` as const, `std::function::operator()` is itself const and can still be called.
