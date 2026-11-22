CROSSTALK — PASS BY VALUE AND PASS BY REFERENCE

A: Take a look here: when this `Data` goes into a function, what exactly goes in?

B: The thing itself — though it depends on how the parameter is written.

A: Suppose the parameter is written `Data data`. What arrives?

B: A copy. If the other fellow adds an entry to his copy, your copy has not one number more.

A: And if the parameter is written `Data& data`?

B: Then the man himself arrives. If he adds an entry to the man himself, your copy gains an entry too, and there is no avoiding it.

A: What about `const Data&`?

B: Still the man himself, but he may look and not touch, so your copy comes back exactly as it was.

A: Then what is the difference between returning `Data` and returning `const int&`?

B: Return `Data` and you receive fresh data of your own: change it and the original is untouched. Return `const int&` and you receive an alias of the original element; point it at the wrong place and the audience will boo you off the stage.

A: Well, with that, values, references and copies are all clear.

**In plain words**: Implement `total`, `append_copy`, `append_ref`, `make_data`, and `first_value`: a by-value parameter may only change its copy, a by-reference parameter changes the original, and `first_value` must return a reference to `values[0]`.

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

### Requirements

- `total` returns the sum of `values`; an empty vector gives `0`.
- `append_copy` takes its `Data` by value, appends `value` to its own copy, and must leave the caller's object untouched.
- `append_ref` takes its `Data` by reference, appends `value` to the original object, and keeps `id` unchanged.
- `make_data` returns an object whose `id` is the parameter and whose `values` are `0, 1, …, count - 1`; with `count == 0` the vector is empty.
- `first_value` returns a reference to `data.values[0]` (the caller guarantees a non-empty vector); it must not return a copy.
- A direct copy of `Data` must be independent: changing the copy's `id` or `values` must not affect the original.
- Do not write `main`.

### Hint

Copying a `std::vector` deep-copies its elements, so the default copy of `Data` is already independent. Watch the missing `&` in `append_copy`'s parameter and the `const int&` in `first_value`'s return type.
