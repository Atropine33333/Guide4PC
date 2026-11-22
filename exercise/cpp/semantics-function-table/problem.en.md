This exercise reads no stdin. Implement a table that maps names to binary functions; a hidden harness calls them.

```cpp
#include <string>
#include <vector>

using BinOp = int (*)(int, int);

int op_add(int a, int b);   // a + b
int op_sub(int a, int b);   // a - b
int op_mul(int a, int b);   // a * b
int op_min(int a, int b);   // the smaller one

BinOp lookup_op(const std::string& name);
int apply_op(const std::string& name, int a, int b, int fallback);
std::vector<int> apply_all(const std::vector<BinOp>& ops, int a, int b);
```

Rules:

- `lookup_op` knows `"add"`, `"sub"`, `"mul"`, and `"min"`, returning the matching function pointer; any other name returns `nullptr`.
- `apply_op` computes `a` and `b` with the named function; for an unknown name it returns `fallback` and must never dereference a null pointer.
- `apply_all` calls every non-null function in `ops` in order and appends each result; null pointers are skipped.
- Input range `|a|, |b| <= 1000`, so the product fits in `10^6`.
- Do not write `main`.

### Hint

A linear scan over an array of four function pointers is fine, and so is an `if` chain; remember that a function pointer converts to `bool`.
