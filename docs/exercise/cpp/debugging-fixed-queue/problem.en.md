This exercise reads no stdin and writes no stdout. The provided ring-buffer queue compiles, but its `push` behaves incorrectly when the queue is full. Find and fix the bug without changing the public interface. A hidden harness runs the tests.

```cpp
class FixedQueue {
public:
    explicit FixedQueue(int capacity);
    ~FixedQueue();

    FixedQueue(const FixedQueue&) = delete;
    FixedQueue& operator=(const FixedQueue&) = delete;

    bool push(int value);
    bool pop(int& value);
    int size() const;
    int capacity() const;
    void clear();

private:
    int* data_;
    int capacity_;
    int head_;
    int count_;
};
```

Correct behavior:

- `capacity >= 0`; with capacity 0, both `push` and `pop` return `false`.
- `push(value)`: when the queue is not full, append `value` at the back and return `true`; when full, leave the queue unchanged and return `false`.
- `pop(value)`: when non-empty, copy the front into `value`, remove it, and return `true`; when empty, return `false` and leave `value` unchanged.
- `size()` returns the current element count; `capacity()` returns the capacity; `clear()` empties the queue but keeps the internal array.
- When the queue is full, never overwrite an old element that has not been popped yet.
- Do not write `main`.

### Hint

The starter overwrites the front with the new value and returns `true` when full. Change it to reject new elements; then confirm that wraparound, clearing, and zero capacity still work.
