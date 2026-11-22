Hui Shi said to Zhuangzi: "I have a tree, called a binary search tree. When a number enters, the lesser goes left of the current node, the greater goes right, and an equal is turned away. Each node keeps its own place."

Zhuangzi said: "How many levels has your tree? Can you count them yourself?"

Hui Shi said: "An empty tree is zero, and a single root is one. But if I put numbers in ascending order, one after another, the tree leans into a long chain, and its levels equal its nodes. What of that?"

Zhuangzi said: "Leaning or straight, still the levels must be counted one upon another. Ask from the root: 'How tall is this subtree of yours?' The root answers: 'One level above the taller of my two sides.' The leaf answers: 'I owe not one level.' Ask down to the leaves, and the number comes out. To count is, after all, to hand your own tree to your left and right subtrees and let them count."

Hui Shi said: "I would also report the numbers in order."

Zhuangzi said: "Then the in-order way. The left is reported, then oneself, then the right. And when the reporting ends, the whole tree has been walked once."

**In plain words**: implement `IntBST`, a binary search tree of `int`; it reads no stdin and writes no stdout, and a hidden harness runs the tests. `insert` returns `true` for a new value and `false` if it is already present; `contains` looks a value up; `size` returns the node count; `height` returns the number of levels, 0 for an empty tree and 1 for a root alone; `inorder` appends ascending values to `out` without clearing it first. Duplicates are not allowed; manage node memory yourself and free every node in the destructor; do not write `main`. Recursive insertion and recursion for height are both natural, and a tree degenerating into a chain (for example, inserting in ascending order) is allowed, with height equal to size.

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
