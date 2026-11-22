And the LORD spake unto Moses, saying: "Number the congregation from `first` even unto `stop`, and all that lie between thou shalt not leave out. At `stop` thou shalt halt, and not pass over it.

"Let the order of the counting be changed by no man; one step forward only at each count. If the congregation already stand from the greater to the lesser, or the count hath come to its end, then that counting is empty; force it not.

"And they that count may each hold his own book, and count in rows and in ranks; the books shall not trouble one another, neither shall the camp be moved by the counting thereof."

And Moses did according to all that the LORD commanded him.

**In plain words**: Implement the integer range `IntRange` with an input iterator: it holds every integer from `first` up to but not including `stop`, step 1; when `first >= stop` it is empty and `begin() == end()`. It must work with range-`for`, `std::distance`, `std::accumulate`, and with `std::vector` built from its iterators; the iterator supports prefix and postfix `++`, dereference, `==` and `!=`, and copies by value.

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

### Rules

- The range contains every integer from `first` up to but not including `stop`, with step 1.
- When `first >= stop`, the range is empty and `begin() == end()`.
- Support range-`for`, `std::distance`, `std::accumulate`, and constructing a `std::vector` from the iterators.
- The iterator must support pre-increment, post-increment, dereference, `==`, and `!=`.
- Range objects are copyable, and iterating must not modify the range itself.
- Do not write `main`, and do not change the public interface.
