This task reads no stdin and writes no stdout. Implement a copy-on-write string buffer.

```cpp
#include <cstddef>
#include <string>

class CowBuffer {
public:
    CowBuffer();
    explicit CowBuffer(const std::string& data);
    CowBuffer(const CowBuffer& other);
    CowBuffer& operator=(const CowBuffer& other);
    ~CowBuffer();

    const std::string& str() const;
    void append(const std::string& suffix);
    std::size_t shares() const;

    static int live_blocks();
};
```

Rules:

- Copy construction and copy assignment share the underlying storage; they do not copy the string and do not increase `live_blocks()`.
- `shares()` returns how many `CowBuffer` objects share this storage, including the object itself (at least 1).
- `append` must first ensure unique storage: if `shares() > 1`, clone the storage before appending. The other sharing object's string must not change.
- `live_blocks()` returns the number of distinct storage blocks that are currently alive, not the number of objects.
- Copy assignment must handle self-assignment correctly; release replaced storage as soon as no object uses it.
- Even an empty buffer must own a valid storage block.
- Do not write `main`, and do not change the public interface.
