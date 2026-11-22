**THE BOOK OF THE SHARED NUMBER**

Jesus sat by the sea, and a great crowd gathered before him, and he spoke to them in a parable:

"The kingdom of heaven is like a master who handed one estate to many people. They keep one storehouse between them, and in that storehouse there is one number and one ledger."

His disciples asked, "Lord, how shall the ledger be kept?"

Jesus said, "Each time a man receives the estate, the ledger adds one. Whoever gives the estate to another first settles what he himself holds, then enters the new account; when a man gives to himself, the ledger does not move by a single coin. Whoever changes the number through that storehouse, all the others see the new number, for the storehouse is one."

"When the time comes, every man shall give account. When the count falls to zero, the storehouse is torn down, and dust returns to dust."

**In plain words**: implement `SharedInt`. Each object stores only a pointer to a control block; the block holds one `int` value and a reference count. Copy construction and copy assignment increment the count and share the value. Assignment releases the left operand's old reference before sharing the right operand's data, and self-assignment must be safe. The destructor decrements the count and frees the control block only at zero. Calling `set_value` through any copy is visible to all the others. Do not write `main`.

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

And it is written: a private nested `struct Control { int value; int count; };` with the object holding only a `Control*` works well.
