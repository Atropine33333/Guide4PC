Implement a text-statistics class whose const member functions cache their results. Complete this interface:

```cpp
class TextStats {
public:
    explicit TextStats(std::string text);

    void append(const std::string& extra);

    int word_count() const;     // words: each maximal run of letters
    int letter_count() const;   // total number of letters

    static int computations();  // total number of cache fills

private:
    std::string text_;
    // add cache members (they must be mutable) and a counter
};
```

Requirements:

- `word_count()` and `letter_count()` are `const` members, yet the first call stores its result in a cache, so the cache members must be `mutable`.
- As long as the text is unchanged, repeated calls return the cached value without increasing `computations()`; every cache fill increments `computations()` by one.
- `append()` changes the text, so both caches become invalid and the next query recomputes.
- Only English letters count; digits, spaces, and punctuation are not letters.

The judge includes your implementation and calls these members. **Do not write `main`.**

### Input

None.

### Output

None.

### Example

```cpp
TextStats stats("hello world");
stats.word_count();   // 2, computations() increases by one
stats.word_count();   // still 2, computations() unchanged
stats.append(" again");
stats.word_count();   // 3, cache recomputed
```

### Constraints

- The text is at most 100000 characters long.
- Thread safety is not required.
