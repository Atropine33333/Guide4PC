THE STORYTELLER'S HALL — THE TABLE OF FUNCTION POINTERS

The storyteller strikes the block: "Ladies and gentlemen, today we set aside swords and horses and speak of a single table. Four rows it has, and each row a hero: one named `add`, who wields addition; one named `sub`, who keeps subtraction; one named `mul`, mighty in products; one named `min`, who takes only the lesser. Their names stand on the left, their deeds on the right, and such a register of deeds is called a table of function pointers."

"You ask what happens when a name is not on the roll? The answer: hand back a null pointer, as one reports that the man is not enrolled. Whoever takes that null pointer and bids it work is asking for trouble."

The block sounds again: "And here the tale pauses."

**In plain words**: Use function pointers to build a table mapping names to binary functions: `lookup_op` returns `op_add`, `op_sub`, `op_mul`, or `op_min` for the matching name and `nullptr` otherwise; `apply_op` calls the function when found and returns `fallback` when not, never dereferencing a null pointer; `apply_all` calls every non-null pointer in `ops` in order and collects the results, skipping nulls.

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

### Rules

- `lookup_op` knows `"add"`, `"sub"`, `"mul"`, and `"min"`, returning the matching function pointer; any other name returns `nullptr`.
- `apply_op` computes `a` and `b` with the named function; for an unknown name it returns `fallback` and must never dereference a null pointer.
- `apply_all` calls every non-null function in `ops` in order and appends each result; null pointers are skipped.
- Input range `|a|, |b| <= 1000`, so the product fits in `10^6`.
- Do not write `main`.

### Hint

A linear scan over an array of four function pointers is fine, and so is an `if` chain; remember that a function pointer converts to `bool`.
