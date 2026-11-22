This exercise reads no stdin. A hidden harness calls the functions below and checks whether you can tell values, references and copies apart.

```cpp
#include <vector>

struct Data {
    int id;
    std::vector<int> values;
};

int total(const Data& data);
void append_copy(Data data, int value);
void append_ref(Data& data, int value);
Data make_data(int id, int count);
const int& first_value(const Data& data);
```

Rules:

- `total` returns the sum of `values`; an empty vector gives `0`.
- `append_copy` takes its `Data` by value, appends `value` to its own copy, and must leave the caller's object untouched.
- `append_ref` takes its `Data` by reference, appends `value` to the original object, and keeps `id` unchanged.
- `make_data` returns an object whose `id` is the parameter and whose `values` are `0, 1, …, count - 1`; with `count == 0` the vector is empty.
- `first_value` returns a reference to `data.values[0]` (the caller guarantees a non-empty vector); it must not return a copy.
- A direct copy of `Data` must be independent: changing the copy's `id` or `values` must not affect the original.
- Do not write `main`.

### Hint

Copying a `std::vector` deep-copies its elements, so the default copy of `Data` is already independent. Watch the missing `&` in `append_copy`'s parameter and the `&` in `first_value`'s return type.
