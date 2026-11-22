This exercise reads no stdin and writes no stdout. Implement a counter class; a hidden harness runs the tests.

```cpp
class Counter {
public:
    Counter();
    explicit Counter(int start);

    void increment();
    void decrement();
    int value() const;
    void reset();

    Counter& operator++();
    Counter operator++(int);
    Counter& operator+=(int amount);
};
```

Rules:

- A default-constructed counter is 0; `Counter(start)` also becomes 0 when `start < 0`.
- `increment` adds 1; `decrement` subtracts 1 but never goes below 0.
- `value` returns the current count; `reset` sets it to 0.
- Prefix `++` adds 1 and returns a reference to the object.
- Postfix `++` returns the **value** before incrementing (return type `Counter`, not a reference), then increments the object.
- `+=` accepts negative amounts; the result is clamped at 0, and it returns a reference to the object.
- Copying a counter produces an independent value.
- Do not write `main`.

### Hint

Postfix `++` saves the old value in a temporary object and returns it. Be careful to distinguish returning a reference from returning a value.
