Implement a rule-of-five resource class: it owns one heap `int` and provides complete copy, move, assignment, and destruction semantics. Complete this interface:

```cpp
class Resource {
public:
    explicit Resource(int value);
    Resource(const Resource& other);
    Resource(Resource&& other) noexcept;
    Resource& operator=(const Resource& other);
    Resource& operator=(Resource&& other) noexcept;
    ~Resource();

    int value() const;      // -1 when there is no resource
    void set(int value);    // does nothing when there is no resource

    static int live();      // number of resources currently owned
    static int copies();    // total copy constructions + copy assignments
    static int moves();     // total move constructions + move assignments

private:
    int* value_ = nullptr;
};
```

Requirements:

- Construction allocates one `int` and increments `live()`; destruction frees it and decrements `live()`.
- Copy construction and copy assignment allocate a fresh `int` and increment `copies()`.
- Move construction and move assignment take over the pointer, leave the source empty, and increment `moves()`; after a move the source's `value()` returns `-1`.
- Self-assignment (`a = a`) does nothing and leaves `copies()` unchanged.
- Move operations must be `noexcept`; otherwise `std::vector` falls back to copying.

The judge includes your implementation and calls these members. **Do not write `main`.**

### Input

None.

### Output

None. The judge checks return values and the static counters.

### Example

```cpp
Resource a(7);
Resource b(std::move(a));
// b.value() == 7, a.value() == -1, Resource::live() == 1
```

### Constraints

- Do not use `std::shared_ptr` or `std::unique_ptr`; manage the pointer yourself.
- Allocation failure need not be handled.
