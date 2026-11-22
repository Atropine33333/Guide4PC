Implement two function templates that dispatch on types at compile time:

```cpp
template <typename T>
std::string classify(const T& value);

template <typename T>
std::string render(const T& value);
```

`classify` returns one of these strings based on `std::decay_t<T>`:

- `"integral"` for integer types (including `bool` and all `char`/`unsigned` types);
- `"floating"` for `float`, `double`, `long double`;
- `"string"` for `std::string`;
- `"other"` for everything else.

`render` produces the textual form of a value:

- integers: decimal, for example `42`, `-7`, and `true` renders as `"1"`;
- floating point: fixed with two decimals, so `3.14159` renders as `"3.14"` and `2.5` as `"2.50"`;
- `std::string`: returned unchanged;
- anything else: `"?"`.

Use `if constexpr` for compile-time dispatch (do not write a runtime `if` over type traits), and make sure the code still compiles when the passed type cannot participate in arithmetic.

These templates will be instantiated with many types. **Do not write `main`.**

### Input

None.

### Output

None.

### Example

```cpp
classify(42);                  // "integral"
classify(3.5);                 // "floating"
classify(std::string("hi"));   // "string"
render(3.14159);               // "3.14"
```

### Constraints

- Standard library only.
- Do not dodge the templates by writing one overload per type.
