And the LORD spake unto Moses, saying:

"Take for each tribe of Israel one portion. The number of portions is `N`, fixed from the beginning; on the road out of Egypt it shall neither grow nor shrink."

"Each portion is empty at first. Whole numbers are zero, floating numbers are zero; so they shall know that the portion comes before its contents, and the storehouse before the grain."

"Let every portion be taken in order. `operator[]` knows only the places already fixed. If one asks for a place outside the border, `try_get` shall not give it to him, nor touch the vessel in his hand; it shall answer only `false`."

"Thou shalt store in a storehouse that is truly fixed, and not in a skin that stretches of itself. `T items_[N]` is such a storehouse; when `{}` falls upon it, every portion is empty."

"An empty portion must still be fillable. One call of `fill`, and all the store is alike."

And Moses did as the LORD commanded him.

**In plain words**: implement the class template `FixedArray<T, N>` with real fixed storage, default construction that value-initializes all elements, and a `try_get` that checks `0 <= index < N`.

```cpp
template <typename T, int N>
class FixedArray {
public:
    FixedArray();       // value-initialize all N elements to T{}

    int size() const;   // returns N

    T& operator[](int index);
    const T& operator[](int index) const;

    void fill(const T& value);  // set every element to value

    // Return false without touching out when out of range; otherwise store and return true.
    bool try_get(int index, T& out) const;

    T* data();
    const T* data() const;
};
```

Requirements:

- Use real fixed storage (such as `T items[N]`), not a dynamically allocated `std::vector`;
- Default construction of `FixedArray<T, N>` must initialize `int` elements to 0 and `double` elements to 0.0;
- `operator[]` assumes a valid index; `try_get` must check `0 <= index < N`;
- Do not write `main`.

Hint: `T items_[N]{};` performs value initialization.
