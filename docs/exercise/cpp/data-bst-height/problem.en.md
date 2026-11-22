This exercise reads no stdin and writes no stdout. Implement a binary search tree of `int`; a hidden harness runs the tests.

Implement:

```cpp
class IntBST {
public:
    IntBST();
    ~IntBST();
    IntBST(const IntBST&) = delete;
    IntBST& operator=(const IntBST&) = delete;

    bool insert(int value);   // true for a new value, false if present
    bool contains(int value) const;
    int size() const;
    int height() const;       // 0 for an empty tree, 1 for a root only
    void inorder(std::vector<int>& out) const;  // append ascending values
};
```

Requirements:

- Duplicate values are not allowed;
- `height()` is the number of node levels: 0 when empty and 1 with only a root;
- `inorder` appends to `out` without clearing it first;
- Manage node memory yourself and release every node in the destructor; do not write `main`.

Hint: recursive insertion and height are natural. The tree may degenerate into a chain (for example, inserting in ascending order); that is allowed, and its height then equals its size.
