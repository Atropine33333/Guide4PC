A: Tonight a class called `IntView`. This gentleman raises no children; he only keeps an eye on yours.

B: And how does he watch them?

A: Hand him an `int* data` and a `count`. If `data` is null, or `count <= 0`, he shrugs and reports an empty view: `data()` gives `nullptr`, `size()` gives 0.

B: Frugal. Saves on rice.

A: A view copies no data; it pockets the pointer and the length. So a write through `operator[]` lands in the original array — same household, after all.

B: Borrows your pot to cook his own dinner.

A: `slice(begin, count)` is even simpler: cut a stretch from `begin`, still the same memory, sharing one pair of trousers with the original view, while the original does not stir by a hair.

B: And if the cut runs past the wall?

A: On `begin < 0`, `count < 0`, `begin >= size()`, `begin + count > size()`, or `count == 0`, he hands back an empty view without a word.

B: A wasted trip, then.

A: Indexes passed to `operator[]` are guaranteed valid, so take them without a gate; `empty()` reports `true` as soon as `size() == 0`.

B: Neatly done.

**In plain words**: implement `IntView`. The default constructor makes an empty view, and so does passing `data == nullptr` or `count <= 0` to the other constructor. A view stores only a pointer and a length; it copies no data, so writing through a view writes the original array. `slice(begin, count)` returns a new view over the same memory and leaves the original view untouched; an out-of-range slice or `count == 0` yields an empty view. Indexes passed to `operator[]` are guaranteed valid, so no bounds check is needed. This exercise reads no stdin and writes no stdout; do not write `main`.

```cpp
class IntView {
public:
    IntView();
    IntView(int* data, int count);

    int* data() const;
    int size() const;
    bool empty() const;

    int& operator[](int index);
    const int& operator[](int index) const;

    IntView slice(int begin, int count) const;
};
```
