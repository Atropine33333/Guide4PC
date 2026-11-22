A: I keep a counter here. It counts up, it counts down, and it has one stubborn habit — at zero it lies down and will go no lower.

B: Ah, the "never negative" sort.

A: It also has two ways of stepping up. The prefix `++c` adds one to itself first, then hands you itself. The postfix `c++` hands you a copy of the old value first, and only afterwards sneaks in the increment.

B: One hands over the body, the other hands over the shadow.

A: `+=` takes negative amounts; overshoot below zero and the result is zero, and back comes the object itself. Sounds easy — yet eight writers in ten type the postfix return as a reference.

B: And hand over a fake shadow.

A: That is the whole test here: prefix returns `Counter&`, postfix returns `Counter`, one character apart. Copying a counter must also give a separate counter with its own number.

**In plain words**: implement the counter class `Counter`: default construction gives 0 and a negative start also becomes 0; `increment` adds one, `decrement` subtracts one but never goes below 0, `reset` returns to 0, `value` reads it; prefix `++` adds one and returns a reference to the object, postfix `++` returns the **value** from before the increment with return type `Counter` (no reference), `+=` accepts negatives, clamps the result at 0 and returns a reference to the object.

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

### Rules

- A default-constructed counter is 0; `Counter(start)` also becomes 0 when `start < 0`.
- `increment` adds 1; `decrement` subtracts 1 but never goes below 0.
- `value` returns the current count; `reset` sets it to 0.
- Prefix `++` adds 1 and returns a reference to the object.
- Postfix `++` returns the **value** before incrementing (return type `Counter`, no reference), then increments the object.
- `+=` accepts negative amounts; the result is clamped at 0, and it returns a reference to the object.
- Copying a counter produces an independent value.
- Do not write `main`.

### Hint

Postfix `++` saves the old value in a temporary object and returns it. Keep returning a reference and returning a value strictly apart.
