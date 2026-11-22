Implement your own min-heap (do not use `std::priority_queue`) by completing this class:

```cpp
class MinHeap {
public:
    void push(int value);
    int pop();                 // remove and return the minimum
    int top() const;           // return the minimum without removing
    std::size_t size() const;
    bool empty() const;
};
```

The judge includes your implementation and calls these members directly. **Do not write `main`.**

### Input

None.

### Output

None. The judge inspects return values.

### Example

```cpp
MinHeap heap;
heap.push(5);
heap.push(1);
heap.push(3);
heap.pop();   // returns 1
heap.top();   // returns 3
```

### Constraints

- At most 100000 elements.
- Any `int` value, including negatives.
- `pop` and `top` are only called on non-empty heaps.
- The judge uses fixed-seed random operations and compares against a multiset.
