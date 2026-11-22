Implement a class that builds text incrementally.

```cpp
class TextBuilder {
public:
    TextBuilder();
    explicit TextBuilder(const std::string& initial);

    TextBuilder& append(const std::string& text);
    TextBuilder& append(char c);
    TextBuilder& operator+=(const std::string& text);
    TextBuilder operator+(const std::string& text) const;

    const std::string& str() const;
    int size() const;
    bool empty() const;
    void clear();

    char& operator[](int index);
    const char& operator[](int index) const;

    bool operator==(const TextBuilder& other) const;
    bool operator<(const TextBuilder& other) const;
};

std::ostream& operator<<(std::ostream& os, const TextBuilder& builder);
```

Rules:

- Default construction gives an empty string; `TextBuilder(initial)` starts from the given text.
- `append` and `+=` add text at the end and return a reference to `*this`, so calls can be chained.
- `operator+` returns a **new object** containing the original text plus the argument; the original is unchanged. It is a `const` member function.
- `str` returns a const reference to the internal string; `size` returns the character count; `empty` tests for emptiness; `clear` empties it.
- `operator[]` allows reading and writing and is only called with a valid index; the const overload returns `const char&`.
- `==` compares text content; `<` compares content lexicographically.
- `operator<<` writes the internal text to the stream and returns that stream.
- Do not write `main`.

### Hint

Keep a `std::string` inside. `operator+` can copy `*this`, append to the copy, and return it. `operator<<` is a free function: write `os << builder.str()`.
