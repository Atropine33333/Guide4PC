Guide4PC Exercise Bureau

Document No. 7 of 2026

NOTICE ON STRENGTHENING THE MANAGEMENT OF TEXT-STATISTICS CACHES

To all implementing units:

　　To reduce duplicated labour and raise statistical efficiency, the following provisions govern caching in the `TextStats` class.

　　I. `word_count()` and `letter_count()` are const member functions. Their first call must store its result in a cache, so every cache member is declared `mutable`.

　　II. While the text is unchanged, repeated calls read the cache only and `computations()` must not increase; each cache fill raises `computations()` by one.

　　III. After `append()` alters the text, both caches are void at once and the next query recomputes.

　　IV. Only English letters are counted; digits, spaces and punctuation are all excluded.

　　V. The text is at most 100000 characters long; thread safety is not required.

　　Please comply.

**In plain words**: Complete the `TextStats` class: `word_count()` and `letter_count()` are `const`, and the first call writes its result into a `mutable` cache; while the text is unchanged later calls return that cache and leave `computations()` alone, and every cache fill raises `computations()` by one. `append()` invalidates both caches, so the next query recomputes. A word is a maximal run of English letters.

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
