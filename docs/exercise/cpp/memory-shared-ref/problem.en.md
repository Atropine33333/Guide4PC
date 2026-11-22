This exercise reads no stdin and writes no stdout. Implement a reference-counted shared integer; a hidden harness runs the tests.

Implement:

```cpp
class SharedInt {
public:
    explicit SharedInt(int value = 0);
    SharedInt(const SharedInt& other);
    SharedInt& operator=(const SharedInt& other);
    ~SharedInt();

    int value() const;
    void set_value(int value);
    int use_count() const;  // objects sharing this data
};
```

Requirements:

- Each object shares one `int` value and a reference count through a control block;
- Copy construction and copy assignment increment the count and share the value;
- Assignment must release the left operand's old reference before sharing the right operand's data; self-assignment must be safe;
- The destructor decrements the count and frees the control block only at zero;
- Calling `set_value` through any copy is visible to the others;
- Do not write `main`.

Hint: a private nested `struct Control { int value; int count; };` with the object holding only a `Control*` works well.
