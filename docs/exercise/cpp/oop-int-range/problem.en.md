This task reads no stdin and writes no stdout. Implement an integer range usable in range-`for` loops and standard algorithms.

```cpp
#include <cstddef>
#include <iterator>

class IntRange {
public:
    class iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = const int*;
        using reference = int;

        iterator();
        explicit iterator(int value);

        int operator*() const;
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
    };

    IntRange(int first, int stop);
    iterator begin() const;
    iterator end() const;
};
```

Rules:

- The range contains every integer from `first` up to but not including `stop`, with step 1.
- When `first >= stop`, the range is empty and `begin() == end()`.
- Support range-`for`, `std::distance`, `std::accumulate`, and constructing a `std::vector` from the iterators.
- The iterator must support pre-increment, post-increment, dereference, `==`, and `!=`.
- Range objects are copyable, and iterating must not modify the range itself.
- Do not write `main`, and do not change the public interface.
