Implement a small dynamic-array class with correct copy, assignment, and destruction. Complete this interface:

```cpp
class IntBuffer {
public:
    explicit IntBuffer(std::size_t size);            // allocate size ints, all zero
    IntBuffer(const IntBuffer& other);               // deep copy
    IntBuffer& operator=(const IntBuffer& other);    // deep copy, self-assignment safe
    ~IntBuffer();                                    // free the memory

    std::size_t size() const;
    int& at(std::size_t index);
    const int& at(std::size_t index) const;

private:
    int* data_ = nullptr;
    std::size_t size_ = 0;
};
```

After copying or assigning, the two objects must own independent memory; the destructor frees memory exactly once; `size == 0` is allowed and allocates nothing.

**Do not write `main`.**

### Input

None.

### Output

None.

### Example

```cpp
IntBuffer a(3);
a.at(0) = 7;
IntBuffer b(a);      // deep copy
b.at(0) = 9;
// a.at(0) is still 7
```

### Constraints

- Indices are always in range.
- Move semantics are not required.
- Checks compare values only, never addresses.
