Implement an accumulator that collects integers in fixed-size batches.

```cpp
#include <cstddef>
#include <vector>

class BatchAccumulator {
public:
    explicit BatchAccumulator(std::size_t batch_size);

    bool add(int value);
    std::vector<int> take_batch();
    std::vector<int> flush();

    std::size_t pending() const;
    std::size_t emitted() const;
};
```

Rules:

- A `batch_size` below 1 is treated as 1.
- `add` appends and returns `true` while fewer than `batch_size` values are pending. When a full batch is already pending, return `false` and add nothing.
- `take_batch` returns a full batch in insertion order and clears it only when exactly `batch_size` values are pending; otherwise return an empty `vector` and change nothing.
- `flush` returns every currently pending value (possibly a partial batch or empty) and clears them.
- `pending()` returns the current pending count.
- `emitted()` returns the total number of values actually returned by `take_batch` and `flush`.
- Do not write `main`, and do not change the public interface.
