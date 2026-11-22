NOTICE ON THE IMPLEMENTATION OF THE MIN-HEAP

To all implementation units:

For the sake of a uniform standard in the implementation of the min-heap, the following matters are hereby notified.

Article 1. The min-heap referred to in this notice is one written by the unit itself; calling `std::priority_queue` is prohibited.
Article 2. The five members `push`, `pop`, `top`, `size`, and `empty` shall be implemented; their signatures follow the interface in the attachment and shall neither be added to nor removed.
Article 3. `pop` removes and returns the current minimum; `top` reads it without removal. Both are called only on a non-empty heap.
Article 4. The number of elements shall not exceed 100000; any value within the `int` range, negatives included, shall be accepted.
Article 5. The heap-order invariant shall hold after every `push` and `pop`: no node is greater than either of its children.
Article 6. The judge includes the implementation and calls its members directly; it reads no stdin and writes no stdout, and the unit shall not supply its own `main`.
Article 7. The judge generates random operations from fixed seeds and compares the results against a multiset.
Article 8. The usage example in the attachment may serve for self-testing.

Please comply.

**In plain words**: write your own min-heap without `std::priority_queue` and complete the `MinHeap` members `push`, `pop`, `top`, `size`, and `empty`; the judge includes your implementation and calls them directly. At most 100000 elements, any `int` value including negatives; `pop` and `top` are called only on a non-empty heap; the heap-order invariant must hold after every `push` and `pop`, and the judge generates fixed-seed random operations and compares against a multiset. It reads no stdin and writes no stdout — **do not write `main`**.

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
