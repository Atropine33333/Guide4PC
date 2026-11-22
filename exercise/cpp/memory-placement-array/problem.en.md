This task reads no stdin and writes no stdout. Implement a fixed-capacity array whose elements exist only after placement `new`.

```cpp
#include <cstddef>

struct Tracker {
    explicit Tracker(int value);
    ~Tracker();

    int value() const;
    static int alive();
    static int constructions();
    static int destructions();

    Tracker(const Tracker&) = delete;
    Tracker& operator=(const Tracker&) = delete;
};

class TrackerBuffer {
public:
    explicit TrackerBuffer(std::size_t capacity);
    ~TrackerBuffer();

    TrackerBuffer(const TrackerBuffer&) = delete;
    TrackerBuffer& operator=(const TrackerBuffer&) = delete;

    bool push(int value);
    std::size_t size() const;
    Tracker& operator[](std::size_t index);
    const Tracker& operator[](std::size_t index) const;
};
```

Rules:

- `Tracker` increments `alive()` and `constructions()` when constructed; it decrements `alive()` and increments `destructions()` when destroyed.
- `TrackerBuffer` allocates only raw storage; do not use `new Tracker[n]` or `std::vector<Tracker>` to preconstruct elements.
- `push` placement-constructs one element and returns `true` if there is room; when full, return `false` and construct nothing.
- `size()` returns the current element count; `operator[]` accesses constructed elements.
- `TrackerBuffer`'s destructor must explicitly destroy every element; once the last buffer element dies, `alive()` returns to its prior value.
- The storage must satisfy `alignof(Tracker)`.
- Do not write `main`, and do not change the public interface.
