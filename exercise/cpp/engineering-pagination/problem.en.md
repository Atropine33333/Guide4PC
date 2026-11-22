Implement a pager that exposes page numbers and item ranges:

```cpp
class Pager {
public:
    Pager(int total_items, int page_size, int current_page);

    int page_count() const;      // total number of pages
    int current() const;         // current page, 1-based
    bool has_prev() const;
    bool has_next() const;

    int first_index() const;     // 0-based index of the first item on this page; -1 when empty
    int last_index() const;      // 0-based index of the last item on this page; -1 when empty

    std::vector<int> window(int radius) const;  // page numbers around the current page
};
```

Invariants:

- `page_size` is at least 1; `total_items == 0` means there is no data.
- `current_page` may be any value, but internally it must be clamped to `[1, page_count]`; with no data, `page_count() == 0` and `current() == 1`.
- `window(r)` returns every page number from `current() - r` through `current() + r` in ascending order, clipped to the valid range; a negative `r` is treated as 0.

**Do not write `main`.**

### Input

None.

### Output

None.

### Example

```cpp
Pager pager(95, 10, 5);
pager.page_count();   // 10
pager.first_index();  // 40
pager.window(2);      // {3, 4, 5, 6, 7}
```

### Constraints

- `total_items` is at most `10^9` and `page_size` at most `10^6`.
- The `window` radius is at most `10^4`.
