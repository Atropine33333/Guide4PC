Implement a sequence class template and add deduction guides so callers can omit template arguments:

```cpp
template <typename T>
class Seq {
public:
    Seq() = default;
    Seq(std::initializer_list<T> items);
    template <typename It>
    Seq(It first, It last);
    template <typename U, std::size_t N>
    Seq(const U (&items)[N]);

    std::size_t size() const;
    T sum() const;                      // returns T{} for an empty sequence
    bool contains(const T& value) const;
    const T& at(std::size_t index) const;

private:
    std::vector<T> items_;
};

template <typename It>
Seq(It, It) -> Seq<typename std::iterator_traits<It>::value_type>;

template <typename T, std::size_t N>
Seq(const T (&)[N]) -> Seq<T>;
```

Requirements:

- `Seq s{1, 2, 3}` deduces `Seq<int>`;
- `Seq q(v.begin(), v.end())` for a `std::vector<long long>` deduces `Seq<long long>`;
- `Seq a(raw)` for `long long raw[4]` deduces `Seq<long long>`;
- explicitly writing `Seq<int> empty;` must also work;
- `sum()` returns `T{}` for an empty sequence.

The deduced types (via `std::is_same_v`) and the member results must be correct. **Do not write `main`.**

### Input

None.

### Output

None.

### Example

```cpp
Seq s{1, 2, 3};               // Seq<int>
std::vector<long long> v{5};  // value_type of v.begin() is long long
Seq q(v.begin(), v.end());    // Seq<long long>
```

### Constraints

- Standard library only.
- Do not dodge deduction with a non-template alias of `Seq`.
