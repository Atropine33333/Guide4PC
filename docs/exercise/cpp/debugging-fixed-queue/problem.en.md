Ring Buffer Management Office Document

Huan-dui-ban fa [2026] No. 13

Notice on the Special Rectification of Fixed-Capacity Ring Queues

To all heads and all tails:

In order to standardise the enqueue, dequeue, clear and wraparound behaviour of fixed-capacity queues, and to put an end to full-queue overwriting, misreported capacity, and old elements being displaced by new ones, a special rectification has been decided upon. The relevant matters are hereby notified as follows.

1. Every unit shall implement `FixedQueue` according to the following interface, and shall not add or remove member functions on its own authority.

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

2. `capacity >= 0`; when the capacity is 0, both `push` and `pop` return `false`.

3. `push(value)`: when the queue is not full, place `value` at the back and return `true`; when it is full, leave the queue untouched and return `false`.

4. `pop(value)`: when the queue is not empty, copy the front into `value`, remove it, and return `true`; when it is empty, return `false` and leave `value` as it was.

5. `size()` shall truthfully report the current element count; `capacity()` shall truthfully report the capacity; `clear()` shall empty the queue without releasing the internal array.

6. When the queue is full, no old element that has not yet been dequeued may be overwritten. This is the focus of the present rectification.

7. Do not write `main`.

This notice takes effect from the day of issue.

Ring Buffer Management Office
22 November 2026

Copy to: Stack Management Committee; Dynamic Array Management Committee.

**In plain words**: finish the `FixedQueue` implementation: a ring array with `head_` for the front position and `count_` for the current element count, wrapping writes at `(head_ + count_) % capacity_`. When full, `push` returns `false` without touching the queue; when empty, `pop` returns `false`. The starter overwrites the front with the new value and returns `true` when full; change that, then confirm wraparound, clearing, and zero capacity still behave.

### Hint

Test fullness with `count_ == capacity_` before writing anything; `clear()` only needs to reset `head_` and `count_` to zero.
