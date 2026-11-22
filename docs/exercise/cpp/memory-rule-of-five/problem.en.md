A: Tonight, a class called `Resource`, holding one heap `int` in one hand.

B: One hand, one hole.

A: And it must do five jobs: copy construction, move construction, copy assignment, move assignment, and destruction.

B: Five jobs, no absences.

A: Copying first. After `Resource b(a);` the two gentlemen each carry their own `int`; `copies()` records one and `live()` goes up by one.

B: Separate households, no shared keys.

A: Moving is another trade. `std::move` arrives, the pointer takes the luggage and leaves, and the source stands empty: `value()` reports `-1`, `moves()` records one.

B: And leave the old key behind.

A: One more place where people trip: both move operations must be marked `noexcept`.

B: And if they aren't?

A: Then `std::vector` reads the résumé, sees no promise of staying put, and copies instead — every time.

B: So the class scared off the container with its own paperwork.

A: Self-assignment needs care too. `a = a` behaves as if nothing happened, and `copies()` stays put.

B: Why throw out your own goods only to pick them up again.

A: It is all on this sheet.

**In plain words**: implement `Resource`. Construction allocates one `int` and increments `live()`; destruction frees it and decrements `live()`. Copy construction and copy assignment allocate a fresh `int` and increment `copies()`. Move construction and move assignment take the pointer, leave the source empty, and increment `moves()`; after a move the source's `value()` is `-1`. Self-assignment does nothing and leaves `copies()` unchanged. Both move operations are `noexcept`.

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
- **Do not write `main`.**
