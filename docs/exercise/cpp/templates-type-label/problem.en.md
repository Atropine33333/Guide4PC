This exercise reads no stdin and writes no stdout. Implement a type label through template specialization; a hidden harness runs the tests.

Implement a primary template and a family of specializations:

```cpp
template <typename T>
struct TypeLabel {
    static const char* name();  // "other" for the primary template
    static int rank();          // 0 for the primary template
};
```

Specialization rules:

| Type | `name()` | `rank()` |
| --- | --- | --- |
| `int` | `"int"` | 1 |
| `double` | `"double"` | 2 |
| `bool` | `"bool"` | 3 |
| any pointer `T*` | `"pointer"` | 4 |
| anything else | `"other"` | 0 |

Requirements:

- Write header-style definitions only; do not write `main`;
- The pointer rule must work for `int*`, `double*`, and `const char*`;
- Labels are chosen at compile time; do not use `typeid` or runtime string comparisons;
- Strings returned by `name()` must live at least until the program ends.
